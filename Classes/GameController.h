#pragma once
#include "cocos2d.h"
#include "Board.h"
#include "LevelParser.h"
#include "MiniMap.h"

class GameController : public cocos2d::Node {
public:
    static GameController* create();
    bool init() override;

    void loadLevel(const std::string& levelFile);
    void restartLevel();
    void nextLevel();

private:
    void setupUI();
    void setupEventListeners();
    void onChipSelected(Chip* chip);
    void onPointSelected(int point);
    void checkGameState();
    void update(float dt) override;
    void updateTimer();
    void createMiniMap();

    MiniMap* _miniMap;
    Board* _board;
    LevelData _currentLevel;
    Chip* _selectedChip;
    std::string _currentLevelFile;
    float _levelTime;
    cocos2d::Label* _timerLabel;

    cocos2d::Label* _movesLabel;
    cocos2d::Label* _levelLabel;
    int _movesCount;
};