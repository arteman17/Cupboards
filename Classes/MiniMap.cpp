#include "MiniMap.h"

USING_NS_CC;

MiniMap* MiniMap::create(const LevelData& levelData) {
    MiniMap* miniMap = new MiniMap();
    if (miniMap && miniMap->init(levelData)) {
        miniMap->autorelease();
        return miniMap;
    }
    CC_SAFE_DELETE(miniMap);
    return nullptr;
}

bool MiniMap::init(const LevelData& levelData) {
    if (!Node::init()) {
        return false;
    }

    _targetPositions = levelData.targetPositions;
    calculateAutoScale(levelData);
    _miniCenter = Vec2(70, 70);

    // Фон мини-карты
    auto background = LayerColor::create(Color4B(80, 80, 80, 150), -140, 140);
    background->setIgnoreAnchorPointForPosition(false);
    background->setAnchorPoint(Vec2(1.0f, 0.0f));
    background->setPosition(Vec2::ZERO);
    this->addChild(background, -1);

    // Белая рамка
    auto border = DrawNode::create();
    border->drawRect(Vec2(0, 0), Vec2(140, 140), Color4F(1.0f, 1.0f, 1.0f, 0.3f));
    this->addChild(border, 1);

    createConnections(levelData);
    createMiniPoints(levelData);
    createTargetChips(levelData);

    // Заголовок
    auto title = Label::createWithTTF("Target", "fonts/arial.ttf", 14);
    title->setPosition(Vec2(70, 155));
    title->setColor(Color3B::YELLOW);
    this->addChild(title, 10);

    return true;
}

void MiniMap::calculateAutoScale(const LevelData& levelData) {
    if (levelData.points.empty()) {
        _scale = 0.2f;
        return;
    }

    // Находим границы игрового поля
    float minX = FLT_MAX, maxX = FLT_MIN;
    float minY = FLT_MAX, maxY = FLT_MIN;

    for (const auto& point : levelData.points) {
        minX = MIN(minX, point.x);
        maxX = MAX(maxX, point.x);
        minY = MIN(minY, point.y);
        maxY = MAX(maxY, point.y);
    }
    cocos2d::log("MaxX MaxY %d %d", maxX, maxY);
    cocos2d::log("MinX MinY %d %d", minX, minY);

    float width = maxX - minX;
    float height = maxY - minY;
    _center = Vec2(maxX - width / 2, maxY - height / 2);
    float maxDimension = MAX(width, height);

    // Автоматический масштаб
    _scale = (maxDimension > 0) ? 120.0f / maxDimension : 0.2f;
}

void MiniMap::createConnections(const LevelData& levelData) {
    auto connectionsNode = DrawNode::create();

    for (const auto& connection : levelData.connections) {
        if (connection.first < levelData.points.size() &&
            connection.second < levelData.points.size()) {

            Vec2 from = (levelData.points[connection.first] - _center) * _scale + _miniCenter;
            Vec2 to = (levelData.points[connection.second] - _center) * _scale + _miniCenter;

            connectionsNode->drawLine(from, to, Color4F(1.0f, 1.0f, 1.0f, 0.3f));
        }
    }

    this->addChild(connectionsNode, 0);
}

void MiniMap::createMiniPoints(const LevelData& levelData) {
    for (int i = 0; i < levelData.pointCount; ++i) {
        Vec2 miniPoint = (levelData.points[i] - _center) * _scale + _miniCenter;
        _miniPoints.push_back(miniPoint);

        // Создаем точку на мини-карте
        auto point = Sprite::create();
        auto drawNode = DrawNode::create();
        drawNode->drawSolidCircle(Vec2::ZERO, 1.2f, 0, 8, 1.0f, 1.0f, Color4F(1.0f, 1.0f, 1.0f, 0.6f));
        point->addChild(drawNode);
        point->setPosition(miniPoint);
        this->addChild(point);
        _pointSprites.push_back(point);
    }
}

void MiniMap::createTargetChips(const LevelData& levelData) {
    // Яркие цвета для целевых фишек
    std::vector<Color4B> targetColors = {
        cocos2d::Color4B::RED,
        cocos2d::Color4B::BLUE,
        cocos2d::Color4B::GREEN,
        cocos2d::Color4B::MAGENTA,
        cocos2d::Color4B::ORANGE,
        cocos2d::Color4B::GRAY,
        cocos2d::Color4B::WHITE
    };

    // Создаем целевые фишки
    for (int chipId = 0; chipId < levelData.targetPositions.size(); ++chipId) {
        int pointIndex = levelData.targetPositions[chipId];

        if (pointIndex < _miniPoints.size()) {
            Vec2 miniPoint = _miniPoints[pointIndex];

            // Создаем фишку
            auto chip = Sprite::create();
            auto drawNode = DrawNode::create();

            // Заливка
            drawNode->drawSolidCircle(Vec2::ZERO, 4.5f, 0, 16, 1.0f, 1.0f,
                Color4F(targetColors[chipId % targetColors.size()]));

            // Обводка
            drawNode->drawCircle(Vec2::ZERO, 4.5f, 0, 16, false, Color4F(0, 0, 0, 1));

            chip->addChild(drawNode);
            chip->setPosition(miniPoint);
            this->addChild(chip, 2);
        }
    }
}