#include "z_include/myUseFun.h"

// 获取当前时间点
std::string myUse::getCurrentTimeAsString() {
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

// 将key-value键值对转换成一个JSON字符串
std::string myUse::generateJsonString(const std::vector<std::string>& key, const std::vector<std::string>& value) {
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

// 将JSON字符串转换成key-value键值
std::vector<std::string> myUse::parseFromJsonString(std::string jsonStr, std::vector<std::string> & key) {
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

// 将JSON字符串转换成key-value键值
std::map<std::string, std::string> myUse::parseFromJsonString2(std::string jsonStr, std::vector<std::string>& key) {
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