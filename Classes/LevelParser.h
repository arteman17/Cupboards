#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "cocos2d.h"

struct LevelData {
    int chipCount;
    int pointCount;
    std::vector<cocos2d::Vec2> points;
    std::vector<int> initialPositions;
    std::vector<int> targetPositions;
    int connectionCount;
    std::vector<std::pair<int, int>> connections;
};

class LevelParser {
public:
    static LevelData parseLevel(const std::string& filename);
    static std::vector<std::string> getAvailableLevels();
};