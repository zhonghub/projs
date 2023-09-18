#include "sql.h"

// ��̬������ʼ��
SQL_con* SQL_con::instance = nullptr;

/*
 ��ȡ��ǰʱ��
*/
std::string my_sql_use::getCurrentTimeAsString() {
    // ��ȡ��ǰʱ���
    auto now = std::chrono::system_clock::now();
    // ��ʱ���ת��Ϊtime_t����
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    // ʹ��tm�ṹ�彫time_tת��Ϊ����ʱ��
    std::tm localTime;
    localtime_s(&localTime, &currentTime);
    // ��ʽ��ʱ��Ϊ�ַ���
    char timeStr[100];
    std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &localTime);
    // ��ʱ���ַ���ת��Ϊstd::string������
    return std::string(timeStr);
}

/*
 ��һ���ַ�����"#"�ָ�ɶ���Ӵ�
*/
std::vector<std::string> my_sql_use::splitString(const std::string& input, char delimiter) {
    std::vector<std::string> result;
    std::stringstream ss(input);
    std::string item;

    while (std::getline(ss, item, delimiter)) {
        result.push_back(item);
    }
    return result;
}