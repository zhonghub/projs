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