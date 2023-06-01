#ifndef __SETTING_LAYER_H__
#define __SETTING_LAYER_H__

#include "BaseDialog.h"
#include "ui/UIButton.h"
#include "audio/include/AudioEngine.h"
#include "Block.h"

using namespace cocos2d::ui;

class SettingLayer: public BaseDialog
{
public:
	static cocos2d::Layer* createLayer();
	virtual bool init();
	void disappear() override ;
	CREATE_FUNC(SettingLayer);
};

#endif // __SETTING_LAYER_H__

