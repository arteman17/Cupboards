#include "GameController.h"
#include "Utils.h"
#include "GameState.h"
#include "MessageBoxLayer.h"

GameController* GameController::create() {
    GameController* controller = new GameController();
    if (controller && controller->init()) {
        controller->autorelease();
        return controller;
    }
    CC_SAFE_DELETE(controller);
    return nullptr;
}

bool GameController::init() {
    if (!Node::init()) {
        return false;
    }

    _movesCount = 0;
    _selectedChip = nullptr;
    _levelTime = 0.0f;

    this->scheduleUpdate();
    setupUI();
    createMiniMap();
    setupEventListeners();


    // Загрузка первого уровня
    auto levels = LevelParser::getAvailableLevels();
    if (!levels.empty()) {
        loadLevel(levels[0]);
    }

    return true;
}

void GameController::setupUI() {
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

    _levelLabel = cocos2d::Label::createWithTTF("Level: 1", "fonts/arial.ttf", 24);
    _levelLabel->setPosition(visibleSize.width - 100, visibleSize.height - 30);
    this->addChild(_levelLabel);

    _movesLabel = cocos2d::Label::createWithTTF("Moves: 0", "fonts/arial.ttf", 24);
    _movesLabel->setPosition(visibleSize.width - 100, visibleSize.height - 60);
    this->addChild(_movesLabel);

    _timerLabel = cocos2d::Label::createWithTTF("Time: 0s", "fonts/arial.ttf", 24);
    _timerLabel->setPosition(visibleSize.width - 100, visibleSize.height - 90);
    this->addChild(_timerLabel);

    auto restartBtn = cocos2d::MenuItemLabel::create(
        cocos2d::Label::createWithTTF("Restart", "fonts/arial.ttf", 20),
        [this](Ref* sender) { restartLevel(); }
    );
    restartBtn->setPosition(visibleSize.width / 2, 50);

    auto menu = cocos2d::Menu::create(restartBtn, nullptr);
    menu->setPosition(cocos2d::Vec2::ZERO);
    this->addChild(menu);
}

void GameController::setupEventListeners() {
    auto touchListener = cocos2d::EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = [this](cocos2d::Touch* touch, cocos2d::Event* event) {
        if (_board && GameState::getInstance()->getCurrentState() == GameState::State::PLAYING) {
            auto touchPos = touch->getLocation();

            // Convert to board coordinates if needed
            auto boardPos = _board->convertToNodeSpace(touchPos);

            // Check chips
            const auto& chips = _board->getChips();
            for (const auto& chipPair : chips) {
                auto chip = chipPair.second;
                if (chip->getBoundingBox().containsPoint(boardPos)) {
                    onChipSelected(chip);
                    return true;
                }
            }

            // Check points
            for (int i = 0; i < _board->getPointCount(); ++i) {
                auto pointPos = _board->getPointPosition(i);
                float distance = pointPos.distance(boardPos);
                if (distance < 30.0f) {
                    onPointSelected(i);
                    return true;
                }
            }
        }
        return false;
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void GameController::onChipSelected(Chip* chip) {
    if (_selectedChip) {
        _selectedChip->highlight(false);
    }

    _selectedChip = chip;
    chip->highlight(true);

    _board->clearHighlights();
    _board->highlightAvailableMoves(chip->getCurrentPoint());
    cocos2d::log("HELOOOOOOOO");
}

void GameController::onPointSelected(int point) {
    if (!_selectedChip ||
        GameState::getInstance()->getCurrentState() != GameState::State::PLAYING) {
        return;
    }

    int currentPoint = _selectedChip->getCurrentPoint();

    if (_board->canMoveChip(currentPoint, point)) {
        _board->setChipPosition(_selectedChip->getId(), point);
        _movesCount++;
        _movesLabel->setString("Moves: " + std::to_string(_movesCount));

        _selectedChip->highlight(false);
        _selectedChip = nullptr;
        _board->clearHighlights();

        checkGameState();
    }
}

void GameController::checkGameState() {
    if (_board->checkWinCondition()) {
        GameState::getInstance()->levelCompleted(_movesCount, _levelTime);
        GameState::getInstance()->setCurrentState(GameState::State::LEVEL_COMPLETE);

        std::string message = "Level completed!\nMoves: " + std::to_string(_movesCount) +
            "\nTime: " + std::to_string(static_cast<int>(_levelTime)) + "s";
        MessageBoxLayer::show(this, "Victory", message, [this]() {
            this->nextLevel();
        });
    }
}

void GameController::update(float dt) {
    if (GameState::getInstance()->getCurrentState() == GameState::State::PLAYING) {
        _levelTime += dt;
        updateTimer();
    }
}

void GameController::updateTimer() {
    _timerLabel->setString("Time: " + std::to_string(static_cast<int>(_levelTime)) + "s");
}

void GameController::loadLevel(const std::string& levelFile) {
    _currentLevelFile = levelFile;
    _currentLevel = LevelParser::parseLevel(levelFile);
    _movesCount = 0;
    _levelTime = 0.0f;
    _movesLabel->setString("Moves: 0");
    updateTimer();

    GameState::getInstance()->setCurrentState(GameState::State::PLAYING);

    if (_board) {
        _board->removeFromParent();
    }

    if (_miniMap) {
        _miniMap->removeFromParent();
        _miniMap = nullptr;
    }

    _board = Board::create(_currentLevel);

    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    auto boardCenter = _board->getBoardCenter();
    cocos2d::Vec2 screenCenter = visibleSize / 2;

    // Вычисляем смещение
    cocos2d::Vec2 offset = screenCenter - boardCenter;
    _board->setPosition(_board->getPosition() + offset);

    this->addChild(_board, 10);

    createMiniMap();

    // Обновление номера уровня в UI
    auto levelName = Utils::getFileName(levelFile);
    _levelLabel->setString("Level: " + levelName);
}

void GameController::restartLevel() {
    loadLevel(_currentLevelFile);
}

void GameController::nextLevel() {
    auto levels = LevelParser::getAvailableLevels();
    auto it = std::find(levels.begin(), levels.end(), _currentLevelFile);

    if (it != levels.end() && std::next(it) != levels.end()) {
        loadLevel(*std::next(it));
    }
    else {
        MessageBoxLayer::show(this, "All levels completed!", "Game Over");
    }
}

void GameController::createMiniMap() {
    _miniMap = MiniMap::create(_currentLevel);

    // Позиция в нижнем правом углу с небольшим отступом
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    _miniMap->setAnchorPoint(cocos2d::Vec2(1.0f, 0.0f));
    _miniMap->setPosition(visibleSize.width - 150, 10);

    this->addChild(_miniMap, 100); // Высокий z-order чтобы была поверх всего
}