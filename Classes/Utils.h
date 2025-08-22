#pragma once
#include <string>
#include <vector>
#include "cocos2d.h"

namespace Utils {
    std::vector<std::string> splitString(const std::string& str, char delimiter);
    std::string getFileName(const std::string& path);
    cocos2d::Color4B randomColor();
}