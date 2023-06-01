//
// Created by CUONG on 11/10/2017.
//

#ifndef PROJ_ANDROID_STUDIO_GAMEOVERLAYER_H
#define PROJ_ANDROID_STUDIO_GAMEOVERLAYER_H



#include "BaseDialog.h"
#include "ui/UIButton.h"

using namespace cocos2d::ui;

class GameOverLayer: public BaseDialog
{
    cocos2d::Label *_scoreLabel, *_highScoreLabel;
public:
    static GameOverLayer* createLayer();

    virtual bool init();
    void disappear() override ;
    void setScores(int64_t score, int64_t highScore);
    CREATE_FUNC(GameOverLayer);
};

#endif //PROJ_ANDROID_STUDIO_GAMEOVERLAYER_H
