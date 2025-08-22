#pragma once
#include "cocos2d.h"
#include "GameController.h"

class GameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    CREATE_FUNC(GameScene);

private:
    GameController* _gameController;
};