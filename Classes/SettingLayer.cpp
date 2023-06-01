#include "SettingLayer.h"
#include "AdmobHelper.h"

USING_NS_CC;

cocos2d::Layer * SettingLayer::createLayer()
{
	return SettingLayer::create();
}

bool SettingLayer::init()
{
	if (!BaseDialog::init())
	{
		return false;
	}

	auto bg = Sprite::create("setting_board.png");
	bg->setPosition(uiLayer->getContentSize() / 2);
	uiLayer->addChild(bg);

	auto btnMusic = Button::create();
	if (isMusicOn)
		btnMusic->loadTextureNormal("on.png");
	else
		btnMusic->loadTextureNormal("off.png");
	btnMusic->addTouchEventListener([&](Ref *ref, Widget::TouchEventType type)
	{
		if (type != Widget::TouchEventType::ENDED)
			return;
		PLAY_EFFECT("button_clicked.mp3");
		isMusicOn = !isMusicOn;
		if (isMusicOn)
			static_cast<Button*>(ref)->loadTextureNormal("on.png");
		else
			static_cast<Button*>(ref)->loadTextureNormal("off.png");
		UserDefault::getInstance()->setBoolForKey("isMusicOn", isMusicOn);
		experimental::AudioEngine::setVolume(bgmId, isMusicOn ? 0.5f : 0.0f);
    });
	btnMusic->setPosition(Vec2(714, 624));
	bg->addChild(btnMusic);

	auto btnSound = Button::create();
	if (isSoundOn)
		btnSound->loadTextureNormal("on.png");
	else
		btnSound->loadTextureNormal("off.png");
	btnSound->addTouchEventListener([&](Ref *ref, Widget::TouchEventType type)
	{
		if (type != Widget::TouchEventType::ENDED)
			return;
		PLAY_EFFECT("button_clicked.mp3");
		isSoundOn = !isSoundOn;
		if (isSoundOn)
			static_cast<Button*>(ref)->loadTextureNormal("on.png");
		else
			static_cast<Button*>(ref)->loadTextureNormal("off.png");
		UserDefault::getInstance()->setBoolForKey("isSoundOn", isSoundOn);
	});
	btnSound->setPosition(Vec2(714, 467));
	bg->addChild(btnSound);

	auto btnMode = Button::create();
	btnMode->setName("PlayService");

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (AdmobHelper::isSignIn())
		btnMode->loadTextureNormal("on.png");
	else
		btnMode->loadTextureNormal("off.png");
	btnMode->addTouchEventListener([&](Ref *ref, Widget::TouchEventType type)
	{
		if (type != Widget::TouchEventType::ENDED)
			return;
		PLAY_EFFECT("button_clicked.mp3");
		if (AdmobHelper::isSignIn())
		{
			AdmobHelper::signOut();
			UserDefault::getInstance()->setBoolForKey("isAutoSignIn", false);
		}
		else
		{
			AdmobHelper::signIn();
			UserDefault::getInstance()->setBoolForKey("isAutoSignIn", true);
		}
	});

	this->runAction(RepeatForever::create(Sequence::create(DelayTime::create(1.0f), CallFunc::create([this, btnMode]()
	{
		if (AdmobHelper::isSignIn())
			btnMode->loadTextureNormal("on.png");
		else
			btnMode->loadTextureNormal("off.png");
	}),
		nullptr)));
	btnMode->setPosition(Vec2(714, 310));
	bg->addChild(btnMode);
#endif

	auto btnClose = Button::create("close.png", "close_selected.png");
	btnClose->setPosition(Vec2(432, 114));
	btnClose->addTouchEventListener([&](Ref *ref, Widget::TouchEventType type)
	{
		if (type != Widget::TouchEventType::ENDED)
			return;
		PLAY_EFFECT("button_clicked.mp3");
		disappear();
	});
	bg->addChild(btnClose);

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

void SettingLayer::disappear()
{
	auto action = MoveTo::create(0.2f, Vec2((visibleSize.width - 1080) / 2, visibleSize.height * 1.5f));
	uiLayer->runAction(Sequence::create(action, CallFunc::create([this]() {removeFromParentAndCleanup(true); }), nullptr));
}