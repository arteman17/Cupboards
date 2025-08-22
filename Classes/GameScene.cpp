#include "GameScene.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
    return GameScene::create();
}

bool GameScene::init()
{
    if (!Scene::init()) {
        return false;
    }

    auto background = LayerColor::create(Color4B(80, 80, 80, 255));
    this->addChild(background, -1);

    // Создаем и добавляем игровой контроллер
    _gameController = GameController::create();
    this->addChild(_gameController);

    return true;
}