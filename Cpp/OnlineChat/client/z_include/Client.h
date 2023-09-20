#ifndef CLIENT_H
#define CLIENT_H

#include "myUseFun.h"

#include <iostream>
#include <WinSock2.h>
#include <thread>
#include <string>


#pragma comment(lib, "ws2_32.lib")


class Client {
private:
    static Client* instance;
    std::string server_ip;// 服务器ip
    int server_port;// 服务器监听的端口号
    SOCKET clientSocket0;// 专门用于接收新消息的套接字
    WSADATA wsaData;

    Client() {
        username = "no";
        server_ip = "127.0.0.1";
        server_port = 12345;
    }

public:
    std::string username;

    ~Client() {
        clearSocket();
    }

    void setUserName(const std::string& username0) {
        this->username = username0;
    }

    static Client* getInstance() {
        static int count = 0; // 声明并初始化静态变量，记录实例的个数
        if (instance == nullptr) {
            instance = new Client();
            count++;
        }
        return instance;
    }

    std::string initSocket();

    SOCKET& getSocket() {
        return clientSocket0;
    }


    // 关闭套接字并清理资源
    void clearSocket() {
        closesocket(clientSocket0);
        WSACleanup();
    }

    void init(std::string ip, int port_server){
        this->server_ip = ip;
        this->server_port = port_server;
        initSocket();
    }

    // 接收来自服务器的回复，可能是长度超出1024字节的字符串
    std::string ReceiveLongMessages(SOCKET clientSocket);

    // 主线程发送消息给服务器
    std::string sendStrToServer(std::string message);

};

/* 
在头文件中直接定义并实现非类函数, 这种做法可能导致函数的定义在多个编译单元中重复，从而引发链接错误。
为了避免这种问题，你可以使用 inline 关键字来告诉编译器这些函数是内联函数，以避免多次定义。

使用 inline 关键字有助于确保函数在每个编译单元中都只有一个定义，从而避免链接错误。
然而，这种做法并不适用于所有情况，特别是当函数实现比较复杂时，
建议将函数实现放在单独的 .cpp 文件中。这可以提高代码的可维护性和可读性。
*/

//inline std::string mainClient(std::string message);


#endif // CLIENT_H