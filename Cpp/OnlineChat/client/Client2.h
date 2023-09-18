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
    std::string server_ip;// ������ip
    int server_port;// �����������Ķ˿ں�
    SOCKET clientSocket;// ר�����ڽ�������Ϣ���׽���
    WSADATA wsaData;

    Client2() {
        username = "no";
        server_ip = "127.0.0.1";
        server_port = 12345;
    }

public:
    std::string username;

    static Client2* getInstance() {
        static int count = 0; // ��������ʼ����̬��������¼ʵ���ĸ���
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

    // �������Է������Ļظ��������ǳ��ȳ���1024�ֽڵ��ַ���
    std::string ReceiveLongMessages(SOCKET clientSocket);

    // ���̷߳�����Ϣ��������������ȡ�ظ��ַ���
    std::string sendStrToServer(std::string message);

};

#endif // CLIENT2_H