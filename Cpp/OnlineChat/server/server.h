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

	// 服务器监听的端口号
	// const int PORT = 12345;
	// 服务器所能够同时处理的最大客户端连接数
	const int MAX_CLIENTS = 20;
	// 每个批量消息的大小
	const int MESSAGE_BATCH_SIZE = 1;

	/*
	用于记录所有“监听新消息”的客户端的套接字 及 用户名。

	每当有新的监听新消息的客户端连接时，会将其套接字添加到这个vector中。
	在 HandleClient函数中，当客户端断开连接时，会从这个vector中移除对应的监听新消息的套接字。
	*/
	std::vector<UserInfo> onlineUsers;

	/*
	向量messageBatch 用于积累待发送的消息，以便在满足一定条件后进行批量发送。
	当收到客户端的消息时，会将消息添加到这个向量中。一旦消息数量达到一定阈值，就会触发批量发送操作。
		SendBatchedMessages线程 会清空messageBatch里的消息，
		HandleClient线程 可能往messageBatch添加新消息，
	*/
	std::vector<std::string> messageBatch;

	/*
	互斥量clientsMutex 用于在 多个HandleClient线程 之间对 clients 向量进行安全的访问和修改。
	由于 多个HandleClient线程 可能同时操作 clients 向量，需要使用互斥量来确保线程安全，避免数据竞争。
	*/
	std::mutex clientsMutex;

	/*
	互斥量batchMutex 用于在多个线程之间对 messageBatch 向量进行安全的访问和修改。
	多个线程可能同时操作 messageBatch 向量，需要使用互斥量来保证线程安全：
		SendBatchedMessages线程 会清空messageBatch里的消息，
		HandleClient线程 可能往messageBatch添加新消息，
	*/
	std::mutex batchMutex;

	/*
	条件变量batchCondition 用于实现线程之间的通信和同步。
	信号量：
	广播新消息线程 先执行一个P操作阻塞，
	每接收MESSAGE_BATCH_SIZE=1条新消息，执行一次V操作，唤醒 广播新消息线程

	SendBatchedMessages 函数在等待批量消息的条件满足时，会通过这个条件变量等待。

	而在 HandleClient 函数中，当收到足够的消息并需要触发批量发送时，会通过这个条件变量通知 SendBatchedMessages 函数。
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