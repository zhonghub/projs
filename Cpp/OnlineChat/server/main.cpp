#include <QApplication>

#include "server.h"
#include "sql.h"
#include "MyWindow.h"

int main(int argc, char* argv[]) {

    // std::locale::global(std::locale(""));
    std::cout << "Server start\n";

    std::string server = "tcp://localhost:3306"; // or "tcp://127.0.0.1:3306"
    std::string username = "root";
    std::string password = "123456";
    std::string database = "data1";
    SQL_con* sql_con = SQL_con::getInstance();
    sql_con->init(server, username, password, database);

    QApplication app(argc, argv);
    // ����MyWidget
    MyWindow *widget = new MyWindow();
    widget->show();
    // �޷�����ѡ·�����ҵ� Qt5Gui.pdb
    Server * mainServer = Server::getInstance();
    mainServer->setWindow(widget);
    std::thread receiveThread(&Server::main_server, mainServer);
    receiveThread.detach();
    // mainServer->main_server();
    // updateThread.join(); // �������̣߳�ֱ��updateThread���
    // updateThread.detach(); // ��updateThread����Ϊ��̨�߳�

    std::cout << "Server end\n";
    return app.exec();
}
