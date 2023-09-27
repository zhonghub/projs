#include "z_include/client.h"


// 静态变量初始化
Client* Client::instance = nullptr;

/*
初始化套接字，用于专门接收服务器发送的消息
*/
std::string Client::initSocket() {
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return "error1: WSAStartup failed.";
    }
    // 创建套接字
    clientSocket0 = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket0 == INVALID_SOCKET) {
        std::cerr << "Socket creation failed." << std::endl;
        WSACleanup();
        return "error2: Socket creation failed.";
    }
    // 设置连接信息
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(server_port); // 服务器端口
    serverAddr.sin_addr.s_addr = inet_addr(server_ip.c_str());

    // 连接到服务器
    if (connect(clientSocket0, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed." << std::endl;
        closesocket(clientSocket0);
        WSACleanup();
        return "error3: Connection failed.";
    }
    return "init ok";
}

/*
接收来自服务器的回复，可能是长度超出1024字节的字符串
*/
std::string Client::ReceiveLongMessages(SOCKET clientSocket) {
    std::string replyString; // 用于存储接收到的长字符串
    const int chunkSize = 1024; // 每个数据块的大小
    char buffer[chunkSize]; // 接收数据的缓冲区
    while (true) {
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived == SOCKET_ERROR) {
            // 处理接收错误
            break;
        }
        else if (bytesReceived == 0) {
            // 连接已关闭
            break;
        }
        // 将接收到的数据块添加到长字符串中
        replyString.append(buffer, bytesReceived);
        // 检查是否接收完整字符串
        if (bytesReceived < chunkSize) {
            break;
        }
    }
    return replyString;
}

/*
发送长字符串
*/
bool Client::SendLongMessages(SOCKET clientSocket, std::string replyString) {
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
主线程发送消息给服务器
*/
std::string Client::sendStrToServer(std::string message) {
    //  1. 初始化 Winsock：通过调用 `WSAStartup` 函数，初始化 Windows 套接字库，确保能够在程序中使用网络功能。
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return "error1: WSAStartup failed.";
    }

    // 2. 创建客户端套接字
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed." << std::endl;
        WSACleanup();
        return "error2: Socket creation failed.";
    }

    // 3. 设置连接信息
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(server_port); // 服务器端口
    serverAddr.sin_addr.s_addr = inet_addr(server_ip.c_str());

    // 4. 连接到服务器
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return "error3: Connection failed.";
    }

    // 5. 将字符串发送给服务器
    // send(clientSocket, message.c_str(), message.size(), 0);
    SendLongMessages(clientSocket, message);
    std::cout << "Send str: " << message << std::endl;

    // 6. 接收来自服务器的回复
    // 用于存储接收到的长字符串
    std::string replyString = ReceiveLongMessages(clientSocket); 
    // std::cout << "Received: " << replyString << std::endl;

    // 7. 关闭套接字并清理资源
    closesocket(clientSocket);
    WSACleanup();
    return replyString;
}

