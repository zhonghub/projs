#include "server.h"

Server* Server::instance = nullptr;

Server* Server::getInstance() {
    if (instance == nullptr) // 如果实例尚未创建
    {
        instance = new Server(); // 创建一个新实例
    }
    return instance; // 返回现有实例
}

void Server::setWindow(MyWindow* m) {
    this->myWindow = m;
    //std::thread receiveThread(&Server::main_server, this);
    //receiveThread.detach();
}

// 添加新用户到在线用户列表
void Server::AddUser(SOCKET clientSocket, const std::string& username) {
    UserInfo user;
    user.clientSocket = clientSocket;
    user.username = username;
    onlineUsers.push_back(user);
    // 更新UI
    std::string newstr = getAllOnlineUsers();
    // 发射自定义信号来触发GUI更新
    std::lock_guard<std::mutex> lock1(guiMutex2);
    QMetaObject::invokeMethod(myWindow, "updateGui2", Qt::QueuedConnection,
        Q_ARG(QString, QString::fromStdString(newstr)));
}

// 根据套接字查找用户名
std::string Server::getAllOnlineUsers() {
    std::string all_user;
    for (const auto& user : onlineUsers) {
        all_user.append(user.username + "\n");
    }
    return all_user;
}

/*
根据接收消息对数据库进行操作，并返回操作结果
recvStr[0], 请求编号： 
    0：专门接收新消息的套接字
    1：登录
    2：注册
    3：发布消息
    4：请求全部历史消息
recvStr[1], 用户名 
recvStr[2]，密码
recvStr[3]，时间
*/
std::string Server::operateDatabase(std::vector<std::string> recvStr) {
    // 进行数据库操作
    SQL_con* sql_con = SQL_con::getInstance();
    // 回复字符串（数据库操作结果），默认操作结果成功
    std::string replyString = "ok";
    if (recvStr.size() > 3) {
        if (recvStr[0]._Equal("1")) {
            bool res = sql_con->is_exist(recvStr[1]);
            if (!res) {
                // 登录失败
                replyString = "Log in error! The user is not exist!\n";
            }
            else {
                // 登录
                replyString = sql_con->check_password(recvStr[1], recvStr[2]);
            }
        }
        else if (recvStr[0]._Equal("2")) {
            // 注册
            bool res = sql_con->insert_sql("users", recvStr[1], recvStr[2], recvStr[3]);
            // 注册失败
            if (!res) {
                replyString = "Register error! The user is exist!\n";
            }
        }
        else if (recvStr[0]._Equal("3")) {
            // 发布消息
            bool res = sql_con->insert_sql("msg", recvStr[1], recvStr[2], recvStr[3]);
            if (!res) {
                replyString = "Send msg error!\n";
            }
        }
        else if (recvStr[0]._Equal("4")) {
            // 返回全部历史消息
            replyString = sql_con->select_all_msg();
        }
    }
    return replyString;
}

/*
HandleClient 函数是用来处理与单个客户端的通信的函数。
它在一个单独的线程中执行，负责接收来自客户端的消息并返回对应处理结果。

每当有新的客户端连接时，都会为其创建一个独立的线程来调用 HandleClient 函数。

每当有新的监听新消息的客户端连接时(recvStr[0]._Equal("3"))，会将这个套接字记录到clients，

同时也处理客户端断开连接的情况，将其从clients中移除。

*/
void Server::HandleClient(SOCKET clientSocket) {
    char buffer[1024];
    int bytesReceived;
    // 循环监听消息
    while (true) {
        bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            // Handle client disconnect or error
            break;
        }

        buffer[bytesReceived] = '\0';
        std::string recvString(buffer);
        std::cout << "Received message: " << recvString << std::endl;
        // 将接收消息分割成各个子部分
        std::vector<std::string> recvStr = my_sql_use::splitString(recvString, '#');
        // 0表示这是一个专门接收新消息的套接字
        if (recvStr[0]._Equal("0")) {
            {
                std::lock_guard<std::mutex> lock(clientsMutex);
                if (onlineUsers.size() >= MAX_CLIENTS) {
                    std::cerr << "Max number of clients reached." << std::endl;
                    closesocket(clientSocket);
                    continue;
                }
                // 当前连接数小于最大连接数时，这个连接建立成功
                AddUser(clientSocket, recvStr[1]);
            }
        }
        // 回复字符串
        std::string replyString = operateDatabase(recvStr);
        if (recvStr[0]._Equal("3")) {
            std::unique_lock<std::mutex> lock(batchMutex);
            // 发布新消息
            std::string newStr = recvStr[3] + "  " + recvStr[1] + ": " + recvStr[2];
            messageBatch.push_back(newStr); //("msg", recvStr[1], recvStr[2], recvStr[3]);

            // 更新UI
            newMsg.append(newStr + "\n");
            std::lock_guard<std::mutex> lock2(guiMutex1);
            QMetaObject::invokeMethod(myWindow, "updateGui1", Qt::QueuedConnection,
                Q_ARG(QString, QString::fromStdString(newMsg)));

            // 通过条件变量batchCondition 唤醒新消息广播线程SendBatchedMessages
            if (messageBatch.size() >= MESSAGE_BATCH_SIZE) {
                batchCondition.notify_one();
            }
        }
        // 回复客户端
        // send(clientSocket, replyString.c_str(), replyString.length(), 0);
        SendLongMessages(clientSocket, replyString);
    }

    // 套接字连接断开
    {
        std::lock_guard<std::mutex> lock(clientsMutex);
        // 从 onlineUsers向量中移除已经断开连接的客户端套接字
        // std::remove_if 算法会将满足条件的元素移到向量的末尾，并返回指向最后一个满足条件的元素的迭代器。
        // 然后，erase 函数会删除从该迭代器位置到向量末尾的所有元素。
        auto it = std::remove_if(onlineUsers.begin(), onlineUsers.end(),
            [clientSocket](const UserInfo& user) {
                return user.clientSocket == clientSocket;
            });
        if (it != onlineUsers.end()) {
            onlineUsers.erase(it, onlineUsers.end());
            // 更新UI
            std::string newstr = getAllOnlineUsers();
            // 发射自定义信号来触发GUI更新
            std::lock_guard<std::mutex> lock3(guiMutex2);
            QMetaObject::invokeMethod(myWindow, "updateGui2", Qt::QueuedConnection,
                Q_ARG(QString, QString::fromStdString(newstr)));
        }
    }
    // 关闭套接字
    closesocket(clientSocket);
}

