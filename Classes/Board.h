#pragma once
#include "cocos2d.h"
#include "Chip.h"
#include "LevelParser.h"
#include <vector>
#include <unordered_map>

class Board : public cocos2d::Node {
public:
    static Board* create(const LevelData& levelData);
    bool init(const LevelData& levelData);

    bool canMoveChip(int fromPoint, int toPoint) const;
    std::vector<int> getAvailableMoves(int point) const;
    bool isPathClear(int fromPoint, int toPoint) const;
    bool checkWinCondition() const;

    void setChipPosition(int chipId, int point);
    int getChipAtPoint(int point) const;
    cocos2d::Vec2 getPointPosition(int point) const;

    virtual cocos2d::Rect getBoundingBox() const override;

    cocos2d::Vec2 getBoardCenter() const;
    cocos2d::Size getBoardSize() const;

    const std::unordered_map<int, Chip*>& getChips() const { return _chips; }
    int getPointCount() const { return static_cast<int>(_points.size()); }
    const std::vector<int>& getTargetPositions() const { return _targetPositions; }

    void highlightAvailableMoves(int point);
    void clearHighlights();

private:
    void createPoints(const LevelData& levelData);
    void createConnections(const LevelData& levelData);
    void createChips(const LevelData& levelData);

    std::vector<cocos2d::Vec2> _points;
    std::vector<std::pair<int, int>> _connections;
    std::vector<int> _targetPositions;
    std::unordered_map<int, int> _chipPositions; // point -> chipId
    std::unordered_map<int, Chip*> _chips;       // chipId -> Chip*

    cocos2d::DrawNode* _connectionsLayer;
    std::vector<cocos2d::Sprite*> _pointSprites;
};