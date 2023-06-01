//
// Created by CUONG on 11/9/2017.
//

#ifndef PROJ_ANDROID_STUDIO_BASEDIALOG_H
#define PROJ_ANDROID_STUDIO_BASEDIALOG_H


#include "cocos2d.h"

using namespace cocos2d::ui;

class BaseDialog: public cocos2d::Layer
{
protected:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    cocos2d::LayerColor *bgLayer;
    cocos2d::Layer *uiLayer;
public:
    virtual bool init();
    virtual void disappear();
    CREATE_FUNC(BaseDialog);
};

#endif //PROJ_ANDROID_STUDIO_BASEDIALOG_H
