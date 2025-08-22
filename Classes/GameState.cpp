#include "GameState.h"
#include "Utils.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

USING_NS_CC;

GameState* GameState::_instance = nullptr;

GameState* GameState::getInstance() {
    if (!_instance) {
        _instance = new GameState();
    }
    return _instance;
}

void GameState::destroyInstance() {
    if (_instance) {
        delete _instance;
        _instance = nullptr;
    }
}

GameState::GameState()
    : _currentState(State::MENU)
    , _currentLevel(1)
    , _soundEnabled(true)
    , _musicEnabled(true) {

    loadGameProgress();
}

GameState::~GameState() {
    saveGameProgress();
}

void GameState::setCurrentState(State state) {
    _currentState = state;
    cocos2d::log("Game state changed to: %d", static_cast<int>(state));
}

GameState::State GameState::getCurrentState() const {
    return _currentState;
}

void GameState::loadGameProgress() {
    std::string filePath = FileUtils::getInstance()->getWritablePath() + SAVE_FILE;

    if (!FileUtils::getInstance()->isFileExist(filePath)) {
        // Инициализация дефолтных значений
        for (int i = 1; i <= 10; ++i) {
            LevelStats stats;
            stats.levelNumber = i;
            stats.levelName = "Level " + std::to_string(i);
            stats.movesCount = 0;
            stats.bestMoves = 0;
            stats.completed = false;
            stats.completionTime = 0.0f;
            _levelStats[i] = stats;
        }
        return;
    }

    std::string content = FileUtils::getInstance()->getStringFromFile(filePath);
    rapidjson::Document doc;
    doc.Parse(content.c_str());

    if (doc.HasParseError() || !doc.IsObject()) {
        cocos2d::log("Error parsing save file");
        return;
    }

    // Загрузка настроек
    if (doc.HasMember("sound") && doc["sound"].IsBool()) {
        _soundEnabled = doc["sound"].GetBool();
    }

    if (doc.HasMember("music") && doc["music"].IsBool()) {
        _musicEnabled = doc["music"].GetBool();
    }

    // Загрузка статистики уровней
    if (doc.HasMember("levels") && doc["levels"].IsArray()) {
        const rapidjson::Value& levels = doc["levels"];
        for (rapidjson::SizeType i = 0; i < levels.Size(); i++) {
            const rapidjson::Value& levelData = levels[i];
            if (levelData.IsObject()) {
                LevelStats stats;
                stats.levelNumber = levelData["number"].GetInt();
                stats.bestMoves = levelData["bestMoves"].GetInt();
                stats.completed = levelData["completed"].GetBool();
                stats.completionTime = levelData["time"].GetFloat();
                _levelStats[stats.levelNumber] = stats;
            }
        }
    }
}

void GameState::saveGameProgress() {
    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

    // Сохранение настроек
    doc.AddMember("sound", _soundEnabled, allocator);
    doc.AddMember("music", _musicEnabled, allocator);

    // Сохранение статистики уровней
    rapidjson::Value levels(rapidjson::kArrayType);
    for (const auto& pair : _levelStats) {
        const LevelStats& stats = pair.second;
        rapidjson::Value levelData(rapidjson::kObjectType);
        levelData.AddMember("number", stats.levelNumber, allocator);
        levelData.AddMember("bestMoves", stats.bestMoves, allocator);
        levelData.AddMember("completed", stats.completed, allocator);
        levelData.AddMember("time", stats.completionTime, allocator);
        levels.PushBack(levelData, allocator);
    }
    doc.AddMember("levels", levels, allocator);

    // Запись в файл
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    std::string filePath = FileUtils::getInstance()->getWritablePath() + SAVE_FILE;
    FILE* file = fopen(filePath.c_str(), "w");
    if (file) {
        fputs(buffer.GetString(), file);
        fclose(file);
    }
}

void GameState::resetProgress() {
    _levelStats.clear();
    for (int i = 1; i <= 10; ++i) {
        LevelStats stats;
        stats.levelNumber = i;
        stats.levelName = "Level " + std::to_string(i);
        stats.movesCount = 0;
        stats.bestMoves = 0;
        stats.completed = false;
        stats.completionTime = 0.0f;
        _levelStats[i] = stats;
    }
    saveGameProgress();
}

void GameState::setCurrentLevel(int level) {
    _currentLevel = level;
}

int GameState::getCurrentLevel() const {
    return _currentLevel;
}

void GameState::levelCompleted(int moves, float time) {
    LevelStats& stats = _levelStats[_currentLevel];
    stats.movesCount = moves;
    stats.completed = true;
    stats.completionTime = time;

    if (stats.bestMoves == 0 || moves < stats.bestMoves) {
        stats.bestMoves = moves;
    }

    saveGameProgress();
}

GameState::LevelStats GameState::getLevelStats(int level) const {
    auto it = _levelStats.find(level);
    if (it != _levelStats.end()) {
        return it->second;
    }

    LevelStats defaultStats;
    defaultStats.levelNumber = level;
    defaultStats.levelName = "Level " + std::to_string(level);
    return defaultStats;
}

void GameState::setSoundEnabled(bool enabled) {
    _soundEnabled = enabled;
    saveGameProgress();
}

bool GameState::isSoundEnabled() const {
    return _soundEnabled;
}

void GameState::setMusicEnabled(bool enabled) {
    _musicEnabled = enabled;
    saveGameProgress();
}

bool GameState::isMusicEnabled() const {
    return _musicEnabled;
}