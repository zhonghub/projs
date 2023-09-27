#include "z_include/sql.h"

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


std::string my_sql_use::generateJsonString(const std::vector<std::string>& key, const std::vector<std::string>& value) {
    // ����һ��Json::Value��������ʾJSON����
    Json::Value root;
    if (key.size() != value.size()) {
        return "error";
    }
    // ��Ӽ�ֵ��
    for (int i = 0; i < key.size(); ++i) {
        root[key[i]] = value[i];
    }
    // ��JSON����ת��ΪJSON�ַ���
    Json::StreamWriterBuilder writer;
    writer["emitUTF8"] = true;//utf8֧��,�����,utf8�������ַ�����\uxxx
    std::string jsonString = Json::writeString(writer, root);
    return jsonString;
}

std::vector<std::string> my_sql_use::parseFromJsonString(std::string jsonStr, std::vector<std::string>& key) {
    std::vector<std::string> value;
    // ����JSON�ַ�������ȡ�û���������
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
        std::cerr << "����JSONʱ��������: " << parseErrors << std::endl;
    }
    return value;
}

std::map<std::string, std::string> my_sql_use::parseFromJsonString2(std::string jsonStr, std::vector<std::string>& key) {
    std::map<std::string, std::string> value;
    // ����JSON�ַ�������ȡ�û���������
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
        std::cerr << "����JSONʱ��������: " << parseErrors << std::endl;
    }
    return value;
}