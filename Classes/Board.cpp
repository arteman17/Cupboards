#include "Board.h"
#include "PathFinder.h"
#include "LevelParser.h"

Board* Board::create(const LevelData& levelData) {
    Board* board = new Board();
    if (board && board->init(levelData)) {
        board->autorelease();
        return board;
    }
    CC_SAFE_DELETE(board);
    return nullptr;
}

bool Board::init(const LevelData& levelData) {
    if (!Node::init()) {
        return false;
    }

    _points = levelData.points;
    _connections = levelData.connections;
    _targetPositions = levelData.targetPositions;

    createPoints(levelData);
    createConnections(levelData);
    createChips(levelData);

    return true;
}

void Board::createPoints(const LevelData& levelData) {
    for (int i = 0; i < levelData.pointCount; ++i) {
        auto point = cocos2d::Sprite::create();
        auto drawNode = cocos2d::DrawNode::create();
        drawNode->drawSolidCircle(cocos2d::Vec2::ZERO, 10, 0, 16, 1.0f, 1.0f, cocos2d::Color4F::WHITE);
        point->addChild(drawNode);
        point->setPosition(_points[i]);
        point->setScale(0.5f);
        cocos2d::log("Point %d at position: %.1f, %.1f", i, _points[i].x, _points[i].y);
        this->addChild(point);
        _pointSprites.push_back(point);
    }
}

void Board::createConnections(const LevelData& levelData) {
    _connectionsLayer = cocos2d::DrawNode::create();
    this->addChild(_connectionsLayer, -1);

    for (const auto& connection : _connections) {
        auto from = _points[connection.first];
        auto to = _points[connection.second];
        _connectionsLayer->drawLine(from, to, cocos2d::Color4F::WHITE);
    }
}

void Board::createChips(const LevelData& levelData) {
    cocos2d::log("Creating %d chips", levelData.chipCount);

    std::vector<cocos2d::Color4B> chipColors = {
        cocos2d::Color4B::RED,
        cocos2d::Color4B::BLUE,
        cocos2d::Color4B::GREEN,
        cocos2d::Color4B::MAGENTA,
        cocos2d::Color4B::ORANGE,
        cocos2d::Color4B::GRAY,
        cocos2d::Color4B::WHITE
    };

    for (int i = 0; i < levelData.chipCount; ++i) {
        int pointIndex = levelData.initialPositions[i];
        auto color = chipColors[i % chipColors.size()];

        auto chip = Chip::create(i, color);
        chip->setPosition(_points[pointIndex]);
        chip->setCurrentPoint(pointIndex);
        cocos2d::log("Chip %d at point %d, position: %.1f, %.1f",
            i, pointIndex, _points[pointIndex].x, _points[pointIndex].y);

        this->addChild(chip);
        _chips[i] = chip;
        _chipPositions[pointIndex] = i;
    }
}

bool Board::canMoveChip(int fromPoint, int toPoint) const {
    if (_chipPositions.count(toPoint) > 0) {
        return false; // Точка занята
    }

    return isPathClear(fromPoint, toPoint);
}

std::vector<int> Board::getAvailableMoves(int point) const {
    return PathFinder::findAvailableMoves(point, _connections, _chipPositions);
}

bool Board::isPathClear(int fromPoint, int toPoint) const {
    auto path = PathFinder::findPath(fromPoint, toPoint, _connections, _chipPositions);

    for (int point : path) {
        if (point != fromPoint && point != toPoint && _chipPositions.count(point) > 0) {
            return false;
        }
    }

    return !path.empty();
}

bool Board::checkWinCondition() const {
    for (int i = 0; i < _chips.size(); ++i) {
        if (_chips.at(i)->getCurrentPoint() != _targetPositions[i]) {
            return false;
        }
    }
    return true;
}

void Board::setChipPosition(int chipId, int point) {
    int oldPoint = _chips[chipId]->getCurrentPoint();
    _chipPositions.erase(oldPoint);
    _chipPositions[point] = chipId;
    _chips[chipId]->setCurrentPoint(point);
    _chips[chipId]->moveToPosition(_points[point]);
}

int Board::getChipAtPoint(int point) const {
    auto it = _chipPositions.find(point);
    return it != _chipPositions.end() ? it->second : -1;
}

cocos2d::Vec2 Board::getPointPosition(int point) const {
    if (point >= 0 && point < _points.size()) {
        return _points[point];
    }
    return cocos2d::Vec2::ZERO;
}

void Board::highlightAvailableMoves(int point) {
    auto moves = getAvailableMoves(point);
    for (int move : moves) {
        cocos2d::log("Move %d", move);
        _pointSprites[move]->removeAllChildren();
        auto drawNode = cocos2d::DrawNode::create();
        drawNode->drawSolidCircle(cocos2d::Vec2::ZERO, 10, 0, 16, 1.0f, 1.0f, cocos2d::Color4F::GREEN);
        _pointSprites[move]->addChild(drawNode);
    }
}

void Board::clearHighlights() {
    for (auto point : _pointSprites) {
        point->removeAllChildren();
        auto drawNode = cocos2d::DrawNode::create();
        drawNode->drawSolidCircle(cocos2d::Vec2::ZERO, 10, 0, 16, 1.0f, 1.0f, cocos2d::Color4F::WHITE);
        point->addChild(drawNode);
    }
}

cocos2d::Rect Board::getBoundingBox() const {
    if (_points.empty()) {
        return Node::getBoundingBox();
    }

    // Находим границы всех точек
    float minX = FLT_MAX, maxX = FLT_MIN;
    float minY = FLT_MAX, maxY = FLT_MIN;

    for (const auto& point : _points) {
        minX = MIN(minX, point.x);
        maxX = MAX(maxX, point.x);
        minY = MIN(minY, point.y);
        maxY = MAX(maxY, point.y);
    }

    // Добавляем padding для фишек
    const float padding = 30.0f; // Размер фишки + отступ
    minX -= padding;
    maxX += padding;
    minY -= padding;
    maxY += padding;

    // Преобразуем в мировые координаты
    auto worldPos = this->convertToWorldSpace(cocos2d::Vec2(minX, minY));
    auto worldSize = cocos2d::Size(maxX - minX, maxY - minY);

    return cocos2d::Rect(worldPos.x, worldPos.y, worldSize.width, worldSize.height);
}

cocos2d::Vec2 Board::getBoardCenter() const {
    auto bbox = this->getBoundingBox();
    return cocos2d::Vec2(bbox.getMidX(), bbox.getMidY());
}

cocos2d::Size Board::getBoardSize() const {
    return this->getBoundingBox().size;
}