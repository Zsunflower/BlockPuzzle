//
// Created by CUONG on 11/9/2017.
//

#include "BaseDialog.h"

USING_NS_CC;

bool BaseDialog::init()
{
    if(!Layer::init())
    {
        return false;
    }
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    bgLayer = LayerColor::create(Color4B(0, 0, 0, 120));
    this->addChild(bgLayer);
    uiLayer = Layer::create();
    uiLayer->setContentSize(Size(1080, 1920));
    return true;
}

void BaseDialog::disappear()
{
}