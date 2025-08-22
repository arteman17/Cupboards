#pragma once
#include "cocos2d.h"

class Chip : public cocos2d::Sprite {
public:
    static Chip* create(int id, const cocos2d::Color4B& color);
    bool init(int id, const cocos2d::Color4B& color);

    int getId() const { return _id; }
    int getCurrentPoint() const { return _currentPoint; }
    void setCurrentPoint(int point) { _currentPoint = point; }

    void highlight(bool highlighted);
    void moveToPosition(const cocos2d::Vec2& position);

private:
    int _id;
    int _currentPoint;
    cocos2d::Color4B _originalColor;
};