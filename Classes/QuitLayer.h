//
// Created by CUONG on 11/9/2017.
//

#ifndef PROJ_ANDROID_STUDIO_QUITLAYER_H
#define PROJ_ANDROID_STUDIO_QUITLAYER_H


#include "BaseDialog.h"
#include "ui/UIButton.h"
#include "audio/include/AudioEngine.h"
#include "Block.h"

using namespace cocos2d::ui;

class QuitLayer: public BaseDialog
{
public:
    static cocos2d::Layer* createLayer();

    virtual bool init();
    void disappear() override ;

    CREATE_FUNC(QuitLayer);
};



#endif //PROJ_ANDROID_STUDIO_QUITLAYER_H
