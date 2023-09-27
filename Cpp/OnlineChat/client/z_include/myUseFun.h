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
    // ��ȡ��ǰʱ��
    std::string getCurrentTimeAsString();

    // ��JSON�ַ���ת����key-value��ֵ
    std::string generateJsonString(const std::vector<std::string>& key, const std::vector<std::string>& value);

    // ��JSON�ַ���ת����key-value��ֵ�����ذ���ȫ��value��vector
    std::vector<std::string> parseFromJsonString(std::string jsonStr, std::vector<std::string>& key);

    // ��JSON�ַ���ת����key-value��ֵ������һ��map��value = map[key]
    std::map<std::string, std::string> parseFromJsonString2(std::string jsonStr, std::vector<std::string>& key);
};

#endif // MYUSEFUN_H