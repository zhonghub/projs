#ifndef server_H
#define server_H

#include "sql.h"
#include "MyWindow.h"

#include <atomic>

#include <iostream>
#include <winsock2.h>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <QTextEdit>
#include <QThread>

#pragma comment(lib, "ws2_32.lib")


struct UserInfo {
	SOCKET clientSocket;
	std::string username;
};


class Server {
private:
	WSADATA wsData;
	SOCKET serverSocket;

	MyWindow* myWindow;
	//QTextEdit* lineEdit1;
	//QTextEdit* lineEdit2;
	std::string newMsg;
	static Server* instance;

	// �����������Ķ˿ں�
	// const int PORT = 12345;
	// ���������ܹ�ͬʱ��������ͻ���������
	const int MAX_CLIENTS = 20;
	// ÿ��������Ϣ�Ĵ�С
	const int MESSAGE_BATCH_SIZE = 1;

	/*
	���ڼ�¼���С���������Ϣ���Ŀͻ��˵��׽��� �� �û�����

	ÿ�����µļ�������Ϣ�Ŀͻ�������ʱ���Ὣ���׽�����ӵ����vector�С�
	�� HandleClient�����У����ͻ��˶Ͽ�����ʱ��������vector���Ƴ���Ӧ�ļ�������Ϣ���׽��֡�
	*/
	std::vector<UserInfo> onlineUsers;

	/*
	����messageBatch ���ڻ��۴����͵���Ϣ���Ա�������һ������������������͡�
	���յ��ͻ��˵���Ϣʱ���Ὣ��Ϣ��ӵ���������С�һ����Ϣ�����ﵽһ����ֵ���ͻᴥ���������Ͳ�����
		SendBatchedMessages�߳� �����messageBatch�����Ϣ��
		HandleClient�߳� ������messageBatch�������Ϣ��
	*/
	std::vector<std::string> messageBatch;

	/*
	������clientsMutex ������ ���HandleClient�߳� ֮��� clients �������а�ȫ�ķ��ʺ��޸ġ�
	���� ���HandleClient�߳� ����ͬʱ���� clients ��������Ҫʹ�û�������ȷ���̰߳�ȫ���������ݾ�����
	*/
	std::mutex clientsMutex;

	/*
	������batchMutex �����ڶ���߳�֮��� messageBatch �������а�ȫ�ķ��ʺ��޸ġ�
	����߳̿���ͬʱ���� messageBatch ��������Ҫʹ�û���������֤�̰߳�ȫ��
		SendBatchedMessages�߳� �����messageBatch�����Ϣ��
		HandleClient�߳� ������messageBatch�������Ϣ��
	*/
	std::mutex batchMutex;

	/*
	��������batchCondition ����ʵ���߳�֮���ͨ�ź�ͬ����
	�ź�����
	�㲥����Ϣ�߳� ��ִ��һ��P����������
	ÿ����MESSAGE_BATCH_SIZE=1������Ϣ��ִ��һ��V���������� �㲥����Ϣ�߳�

	SendBatchedMessages �����ڵȴ�������Ϣ����������ʱ����ͨ��������������ȴ���

	���� HandleClient �����У����յ��㹻����Ϣ����Ҫ������������ʱ����ͨ�������������֪ͨ SendBatchedMessages ������
	*/
	std::condition_variable batchCondition;

	std::atomic<bool> shouldExit;

public:
	int init(MyWindow* m);

	static Server* getInstance();

	void AddUser(SOCKET clientSocket, const std::string& username);
	std::string getAllOnlineUsers();

	std::string operateDatabase(std::vector<std::string>);

	bool SendLongMessages(SOCKET clientSocket, std::string replyString);

	void HandleClient(SOCKET clientSocket);

	void SendBatchedMessages();

	int main_server();

	void stopServer() {
		shouldExit = true;
	}
};
#endif // server_H