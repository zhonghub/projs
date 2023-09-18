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
    // 创建MyWidget
    MyWindow *widget = new MyWindow();
    widget->show();
    // 无法在所选路径中找到 Qt5Gui.pdb
    Server * mainServer = Server::getInstance();
    mainServer->setWindow(widget);
    std::thread receiveThread(&Server::main_server, mainServer);
    receiveThread.detach();
    // mainServer->main_server();
    // updateThread.join(); // 阻塞主线程，直到updateThread完成
    // updateThread.detach(); // 将updateThread设置为后台线程

    std::cout << "Server end\n";
    return app.exec();
}