/*
发送长字符串
*/
bool Server::SendLongMessages(SOCKET clientSocket, std::string replyString) {
    const int chunkSize = 1024; // 每个数据块的大小
    for (int i = 0; i < replyString.size(); i += chunkSize) {
        int bytesToSend = chunkSize < static_cast<int>(replyString.size() - i) ? chunkSize : static_cast<int>(replyString.size() - i);
        std::string chunk = replyString.substr(i, bytesToSend);
        int bytesSent = send(clientSocket, chunk.c_str(), bytesToSend, 0);
        if (bytesSent == SOCKET_ERROR) {
            // 处理发送错误
            break;
        }
    }
    return true;
}


/*
SendBatchedMessages 函数是用来将积累的消息批量发送给所有客户端的函数。
它在另一个独立的线程中执行，定期检查是否有足够的消息积累，如果满足条件，
则将消息批量发送给所有客户端。这样可以减少网络通信的开销，提高效率

lambda https://www.cnblogs.com/yajiu/p/15693724.html

*/
void Server::SendBatchedMessages() {
    while (true) {
        std::unique_lock<std::mutex> lock(batchMutex);
        // 通过条件变量batchCondition 等待新消息
        auto& refx_x1 = messageBatch;
        auto& refx_x2 = MESSAGE_BATCH_SIZE;
        batchCondition.wait(lock, [&refx_x1, &refx_x2] { return refx_x1.size() >= refx_x2; });
        // 清空
        std::vector<std::string> messagesToSend(messageBatch.begin(), messageBatch.end());
        messageBatch.clear();
        // 广播给所有在线用户
        for (const auto& user : onlineUsers) {
            for (const std::string& message : messagesToSend) {
                send(user.clientSocket, message.c_str(), static_cast<int>(message.size()), 0);
            }
        }
    }
}


/*
 `main` 函数的思路是：
创建服务器套接字并开始监听连接，然后通过创建多线程来处理客户端的通信。
1. 主线程使用HandleClient子线程 处理每个客户端的消息收发，
2. 另一个SendBatchedMessages线程 定期发送批量消息给所有客户端。
从而服务器能够并发处理多个客户端的连接和通信。
*/
int Server::main_server() {
    // 获取当前线程
    // QThread* currentThread = QThread::currentThread();
    // 将对象移动到当前线程
    // myWindow->moveToThread(currentThread);

    // 1. 初始化 Winsock：通过调用 `WSAStartup` 函数，初始化 Windows 套接字库，确保能够在程序中使用网络功能。
    WSADATA wsData;
    if (WSAStartup(MAKEWORD(2, 2), &wsData) != 0) {
        std::cerr << "Failed to initialize Winsock." << std::endl;
        return 1;
    }
    // 2. 创建服务器套接字
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create server socket." << std::endl;
        WSACleanup();
        return 1;
    }
    // 3. 绑定服务器套接字
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Failed to bind server socket." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    // 4. 开始监听：使用 `listen` 函数开始监听连接请求，等待客户端的连接。
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Failed to listen on server socket." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    std::cout << "Server is listening on port " << PORT << "..." << std::endl;

    // 5. 创建新消息广播线程SendBatchedMessages
    std::thread batchThread(&Server::SendBatchedMessages,this);
    batchThread.detach();

    // 6. 循环等待客户端连接，监听新的客户端套接字连接
    while (true) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Failed to accept client connection." << std::endl;
            continue;
        }
        // 为每个客户端套接字连接分配一个HandleClient线程处理
        std::thread clientThread(&Server::HandleClient,this, clientSocket);
        clientThread.detach();
    }
    // 7. 清理资源
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
