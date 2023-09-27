#include "z_include/server.h"

Server* Server::instance = nullptr;

Server::Server() {
    std::cout << "new server()";

    myWindow = nullptr;
    shouldExit = true;

    key.push_back("code");
    key.push_back("username");
    key.push_back("password");
    key.push_back("time");
    key.push_back("msg");
}

Server* Server::getInstance() {
    if (instance == nullptr) // ���ʵ����δ����
    {
        instance = new Server(); // ����һ����ʵ��
    }
    return instance; // ��������ʵ��
}

// ������û��������û��б�
void Server::AddUser(SOCKET clientSocket, const std::string& username) {
    std::cout << "AddUser = " << username << "\n";
    UserInfo user;
    user.clientSocket = clientSocket;
    user.username = username;
    onlineUsers.push_back(user);
    // ����UI
    std::string newstr = getAllOnlineUsers();
    // �����Զ����ź�������GUI����
    std::lock_guard<std::mutex> lock1(guiMutex2);
    QMetaObject::invokeMethod(myWindow, "updateGui2", Qt::QueuedConnection,
        Q_ARG(QString, QString::fromStdString(newstr)));
}

// �����׽��ֲ����û���
std::string Server::getAllOnlineUsers() {
    std::string all_user;
    for (const auto& user : onlineUsers) {
        all_user.append(user.username + "\n");
    }
    return all_user;
}

/*
* 
�ͻ��˷��͵�json�ַ�����keyֵ�����5����
key = ["code",username","password","time","msg"];

���ݽ�����Ϣ��codeֵ�����ݿ������Ӧ�����������ز������
code, �����ţ� 
    0��ר�Ž�������Ϣ���׽���
    1����¼
    2��ע��
    3��������Ϣ
    4������ȫ����ʷ��Ϣ
username, �û��� 
password������
time��ʱ��
msg, ��Ϣ

*/

std::string Server::operateDatabase(std::map<std::string, std::string>& map2) {
    // �������ݿ����
    SQL_con* sql_con = SQL_con::getInstance();
    // �ظ��ַ��������ݿ�����������Ĭ�ϲ�������ɹ�
    std::string replyString = "ok";
    // std::cout << "code = " << map2["code"] << std::endl;
    int code = std::stoi(map2["code"]);
    bool res;
    switch (code) {
    case 0:

        break;
    case 1:
        res = sql_con->is_exist(map2["username"]);
        if (!res) {
            // ��¼ʧ��
            replyString = "Log in error! The user is not exist!\n";
        }
        else {
            // ��¼
            replyString = sql_con->check_password(map2["username"], map2["password"]);
        }
        break;
    case 2:
        // ע��
        // INSERT INTO users VALUES(0 , 'zhc' , '123' ,  '2023-09-15 10:54:23')
        res = sql_con->insert_sql("users", map2["username"], map2["password"], map2["time"]);
        // ע��ʧ��
        if (!res) {
            replyString = "Register error! The user is exist!\n";
        }
        break;
    case 3:
        // ������Ϣ
        // insert into Msg(User_Cde, Msg, WriteTime) values('username', 'msg', 'time');
        res = sql_con->insert_sql("msg", map2["username"], map2["msg"], map2["time"]);
        if (!res) {
            replyString = "Send msg error!\n";
        }
        break;
    case 4:
        // ����ȫ����ʷ��Ϣ
        replyString = sql_con->select_all_msg();
        break;
    default:
        replyString = "unknow request!";
        break;
    }
    return replyString;
}

