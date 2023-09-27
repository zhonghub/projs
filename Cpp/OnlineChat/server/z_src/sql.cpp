#include "z_include/sql.h"

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


std::string my_sql_use::generateJsonString(const std::vector<std::string>& key, const std::vector<std::string>& value) {
    // 创建一个Json::Value对象来表示JSON数据
    Json::Value root;
    if (key.size() != value.size()) {
        return "error";
    }
    // 添加键值对
    for (int i = 0; i < key.size(); ++i) {
        root[key[i]] = value[i];
    }
    // 将JSON对象转换为JSON字符串
    Json::StreamWriterBuilder writer;
    writer["emitUTF8"] = true;//utf8支持,加这句,utf8的中文字符会编程\uxxx
    std::string jsonString = Json::writeString(writer, root);
    return jsonString;
}

std::vector<std::string> my_sql_use::parseFromJsonString(std::string jsonStr, std::vector<std::string>& key) {
    std::vector<std::string> value;
    // 解析JSON字符串并提取用户名和密码
    Json::CharReaderBuilder reader;
    std::istringstream jsonStream(jsonStr);
    Json::Value parsedRoot;
    std::string parseErrors;

    if (Json::parseFromStream(reader, jsonStream, &parsedRoot, &parseErrors)) {
        for (auto& x : key) {
            value.push_back(parsedRoot.get(x, "null").asString());
        }
    }
    else {
        std::cerr << "解析JSON时发生错误: " << parseErrors << std::endl;
    }
    return value;
}

std::map<std::string, std::string> my_sql_use::parseFromJsonString2(std::string jsonStr, std::vector<std::string>& key) {
    std::map<std::string, std::string> value;
    // 解析JSON字符串并提取用户名和密码
    Json::CharReaderBuilder reader;
    std::istringstream jsonStream(jsonStr);
    Json::Value parsedRoot;
    std::string parseErrors;

    if (Json::parseFromStream(reader, jsonStream, &parsedRoot, &parseErrors)) {
        for (auto& x : key) {
            value[x] = parsedRoot.get(x, "null").asString();
        }
    }
    else {
        std::cerr << "解析JSON时发生错误: " << parseErrors << std::endl;
    }
    return value;
}