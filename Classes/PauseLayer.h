//
// Created by CUONG on 11/9/2017.
//

#ifndef PROJ_ANDROID_STUDIO_PAUSELAYER_H
#define PROJ_ANDROID_STUDIO_PAUSELAYER_H

#include "BaseDialog.h"
#include "ui/UIButton.h"
#include "audio/include/AudioEngine.h"
#include "Block.h"
#include "Menu.h"
#include "GameScene.h"

using namespace cocos2d::ui;

class PauseLayer: public BaseDialog
{
public:
    static cocos2d::Layer* createLayer();

    virtual bool init();
    void disappear() override ;

    CREATE_FUNC(PauseLayer);
};

#endif //PROJ_ANDROID_STUDIO_PAUSELAYER_H
