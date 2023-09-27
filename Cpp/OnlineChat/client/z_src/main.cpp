#include <QApplication>
#include "z_include/MainWindow.h"
#include "z_include/ClientController.h"

int main1() {

    std::vector<std::string> key;
    std::vector<std::string> value;
    key.push_back("username");
    value.push_back("����");
    key.push_back("password");
    value.push_back("123456");
    key.push_back("time");
    value.push_back("9/2237");

    std::string jsonStr = myUse::generateJsonString(key, value);
    // ���JSON�ַ���
    std::cout << "JSON�ַ���: " << jsonStr << std::endl;

    key.push_back("msg");

    std::vector<std::string> value2 = myUse::parseFromJsonString(jsonStr, key);
    std::cout << " \nresult = \n";
    for (auto& x : value2) {
        std::cout << x << " \n";
    }

    std::cout << " \nresult = \n";
    std::map<std::string, std::string> map2 = myUse::parseFromJsonString2(jsonStr, key);
    for (auto& x : key) {
        std::cout << map2[x] << " \n";
    }

    return 0;
}

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
