#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "cocos2d.h"
#include "ui/UIButton.h"
#include "audio/include/AudioEngine.h"
#include "GameScene.h"
#include "SettingLayer.h"
#include "QuitLayer.h"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "JniHelper.h"
#endif

using namespace cocos2d::ui;

class MenuScene: public cocos2d::Scene
{
	Label *highScoreLabel;
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	void updatePlayService(float dt);
#endif
	CREATE_FUNC(MenuScene);
};

#endif // __MENU_SCENE_H__

