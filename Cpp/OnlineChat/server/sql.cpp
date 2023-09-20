#include "sql.h"

// 静态变量初始化
SQL_con* SQL_con::instance = nullptr;

/*
 获取当前时间
*/
std::string my_sql_use::getCurrentTimeAsString() {
    // 获取当前时间点
    auto now = std::chrono::system_clock::now();
    // 将时间点转换为time_t类型
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    // 使用tm结构体将time_t转换为本地时间
    std::tm localTime;
    localtime_s(&localTime, &currentTime);
    // 格式化时间为字符串
    char timeStr[100];
    std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &localTime);
    // 将时间字符串转换为std::string并返回
    return std::string(timeStr);
}

/*
 将一个字符串以"#"分割成多个子串
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
* 获取当前ip
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
    const char* ip = inet_ntoa(saddr->sin_addr); // 将IP地址转换为字符串

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