/*
HandleClient ���������������뵥���ͻ��˵�ͨ�ŵĺ�����
����һ���������߳���ִ�У�����������Կͻ��˵���Ϣ�����ض�Ӧ��������

ÿ�����µĿͻ�������ʱ������Ϊ�䴴��һ���������߳������� HandleClient ������

ÿ�����µļ�������Ϣ�Ŀͻ�������ʱ(recvStr[0]._Equal("3"))���Ὣ����׽��ּ�¼��clients��

ͬʱҲ����ͻ��˶Ͽ����ӵ�����������clients���Ƴ���

*/
void Server::HandleClient(SOCKET clientSocket) {
    // ѭ��������Ϣ
    while (true) {
        std::string recvString = ReceiveLongMessages(clientSocket);
        if (recvString.empty()) {
            break;
        }        
        std::cout << "Received message: " << recvString << std::endl;

        // JSON�ַ�����������������Ϣ�ָ�ɸ����Ӳ���
        std::map<std::string, std::string> map2 = my_sql_use::parseFromJsonString2(recvString, this->key);
        // std::cout << "map.size()=" << map2.size() << std::endl;

        // 0��ʾ����һ��ר�Ž�������Ϣ���׽���
        if (map2["code"]._Equal("0")) {
            {

                std::lock_guard<std::mutex> lock(clientsMutex);
                if (onlineUsers.size() >= MAX_CLIENTS) {
                    std::cerr << "Max number of clients reached." << std::endl;
                    closesocket(clientSocket);
                    continue;
                }
                // ��ǰ������С�����������ʱ��������ӽ����ɹ�
                AddUser(clientSocket, map2["username"]);
            }
        }
        // �ظ��ַ���
        std::string replyString = operateDatabase(map2);
        if (map2["code"]._Equal("3")) {
            std::unique_lock<std::mutex> lock(batchMutex);
            // ��������Ϣ
            std::string newStr = map2["time"]  + "  " + map2["username"] + ": " + map2["msg"];
            messageBatch.push_back(newStr); //("msg", recvStr[1], recvStr[2], recvStr[3]);

            // ����UI
            newMsg.append(newStr + "\n");
            std::lock_guard<std::mutex> lock2(guiMutex1);
            QMetaObject::invokeMethod(myWindow, "updateGui1", Qt::QueuedConnection,
                Q_ARG(QString, QString::fromStdString(newMsg)));

            // ͨ����������batchCondition ��������Ϣ�㲥�߳�SendBatchedMessages
            if (messageBatch.size() >= MESSAGE_BATCH_SIZE) {
                batchCondition.notify_one();
            }
        }
        // �ظ��ͻ���
        // send(clientSocket, replyString.c_str(), replyString.length(), 0);
        SendLongMessages(clientSocket, replyString);
    }

    // �׽������ӶϿ�
    {
        std::lock_guard<std::mutex> lock(clientsMutex);
        // �� onlineUsers�������Ƴ��Ѿ��Ͽ����ӵĿͻ����׽���
        // std::remove_if �㷨�Ὣ����������Ԫ���Ƶ�������ĩβ��������ָ�����һ������������Ԫ�صĵ�������
        // Ȼ��erase ������ɾ���Ӹõ�����λ�õ�����ĩβ������Ԫ�ء�
        auto it = std::remove_if(onlineUsers.begin(), onlineUsers.end(),
            [clientSocket](const UserInfo& user) {
                return user.clientSocket == clientSocket;
            });
        if (it != onlineUsers.end()) {
            onlineUsers.erase(it, onlineUsers.end());
            // ����UI
            std::string newstr = getAllOnlineUsers();
            // �����Զ����ź�������GUI����
            std::lock_guard<std::mutex> lock3(guiMutex2);
            QMetaObject::invokeMethod(myWindow, "updateGui2", Qt::QueuedConnection,
                Q_ARG(QString, QString::fromStdString(newstr)));
        }
    }
    // �ر��׽���
    closesocket(clientSocket);
}

/*
���������͵���Ϣ�������ǳ��ȳ���1024�ֽڵ��ַ���
*/
std::string Server::ReceiveLongMessages(SOCKET clientSocket) {
    std::string replyString; // ���ڴ洢���յ��ĳ��ַ���
    const int chunkSize = 1024; // ÿ�����ݿ�Ĵ�С
    char buffer[chunkSize]; // �������ݵĻ�����
    while (true) {
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived == SOCKET_ERROR) {
            // ������մ���
            break;
        }
        else if (bytesReceived == 0) {
            // �����ѹر�
            break;
        }
        // �����յ������ݿ���ӵ����ַ�����
        replyString.append(buffer, bytesReceived);
        // ����Ƿ���������ַ���
        if (bytesReceived < chunkSize) {
            break;
        }
    }
    return replyString;
}

