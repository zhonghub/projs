#include <QApplication>
#include "z_include/MainWindow.h"
#include "z_include/ClientController.h"

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);
    QFont font;
    font.setFamily("宋体"); // 设置字体为宋体或其他支持中文的字体
    app.setFont(font); // 将字体应用到应用程序
    ClientController controller;
    controller.mainWindow->show();
    return app.exec();
}
