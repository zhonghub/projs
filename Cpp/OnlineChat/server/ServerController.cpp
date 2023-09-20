#include "ServerController.h"

ServerController::ServerController() {
    std::string server = "tcp://localhost:3306"; // or "tcp://127.0.0.1:3306"
    std::string username = "root";
    std::string password = "123456";
    std::string database = "data1";
    SQL_con* sql_con = SQL_con::getInstance();
    sql_con->init(server, username, password, database);

    myWindow = new MyWindow();
    mainServer = Server::getInstance();

    connect(myWindow->startAction, &QAction::triggered, this, &ServerController::handleStart);
    connect(myWindow->stopAction, &QAction::triggered, this, &ServerController::handleStop);

    //std::thread receiveThread(&Server::main_server, mainServer);
    //receiveThread.detach();
}


ServerController::~ServerController() {
    delete myWindow;
}

void ServerController::handleStart() {
    mainServer->init(myWindow);
    std::thread receiveThread(&Server::main_server, mainServer);
    receiveThread.detach();
    QMessageBox::information(this->myWindow, "Information", QString::fromStdString("Server is started!"));
}

void ServerController::handleStop() {
    mainServer->stopServer();
    QMessageBox::information(this->myWindow, "Information", QString::fromStdString("Server is stoped!"));
}