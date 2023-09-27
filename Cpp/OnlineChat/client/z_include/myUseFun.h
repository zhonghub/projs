#pragma once

#ifndef MYUSEFUN_H
#define MYUSEFUN_H

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <thread>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <map>

#include <include/json/json.h>

#pragma comment(lib, "ws2_32.lib")

namespace myUse {
    // 获取当前时间
    std::string getCurrentTimeAsString();

    // 将JSON字符串转换成key-value键值
    std::string generateJsonString(const std::vector<std::string>& key, const std::vector<std::string>& value);

    // 将JSON字符串转换成key-value键值，返回包含全部value的vector
    std::vector<std::string> parseFromJsonString(std::string jsonStr, std::vector<std::string>& key);

    // 将JSON字符串转换成key-value键值，返回一个map，value = map[key]
    std::map<std::string, std::string> parseFromJsonString2(std::string jsonStr, std::vector<std::string>& key);
};

#endif // MYUSEFUN_H