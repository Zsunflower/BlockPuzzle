//
// Created by CUONG on 11/9/2017.
//

#include "QuitLayer.h"

USING_NS_CC;

cocos2d::Layer * QuitLayer::createLayer()
{
    return QuitLayer::create();
}

bool QuitLayer::init()
{
    if (!BaseDialog::init())
    {
        return false;
    }

    auto bg = Sprite::create("quit_board.png");
    bg->setPosition(uiLayer->getContentSize() / 2);
    uiLayer->addChild(bg);

    auto yesBtn = Button::create("yes.png");
    yesBtn->setPosition(Vec2(231, 144));
    yesBtn->addTouchEventListener([&](Ref *ref, Widget::TouchEventType type)
                                  {
                                        if(type != Widget::TouchEventType::ENDED)
                                            return;
                                      Director::getInstance()->end();
                                  });
    bg->addChild(yesBtn);

    auto noBtn = Button::create("no.png");
    noBtn->setPosition(Vec2(634, 144));
    noBtn->addTouchEventListener([&](Ref *ref, Widget::TouchEventType type)
                                  {
                                      if(type != Widget::TouchEventType::ENDED)
                                          return;
                                      disappear();
                                  });
    bg->addChild(noBtn);

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = [&](Touch *touch, Event *event)
    {
        return true;
    };
    touchListener->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    uiLayer->setPosition((visibleSize.width - 1080) / 2, visibleSize.height * 1.5f);
    auto action = MoveTo::create(0.2f, Vec2((visibleSize.width - 1080) / 2, (visibleSize.height - 1920) / 2));
    uiLayer->runAction(action);
    this->addChild(uiLayer);

    return true;
}

void QuitLayer::disappear()
{
    auto action = MoveTo::create(0.2f, Vec2((visibleSize.width - 1080) / 2, visibleSize.height * 1.5f));
    uiLayer->runAction(Sequence::create(action, CallFunc::create([this]() {removeFromParentAndCleanup(true);}), nullptr));
}