/*
���ͳ��ַ���
*/
bool Server::SendLongMessages(SOCKET clientSocket, std::string replyString) {
    const int chunkSize = 1024; // ÿ�����ݿ�Ĵ�С
    for (int i = 0; i < replyString.size(); i += chunkSize) {
        int bytesToSend = chunkSize < static_cast<int>(replyString.size() - i) ? chunkSize : static_cast<int>(replyString.size() - i);
        std::string chunk = replyString.substr(i, bytesToSend);
        int bytesSent = send(clientSocket, chunk.c_str(), bytesToSend, 0);
        if (bytesSent == SOCKET_ERROR) {
            // �����ʹ���
            break;
        }
    }
    return true;
}


/*
SendBatchedMessages ���������������۵���Ϣ�������͸����пͻ��˵ĺ�����
������һ���������߳���ִ�У����ڼ���Ƿ����㹻����Ϣ���ۣ��������������
����Ϣ�������͸����пͻ��ˡ��������Լ�������ͨ�ŵĿ��������Ч��

lambda https://www.cnblogs.com/yajiu/p/15693724.html

*/
void Server::SendBatchedMessages() {
    while (true) {
        std::unique_lock<std::mutex> lock(batchMutex);
        // ͨ����������batchCondition �ȴ�����Ϣ
        auto& refx_x1 = messageBatch;
        auto& refx_x2 = MESSAGE_BATCH_SIZE;
        batchCondition.wait(lock, [&refx_x1, &refx_x2] { return refx_x1.size() >= refx_x2; });
        // ���
        std::vector<std::string> messagesToSend(messageBatch.begin(), messageBatch.end());
        messageBatch.clear();
        // �㲥�����������û�
        for (const auto& user : onlineUsers) {
            std::cout << "spread=" << user.username << std::endl;
            for (const std::string& message : messagesToSend) {
                SendLongMessages(user.clientSocket, message);
                // send(user.clientSocket, message.c_str(), static_cast<int>(message.size()), 0);
            }
        }
    }
}


int Server::init(MyWindow* m) {
    this->myWindow = m;

    // 1. ��ʼ�� Winsock��ͨ������ `WSAStartup` ��������ʼ�� Windows �׽��ֿ⣬ȷ���ܹ��ڳ�����ʹ�����繦�ܡ�
    // WSADATA wsData;
    if (WSAStartup(MAKEWORD(2, 2), &wsData) != 0) {
        std::cerr << "Failed to initialize Winsock." << std::endl;
        return 1;
    }
    // 2. �����������׽���
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create server socket." << std::endl;
        WSACleanup();
        return 1;
    }
    // 3. �󶨷������׽���
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(std::stoi(myWindow->server_port));
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Failed to bind server socket." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    // 4. ��ʼ������ʹ�� `listen` ������ʼ�����������󣬵ȴ��ͻ��˵����ӡ�
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Failed to listen on server socket." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    std::cout << "Server is listening on port " << myWindow->server_port << "..." << std::endl;
}

/*
 `main` ������˼·�ǣ�
�����������׽��ֲ���ʼ�������ӣ�Ȼ��ͨ���������߳�������ͻ��˵�ͨ�š�
1. ���߳�ʹ��HandleClient���߳� ����ÿ���ͻ��˵���Ϣ�շ���
2. ��һ��SendBatchedMessages�߳� ���ڷ���������Ϣ�����пͻ��ˡ�
�Ӷ��������ܹ������������ͻ��˵����Ӻ�ͨ�š�
*/
int Server::main_server() {
    std::cout << "Server start!\n\n";
    shouldExit = false;

    // 5. ��������Ϣ�㲥�߳�SendBatchedMessages
    std::thread batchThread(&Server::SendBatchedMessages,this);
    batchThread.detach();

    // 6. ѭ���ȴ��ͻ������ӣ������µĿͻ����׽�������
    while (!shouldExit) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Failed to accept client connection." << std::endl;
            // break;
            continue;
        }
        // Ϊÿ���ͻ����׽������ӷ���һ��HandleClient�̴߳���
        std::thread clientThread(&Server::HandleClient,this, clientSocket);
        clientThread.detach();
    }
    // 7. ������Դ
    closesocket(serverSocket);
    WSACleanup();
    std::cout << "Server end!\n\n";
    return 0;
}
