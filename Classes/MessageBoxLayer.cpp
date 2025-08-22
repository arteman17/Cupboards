#include "MessageBoxLayer.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

MessageBoxLayer* MessageBoxLayer::create(const std::string& title, const std::string& message,
    const std::function<void()>& okCallback,
    const std::function<void()>& cancelCallback) {
    MessageBoxLayer* layer = new MessageBoxLayer();
    if (layer && layer->init(title, message, okCallback, cancelCallback)) {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return nullptr;
}

bool MessageBoxLayer::init(const std::string& title, const std::string& message,
    const std::function<void()>& okCallback,
    const std::function<void()>& cancelCallback) {
    if (!Layer::init()) {
        return false;
    }

    _okCallback = okCallback;
    _cancelCallback = cancelCallback;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // Затемняющий фон
    auto background = LayerColor::create(Color4B(0, 0, 0, 180));
    this->addChild(background);

    // Диалоговое окно
    auto dialog = ui::Scale9Sprite::create("message_box.png");
    if (!dialog) {
        dialog = ui::Scale9Sprite::create();
        dialog->setContentSize(Size(400, 250));
        dialog->setColor(Color3B::BLACK);
        dialog->setOpacity(200);
    }
    dialog->setPosition(visibleSize / 2);
    this->addChild(dialog);

    // Заголовок
    auto titleLabel = Label::createWithTTF(title, "fonts/arial.ttf", 28);
    titleLabel->setPosition(Vec2(dialog->getContentSize().width / 2,
        dialog->getContentSize().height - 40));
    titleLabel->setColor(Color3B::YELLOW);
    dialog->addChild(titleLabel);

    // Сообщение
    auto messageLabel = Label::createWithTTF(message, "fonts/arial.ttf", 20);
    messageLabel->setPosition(Vec2(dialog->getContentSize().width / 2,
        dialog->getContentSize().height / 2));
    messageLabel->setDimensions(dialog->getContentSize().width - 40, 120);
    messageLabel->setAlignment(TextHAlignment::CENTER);
    dialog->addChild(messageLabel);

    // Кнопка OK
    auto okButton = ui::Button::create();
    okButton->setTitleText("OK");
    okButton->setTitleFontSize(20);
    okButton->setPosition(Vec2(dialog->getContentSize().width / 2 - 60, 40));
    okButton->addClickEventListener(CC_CALLBACK_1(MessageBoxLayer::onOkPressed, this));
    dialog->addChild(okButton);

    // Кнопка Cancel (если нужна)
    if (_cancelCallback) {
        auto cancelButton = ui::Button::create();
        cancelButton->setTitleText("Cancel");
        cancelButton->setTitleFontSize(20);
        cancelButton->setPosition(Vec2(dialog->getContentSize().width / 2 + 60, 40));
        cancelButton->addClickEventListener(CC_CALLBACK_1(MessageBoxLayer::onCancelPressed, this));
        dialog->addChild(cancelButton);
    }

    // Обработка кликов
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [](Touch* touch, Event* event) { return true; };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void MessageBoxLayer::show(cocos2d::Node* parent, const std::string& title,
    const std::string& message, const std::function<void()>& okCallback,
    const std::function<void()>& cancelCallback) {
    auto layer = MessageBoxLayer::create(title, message, okCallback, cancelCallback);
    parent->addChild(layer, 1000);
}

void MessageBoxLayer::onOkPressed(cocos2d::Ref* sender) {
    if (_okCallback) {
        _okCallback();
    }
    this->removeFromParent();
}

void MessageBoxLayer::onCancelPressed(cocos2d::Ref* sender) {
    if (_cancelCallback) {
        _cancelCallback();
    }
    this->removeFromParent();
}