#include "LevelParser.h"
#include "Utils.h"
#include <fstream>

LevelData LevelParser::parseLevel(const std::string& filename) {
    LevelData data;

    std::string fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(filename);
    cocos2d::log("Full path: %s", fullPath.c_str());

    std::ifstream file(fullPath);

    if (!file.is_open()) {
        cocos2d::log("Failed to open level file: %s", filename.c_str());
        return data;
    }

    std::string line;

    // Чтение количества фишек
    std::getline(file, line);
    data.chipCount = std::stoi(line);
    cocos2d::log("ChipCount: %d", data.chipCount);

    // Чтение количества точек
    std::getline(file, line);
    data.pointCount = std::stoi(line);
    cocos2d::log("PointCount: %d", data.pointCount);


    // Чтение координат точек
    for (int i = 0; i < data.pointCount; ++i) {
        std::getline(file, line);
        auto coords = Utils::splitString(line, ',');
        cocos2d::log("Coords Size: %d", coords.size());
        if (coords.size() == 2) {
            data.points.emplace_back(std::stof(coords[0]), std::stof(coords[1]));
        }
    }
    cocos2d::log("PointsSize: %d", data.points.size());

    // Чтение начальных позиций
    std::getline(file, line);
    auto initial = Utils::splitString(line, ',');
    for (const auto& pos : initial) {
        data.initialPositions.push_back(std::stoi(pos) - 1);
    }

    // Чтение целевых позиций
    std::getline(file, line);
    auto target = Utils::splitString(line, ',');
    for (const auto& pos : target) {
        data.targetPositions.push_back(std::stoi(pos) - 1);
    }

    // Чтение количества соединений
    std::getline(file, line);
    data.connectionCount = std::stoi(line);

    // Чтение соединений
    for (int i = 0; i < data.connectionCount; ++i) {
        std::getline(file, line);
        auto connection = Utils::splitString(line, ',');
        if (connection.size() == 2) {
            data.connections.emplace_back(std::stoi(connection[0]) - 1, std::stoi(connection[1]) - 1);
        }
    }

    file.close();
    return data;
}

std::vector<std::string> LevelParser::getAvailableLevels() {
    return {
        "levels/level1.txt",
        "levels/level2.txt"
    };
}