//
// Created by CUONG on 11/9/2017.
//
#include "PauseLayer.h"

USING_NS_CC;

cocos2d::Layer * PauseLayer::createLayer()
{
	return PauseLayer::create();
}

bool PauseLayer::init()
{
	if (!BaseDialog::init())
	{
		return false;
	}

	auto bg = Sprite::create("pause_board.png");
	bg->setPosition(uiLayer->getContentSize() / 2);
	uiLayer->addChild(bg);

	auto sound = Button::create();
	if (isSoundOn)
		sound->loadTextureNormal("sound.png");
	else
		sound->loadTextureNormal("sound_off.png");
	sound->addTouchEventListener([&](Ref *ref, Widget::TouchEventType type)
	{
		if (type != Widget::TouchEventType::ENDED)
			return;
		PLAY_EFFECT("button_clicked.mp3");
		isSoundOn = !isSoundOn;
		if (isSoundOn)
			static_cast<Button*>(ref)->loadTextureNormal("sound.png");
		else
			static_cast<Button*>(ref)->loadTextureNormal("sound_off.png");
		UserDefault::getInstance()->setBoolForKey("isSoundOn", isSoundOn);
	});
	sound->setPosition(Vec2(240, 575));
	bg->addChild(sound);

	auto music = Button::create();
	if (isMusicOn)
		music->loadTextureNormal("music.png");
	else
		music->loadTextureNormal("music_off.png");
	music->addTouchEventListener([&](Ref *ref, Widget::TouchEventType type)
	{
		if (type != Widget::TouchEventType::ENDED)
			return;
		PLAY_EFFECT("button_clicked.mp3");
		isMusicOn = !isMusicOn;
		if (isMusicOn)
			static_cast<Button*>(ref)->loadTextureNormal("music.png");
		else
			static_cast<Button*>(ref)->loadTextureNormal("music_off.png");
		UserDefault::getInstance()->setBoolForKey("isMusicOn", isMusicOn);
		experimental::AudioEngine::setVolume(bgmId, isMusicOn ? 0.5f : 0.0f);
	});
	music->setPosition(Vec2(625, 575));
	bg->addChild(music);

	auto home = Button::create("homeBtn.png");
	home->addTouchEventListener([&](Ref *ref, Widget::TouchEventType type)
	{
		if (type != Widget::TouchEventType::ENDED)
			return;
		PLAY_EFFECT("button_clicked.mp3");
		Director::getInstance()->replaceScene(TransitionCrossFade::create(0.25f, MenuScene::createScene()));
	});
	home->setPosition(Vec2(240, 395));
	bg->addChild(home);

	auto newGame = Button::create("reloadBtn.png");
	newGame->addTouchEventListener([&](Ref *ref, Widget::TouchEventType type)
	{
		if (type != Widget::TouchEventType::ENDED)
			return;
		PLAY_EFFECT("button_clicked.mp3");
		UserDefault::getInstance()->setStringForKey("SaveGame", "no");
		Director::getInstance()->replaceScene(GameScene::createScene());
	});
	newGame->setPosition(Vec2(625, 395));
	bg->addChild(newGame);

	auto resume = Button::create("resume.png");
	resume->addTouchEventListener([&](Ref *ref, Widget::TouchEventType type)
	{
		if (type != Widget::TouchEventType::ENDED)
			return;
		PLAY_EFFECT("button_clicked.mp3");
		disappear();
	});
	resume->setPosition(Vec2(432, 160));
	bg->addChild(resume);

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

void PauseLayer::disappear()
{
	auto action = MoveTo::create(0.2f, Vec2((visibleSize.width - 1080) / 2, visibleSize.height * 1.5f));
	uiLayer->runAction(Sequence::create(action, CallFunc::create([this]() {removeFromParentAndCleanup(true);}), nullptr));
}