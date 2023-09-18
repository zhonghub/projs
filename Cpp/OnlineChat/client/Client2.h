#ifndef CLIENT2_H
#define CLIENT2_H

#include "myUseFun.h"

#include <iostream>
#include <WinSock2.h>
#include <thread>
#include <string>
#include <QTextEdit>
#include <sstream>
#include <mutex>

#pragma comment(lib, "ws2_32.lib")


class Client2 {
private:
    static Client2* instance;
    std::string server_ip;// 服务器ip
    int server_port;// 服务器监听的端口号
    SOCKET clientSocket;// 专门用于接收新消息的套接字
    WSADATA wsaData;

    Client2() {
        username = "no";
        server_ip = "127.0.0.1";
        server_port = 12345;
    }

public:
    std::string username;

    static Client2* getInstance() {
        static int count = 0; // 声明并初始化静态变量，记录实例的个数
        if (instance == nullptr) {
            instance = new Client2();
            count++;
        }
        return instance;
    }

    void init(std::string ip, int port_server) {
        this->server_ip = ip;
        this->server_port = port_server;
    }

    // 接收来自服务器的回复，可能是长度超出1024字节的字符串
    std::string ReceiveLongMessages(SOCKET clientSocket);

    // 主线程发送消息给服务器，并获取回复字符串
    std::string sendStrToServer(std::string message);

};

#endif // CLIENT2_H