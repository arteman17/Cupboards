#pragma once
#include "cocos2d.h"
#include "LevelParser.h"

class MiniMap : public cocos2d::Node {
public:
    static MiniMap* create(const LevelData& levelData);
    bool init(const LevelData& levelData);

private:
    void createMiniPoints(const LevelData& levelData);
    void createTargetChips(const LevelData& levelData);
    void createConnections(const LevelData& levelData);
    void calculateAutoScale(const LevelData& levelData);

    std::vector<cocos2d::Vec2> _miniPoints;
    std::vector<cocos2d::Sprite*> _pointSprites;
    std::vector<int> _targetPositions;
    float _scale;
    cocos2d::Vec2 _miniCenter;
    cocos2d::Vec2 _center;
};