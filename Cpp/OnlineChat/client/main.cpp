#include <QApplication>
#include "mainwindow.h"
// #include "chatWindow.h"
// #include "client.h"

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);
    QFont font;
    font.setFamily("����"); // ��������Ϊ���������֧�����ĵ�����
    app.setFont(font); // ������Ӧ�õ�Ӧ�ó���
    MainWindow mainWindow;
    mainWindow.show();
    return app.exec();
}
