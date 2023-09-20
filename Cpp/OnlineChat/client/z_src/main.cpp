#include <QApplication>
#include "z_include/MainWindow.h"
#include "z_include/ClientController.h"

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);
    QFont font;
    font.setFamily("����"); // ��������Ϊ���������֧�����ĵ�����
    app.setFont(font); // ������Ӧ�õ�Ӧ�ó���
    ClientController controller;
    controller.mainWindow->show();
    return app.exec();
}
