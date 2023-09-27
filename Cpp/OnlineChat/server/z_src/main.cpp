#include <QApplication>

#include "z_include/ServerController.h"

int main(int argc, char* argv[]) {

    // std::locale::global(std::locale(""));
    std::cout << "Controller start\n";

    QApplication app(argc, argv);
    // ����MyWidget
    ServerController myController;
    myController.myWindow->show();
    QMessageBox::information(myController.myWindow, "Information", QString::fromStdString("Port is not set!"));

    /*
    myController.mainServer->init(myController.myWindow);
    std::thread receiveThread(&Server::main_server, myController.mainServer);
    receiveThread.detach();
    */

    // mainServer->main_server();
    // updateThread.join(); // �������̣߳�ֱ��updateThread���
    // updateThread.detach(); // ��updateThread����Ϊ��̨�߳�

    return app.exec();
}
