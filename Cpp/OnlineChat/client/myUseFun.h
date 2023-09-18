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

#pragma comment(lib, "ws2_32.lib")

namespace myUse {
    std::string getCurrentTimeAsString();
    std::vector<std::string> splitString(const std::string& input, char delimiter);
};

#endif // MYUSEFUN_H