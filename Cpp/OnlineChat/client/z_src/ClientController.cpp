#include "z_include/ClientController.h"

ClientController::ClientController() {
    username = "no";

    // 绑定客户端实例
    client = Client::getInstance();

    mainWindow = new MainWindow();

    chatWindow = new ChatWindow();
    mainWindow->show();


    // 连接按钮的点击信号到槽函数
    connect(mainWindow->loginButton, &QPushButton::clicked, this, &ClientController::handleLogin);
    connect(mainWindow->registerButton, &QPushButton::clicked, this, &ClientController::handleRegister);
    connect(chatWindow->sendButton, &QPushButton::clicked, this, &ClientController::onSendMessage);
    connect(chatWindow->getButton, &QPushButton::clicked, this, &ClientController::onGetMessage);
}

ClientController::~ClientController() {
    delete mainWindow;
    delete chatWindow;
}

/*
登录按钮绑定事件
*/
void ClientController::handleLogin()
{
    handleButtonClicked("1");
}

/*
登录按钮绑定事件
*/
void ClientController::handleRegister()
{
    handleButtonClicked("2");
}

/*
登录/注册按钮绑定事件
*/
void ClientController::handleButtonClicked(std::string i) {
    QString btnAction;
    if (i == "1") {
        btnAction = "登录";
    }
    else {
        btnAction = "注册";
 
    }
    QString user = mainWindow->getUsername();
    QString password = mainWindow->getPassword();
    if (user.isEmpty() || password.isEmpty()) {
        QMessageBox::critical(this->chatWindow, "Error!", "Username or password can not be empty!");
        return;
    }
    username = user.toStdString();
    std::string message = i + "#" + username + "#" + password.toStdString() + "#" + myUse::getCurrentTimeAsString();

    int port = std::stoi(mainWindow->server_port);
    // 发送登录消息，并接收登录结果
    std::cout << "ip = " << mainWindow->server_ip << "  port="<< port << std::endl;
    client->init(mainWindow->server_ip, port);
    std::string recv = client->sendStrToServer(message);
    std::cout << "longin/register result = " << recv << std::endl;
    // 处理登录结果
    if (recv._Equal("ok")) {
        // 创建消息发布窗口
        chatWindow->setWindowTitle(QString::fromStdString(username));
        chatWindow->show();
        
        client->setUserName(username);
        std::cout << "开启一个子线程2: 专门接收服务器广播的新消息." << std::endl;
        // 开启一个子线程专门接收服务器广播的新消息
        // std::thread receiveThread(&Client::Receive_New_Messages, client);
        std::thread receiveThread(&ClientController::Receive_New_Messages, this);
        receiveThread.detach();

        // 关闭登录页面
        mainWindow->close();
        QMessageBox::information(this->mainWindow, btnAction+"成功", QString::fromStdString(recv));
    }
    else {
        QMessageBox::critical(this->mainWindow, btnAction+"失败", QString::fromStdString(recv));
    }
}


/*
发布按钮绑定事件
*/
void ClientController::onSendMessage()
{
    // QString::fromStdString(recv)
    // message0.toStdString()
    QString message0 = chatWindow->getMsg();
    std::string str1 = "3#" + chatWindow->windowTitle().toStdString() + "#"; // GetLocalIPAddress()
    std::string message = str1 + message0.toStdString() + "#" + myUse::getCurrentTimeAsString();
    if (!message0.isEmpty())
    {
        std::string recv = client->sendStrToServer(message);
        if (recv._Equal("ok")) {
            chatWindow->clearMsg();
        }
        else {
            QMessageBox::information(this->chatWindow, "发送失败", QString::fromStdString(recv));
        }
    }
}

/*
获取历史消息按钮绑定事件
*/
void ClientController::onGetMessage()
{
    std::string sendstr = "4#" + chatWindow->windowTitle().toStdString() + "#get All Msg#" + myUse::getCurrentTimeAsString();
    std::string recv = client->sendStrToServer(sendstr);
    if (!recv.empty() && recv.back() == '\n') {
        recv.pop_back();
    }
    chatWindow->setChatText(recv);
}

/*
子线程专门接收客户端广播的新消息
*/
void ClientController::Receive_New_Messages() {
    std::string msg = "0#" + username + "#waiting for new msg!";
    send(client->getSocket(), msg.c_str(), msg.size(), 0);
    std::string replyString = client->ReceiveLongMessages(client->getSocket());
    if (!replyString._Equal("ok")) {
        return;
    }
    while (true) {
        // 改了客户回复的消息格式！！！
        std::string recv = client->ReceiveLongMessages(client->getSocket());
        if (recv.size() <= 0) {
            // Handle server disconnect or error
            break;
        }
        std::cout << "\n子线程 Receive_New_Messages： Received: " << recv << std::endl;
        /*
        if (!recv.empty() && recv.back() == '\n') {
            recv.pop_back();
        }*/
        // 发射自定义信号来触发GUI更新
        std::lock_guard<std::mutex> lock3(guiMutex1);
        QMetaObject::invokeMethod(chatWindow, "updateGui1", Qt::QueuedConnection,
            Q_ARG(QString, QString::fromStdString(recv)));

    }
}
