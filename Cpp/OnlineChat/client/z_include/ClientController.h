#ifndef ClientController_H
#define ClientController_H

#include "chatWindow.h"
#include "mainwindow.h"
#include "client.h"
#include "myUseFun.h"

#include <WinSock2.h>
#include <mutex>


#include <QMessageBox>
#include <QObject>
#include <QPushButton>

#include <QMainWindow>
#include <QMenuBar>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QDebug>


class ClientController: public QObject
{
    Q_OBJECT

public slots:
    void handleLogin();
    void handleRegister();
    void onSendMessage();
    void onGetMessage();

private:
    std::string username;
    // std::string server_ip;// ������ip
    // int server_port;// �����������Ķ˿ں�

    void handleButtonClicked(std::string i);
    void Receive_New_Messages();

public:
    ClientController();
    ~ClientController();

    Client* client;
    MainWindow* mainWindow;
    ChatWindow* chatWindow;
};

#endif // ClientController_H
