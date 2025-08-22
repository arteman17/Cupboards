#include "Utils.h"
#include <sstream>
#include <random>

std::vector<std::string> Utils::splitString(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delimiter)) {
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }

    return tokens;
}

std::string Utils::getFileName(const std::string& path) {
    size_t lastSlash = path.find_last_of("/\\");
    size_t lastDot = path.find_last_of(".");

    if (lastSlash != std::string::npos) {
        if (lastDot != std::string::npos && lastDot > lastSlash) {
            return path.substr(lastSlash + 1, lastDot - lastSlash - 1);
        }
        return path.substr(lastSlash + 1);
    }

    return path;
}

cocos2d::Color4B Utils::randomColor() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(50, 255);

    return cocos2d::Color4B(dis(gen), dis(gen), dis(gen), 255);
}