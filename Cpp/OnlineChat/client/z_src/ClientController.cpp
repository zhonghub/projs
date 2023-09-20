#include "z_include/ClientController.h"

ClientController::ClientController() {
    username = "no";

    // �󶨿ͻ���ʵ��
    client = Client::getInstance();

    mainWindow = new MainWindow();

    chatWindow = new ChatWindow();
    mainWindow->show();


    // ���Ӱ�ť�ĵ���źŵ��ۺ���
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
��¼��ť���¼�
*/
void ClientController::handleLogin()
{
    handleButtonClicked("1");
}

/*
��¼��ť���¼�
*/
void ClientController::handleRegister()
{
    handleButtonClicked("2");
}

/*
��¼/ע�ᰴť���¼�
*/
void ClientController::handleButtonClicked(std::string i) {
    QString btnAction;
    if (i == "1") {
        btnAction = "��¼";
    }
    else {
        btnAction = "ע��";
 
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
    // ���͵�¼��Ϣ�������յ�¼���
    std::cout << "ip = " << mainWindow->server_ip << "  port="<< port << std::endl;
    client->init(mainWindow->server_ip, port);
    std::string recv = client->sendStrToServer(message);
    std::cout << "longin/register result = " << recv << std::endl;
    // �����¼���
    if (recv._Equal("ok")) {
        // ������Ϣ��������
        chatWindow->setWindowTitle(QString::fromStdString(username));
        chatWindow->show();
        
        client->setUserName(username);
        std::cout << "����һ�����߳�2: ר�Ž��շ������㲥������Ϣ." << std::endl;
        // ����һ�����߳�ר�Ž��շ������㲥������Ϣ
        // std::thread receiveThread(&Client::Receive_New_Messages, client);
        std::thread receiveThread(&ClientController::Receive_New_Messages, this);
        receiveThread.detach();

        // �رյ�¼ҳ��
        mainWindow->close();
        QMessageBox::information(this->mainWindow, btnAction+"�ɹ�", QString::fromStdString(recv));
    }
    else {
        QMessageBox::critical(this->mainWindow, btnAction+"ʧ��", QString::fromStdString(recv));
    }
}


/*
������ť���¼�
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
            QMessageBox::information(this->chatWindow, "����ʧ��", QString::fromStdString(recv));
        }
    }
}

/*
��ȡ��ʷ��Ϣ��ť���¼�
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
���߳�ר�Ž��տͻ��˹㲥������Ϣ
*/
void ClientController::Receive_New_Messages() {
    std::string msg = "0#" + username + "#waiting for new msg!";
    send(client->getSocket(), msg.c_str(), msg.size(), 0);
    std::string replyString = client->ReceiveLongMessages(client->getSocket());
    if (!replyString._Equal("ok")) {
        return;
    }
    while (true) {
        // ���˿ͻ��ظ�����Ϣ��ʽ������
        std::string recv = client->ReceiveLongMessages(client->getSocket());
        if (recv.size() <= 0) {
            // Handle server disconnect or error
            break;
        }
        std::cout << "\n���߳� Receive_New_Messages�� Received: " << recv << std::endl;
        /*
        if (!recv.empty() && recv.back() == '\n') {
            recv.pop_back();
        }*/
        // �����Զ����ź�������GUI����
        std::lock_guard<std::mutex> lock3(guiMutex1);
        QMetaObject::invokeMethod(chatWindow, "updateGui1", Qt::QueuedConnection,
            Q_ARG(QString, QString::fromStdString(recv)));

    }
}
