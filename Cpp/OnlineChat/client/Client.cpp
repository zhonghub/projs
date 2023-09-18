#include "client.h"
#include <chrono>
#include <ctime>
#include <iomanip>

// ��̬������ʼ��
Client* Client::instance = nullptr;

/*
��ʼ���׽��֣�����ר�Ž��շ��������͵���Ϣ
*/
std::string Client::initSocket() {
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return "error1: WSAStartup failed.";
    }
    // �����׽���
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed." << std::endl;
        WSACleanup();
        return "error2: Socket creation failed.";
    }
    // ����������Ϣ
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(server_port); // �������˿�
    serverAddr.sin_addr.s_addr = inet_addr(server_ip.c_str());

    // ���ӵ�������
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return "error3: Connection failed.";
    }
    return "init ok";
}

/*
���߳�ר�Ž��շ������㲥������Ϣ
������UI
*/
void Client::Receive_New_Messages() {
    std::string msg = "0#" + username + "#waiting for new msg!";
    send(clientSocket, msg.c_str(), msg.size(), 0);

    char buffer[1024];
    int bytesReceived;

    while (true) {
        bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            // Handle server disconnect or error
            break;
        }
        buffer[bytesReceived] = '\0';
        std::string recv(buffer);
        std::cout << "\n���߳� Receive_New_Messages�� Received: " << recv << std::endl;
        /*
        * // ������˿ͻ��ظ�����Ϣ��ʽ������
        std::vector<std::string> strs = myUse::splitString(recv, '#');
        if (strs.size() > 3) {
            std::string newMsg = strs[3] + "  " + strs[1] + ": " + strs[2];
            if (!newMsg.empty() && newMsg.back() == '\n') {
                newMsg.pop_back();
            }
            // �����Զ����ź�������GUI����
            std::lock_guard<std::mutex> lock3(guiMutex1);
            QMetaObject::invokeMethod(myWindow, "updateGui1", Qt::QueuedConnection,
                Q_ARG(QString, QString::fromStdString(newMsg)));
            // chatDisplay->append(QString::fromStdString(newMsg));
        }*/
        if (!recv.empty() && recv.back() == '\n') {
            recv.pop_back();
        }
        // �����Զ����ź�������GUI����
        std::lock_guard<std::mutex> lock3(guiMutex1);
        QMetaObject::invokeMethod(myWindow, "updateGui1", Qt::QueuedConnection,
            Q_ARG(QString, QString::fromStdString(recv)));

    }
}

/*
�������Է������Ļظ��������ǳ��ȳ���1024�ֽڵ��ַ���
*/
std::string Client::ReceiveLongMessages(SOCKET clientSocket) {
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
���̷߳�����Ϣ��������
*/
std::string Client::sendStrToServer(std::string message) {
    //  1. ��ʼ�� Winsock��ͨ������ `WSAStartup` ��������ʼ�� Windows �׽��ֿ⣬ȷ���ܹ��ڳ�����ʹ�����繦�ܡ�
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return "error1: WSAStartup failed.";
    }

    // 2. �����ͻ����׽���
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed." << std::endl;
        WSACleanup();
        return "error2: Socket creation failed.";
    }

    // 3. ����������Ϣ
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(server_port); // �������˿�
    serverAddr.sin_addr.s_addr = inet_addr(server_ip.c_str());

    // 4. ���ӵ�������
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return "error3: Connection failed.";
    }

    // 5. ���ַ������͸�������
    send(clientSocket, message.c_str(), message.size(), 0);
    std::cout << "Send str: " << message << std::endl;

    // 6. �������Է������Ļظ�
    // ���ڴ洢���յ��ĳ��ַ���
    std::string replyString = ReceiveLongMessages(clientSocket); 
    // std::cout << "Received: " << replyString << std::endl;

    // 7. �ر��׽��ֲ�������Դ
    closesocket(clientSocket);
    WSACleanup();
    return replyString;
}

