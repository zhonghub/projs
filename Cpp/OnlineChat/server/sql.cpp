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

/*
* ��ȡ��ǰip
*/
std::string my_sql_use::getLocalIPAddress() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock." << std::endl;
        return "";
    }

    char hostName[256];
    if (gethostname(hostName, sizeof(hostName)) != 0) {
        std::cerr << "Failed to get hostname." << std::endl;
        WSACleanup();
        return "";
    }

    struct addrinfo hints;
    struct addrinfo* result = nullptr;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(hostName, nullptr, &hints, &result) != 0) {
        std::cerr << "Failed to get address info." << std::endl;
        WSACleanup();
        return "";
    }

    struct sockaddr_in* saddr = reinterpret_cast<struct sockaddr_in*>(result->ai_addr);
    const char* ip = inet_ntoa(saddr->sin_addr); // ��IP��ַת��Ϊ�ַ���

    freeaddrinfo(result);
    WSACleanup();

    if (ip != nullptr) {
        return std::string(ip);
    }
    else {
        std::cerr << "Failed to convert IP address to string." << std::endl;
        return "";
    }
}