#include "Chip.h"

Chip* Chip::create(int id, const cocos2d::Color4B& color) {
    Chip* chip = new Chip();
    if (chip && chip->init(id, color)) {
        chip->autorelease();
        return chip;
    }
    CC_SAFE_DELETE(chip);
    return nullptr;
}

bool Chip::init(int id, const cocos2d::Color4B& color) {
    // Создаем текстуру программно
    auto renderTexture = cocos2d::RenderTexture::create(64, 64);
    renderTexture->beginWithClear(0, 0, 0, 0);

    auto drawNode = cocos2d::DrawNode::create();
    drawNode->drawSolidCircle(cocos2d::Vec2(32, 32), 30, 0, 64, 1.0f, 1.0f,
        cocos2d::Color4F(color));

    drawNode->drawCircle(cocos2d::Vec2(32, 32), 30, 0, 32, false,
        cocos2d::Color4F::BLACK);

    this->addChild(drawNode);

    renderTexture->end();
    Sprite::initWithTexture(renderTexture->getSprite()->getTexture());

    _id = id;
    _originalColor = color;

    // Установка цвета фишки
    this->setColor(cocos2d::Color3B(color));

    return true;
}

void Chip::highlight(bool highlighted) {
    if (highlighted) {
        // Подсветка - желтая обводка
        this->removeAllChildren();

        // Основной круг
        auto mainCircle = cocos2d::DrawNode::create();
        mainCircle->drawSolidCircle(cocos2d::Vec2(32, 32), 30, 0, 32,
            1.0f, 1.0f,
            cocos2d::Color4F(_originalColor));

        // Яркая желтая обводка для подсветки
        auto glow = cocos2d::DrawNode::create();
        glow->drawCircle(cocos2d::Vec2(32, 32), 30, 0, 32, false,
            cocos2d::Color4F::YELLOW);
        glow->setLineWidth(3.0f);

        this->addChild(mainCircle);
        this->addChild(glow);
        this->setScale(1.2f);
    }
    else {
        // Обычное состояние
        this->removeAllChildren();

        auto mainCircle = cocos2d::DrawNode::create();
        mainCircle->drawSolidCircle(cocos2d::Vec2(32, 32), 30, 0, 32,
            1.0f, 1.0f,
            cocos2d::Color4F(_originalColor));

        auto border = cocos2d::DrawNode::create();
        border->drawCircle(cocos2d::Vec2(32, 32), 30, 0, 32, false,
            cocos2d::Color4F::BLACK);

        this->addChild(mainCircle);
        this->addChild(border);
        this->setScale(1.0f);
    }
}

void Chip::moveToPosition(const cocos2d::Vec2& position) {
    auto moveAction = cocos2d::MoveTo::create(0.3f, position);
    auto easeAction = cocos2d::EaseBackOut::create(moveAction);
    this->runAction(easeAction);
}