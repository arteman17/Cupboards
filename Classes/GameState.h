#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "cocos2d.h"

class GameState {
public:
    static GameState* getInstance();
    static void destroyInstance();

    // Состояния игры
    enum class State {
        MENU,
        LEVEL_SELECT,
        PLAYING,
        PAUSED,
        LEVEL_COMPLETE,
        GAME_OVER
    };

    // Данные уровня
    struct LevelStats {
        int levelNumber;
        std::string levelName;
        int movesCount;
        int bestMoves;
        bool completed;
        float completionTime;
    };

    void setCurrentState(State state);
    State getCurrentState() const;

    // Управление прогрессом
    void loadGameProgress();
    void saveGameProgress();
    void resetProgress();

    // Статистика уровней
    void setCurrentLevel(int level);
    int getCurrentLevel() const;
    void levelCompleted(int moves, float time);
    LevelStats getLevelStats(int level) const;

    // Настройки
    void setSoundEnabled(bool enabled);
    bool isSoundEnabled() const;
    void setMusicEnabled(bool enabled);
    bool isMusicEnabled() const;

private:
    GameState();
    ~GameState();
    GameState(const GameState&) = delete;
    GameState& operator=(const GameState&) = delete;

    static GameState* _instance;

    State _currentState;
    int _currentLevel;
    std::unordered_map<int, LevelStats> _levelStats;
    bool _soundEnabled;
    bool _musicEnabled;

    const std::string SAVE_FILE = "game_save.json";
};