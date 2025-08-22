#pragma once
#include "cocos2d.h"

class MessageBoxLayer : public cocos2d::Layer {
public:
    static MessageBoxLayer* create(const std::string& title, const std::string& message,
        const std::function<void()>& okCallback = nullptr,
        const std::function<void()>& cancelCallback = nullptr);

    bool init(const std::string& title, const std::string& message,
        const std::function<void()>& okCallback,
        const std::function<void()>& cancelCallback);

    static void show(cocos2d::Node* parent, const std::string& title, const std::string& message,
        const std::function<void()>& okCallback = nullptr,
        const std::function<void()>& cancelCallback = nullptr);

private:
    std::function<void()> _okCallback;
    std::function<void()> _cancelCallback;

    void onOkPressed(cocos2d::Ref* sender);
    void onCancelPressed(cocos2d::Ref* sender);
};