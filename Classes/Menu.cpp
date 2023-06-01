#include "Menu.h"


USING_NS_CC;

Scene* MenuScene::createScene()
{
	return MenuScene::create();
}


bool MenuScene::init()
{
	if(!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Layer *uiLayer = Layer::create();
	uiLayer->setContentSize(Size(1080, 1920));
	auto background = LayerColor::create(Color4B(0x25, 0x38, 0x3C, 0xFF));
	this->addChild(background);
    auto logo = Sprite::create("logo.png");
    logo->setPosition(Vec2(540, 1565));
    uiLayer->addChild(logo);
    auto cup = Sprite::create("cup.png");
    cup->setPosition(Vec2(540, 1214));
    uiLayer->addChild(cup);

	auto btnPlay = Button::create("btnPlay.png");
	btnPlay->setPosition(Vec2(540, 734));
	btnPlay->addTouchEventListener([&](Ref *ref, Widget::TouchEventType type)
	{
		if(type != Widget::TouchEventType::ENDED)
			return;
        PLAY_EFFECT("button_clicked.mp3");
		Director::getInstance()->replaceScene(TransitionCrossFade::create(0.25f, GameScene::create()));
	});
	uiLayer->addChild(btnPlay);

	auto btnLB = Button::create("btnLB.png");
	btnLB->setPosition(Vec2(315, 467));
	btnLB->addTouchEventListener([&](Ref *ref, Widget::TouchEventType type)
	{
		if(type != Widget::TouchEventType::ENDED)
			return;
        PLAY_EFFECT("button_clicked.mp3");
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		AdmobHelper::showLeaderBoard();
	#endif
	});
	uiLayer->addChild(btnLB);

	auto rateBtn = Button::create("btnRate.png");
	rateBtn->setPosition(Vec2(540, 467));
	rateBtn->addTouchEventListener([&](Ref *ref, Widget::TouchEventType type)
	{
		if(type != Widget::TouchEventType::ENDED)
			return;
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    Application::getInstance()->openURL("market://details?id=" + packageName);
	#endif 
	});
	uiLayer->addChild(rateBtn);

	auto settingBtn = Button::create("btnSetting.png");
	settingBtn->setPosition(Vec2(765, 467));
	settingBtn->addTouchEventListener([&](Ref *ref, Widget::TouchEventType type)
	{
		if(type != Widget::TouchEventType::ENDED)
			return;
        PLAY_EFFECT("button_clicked.mp3");
        auto settingLayer = SettingLayer::createLayer();
        settingLayer->setName("SettingLayer");
		this->addChild(settingLayer, 1);
	});
	uiLayer->addChild(settingBtn);
    highScore = UserDefault::getInstance()->getLongForKey("HighScore", 0);
    highScoreLabel = Label::createWithTTF(to_string(highScore), "score.ttf", 80);
	highScoreLabel->setTextColor(Color4B(0xfb, 0xae, 0x17, 0xFF));
	highScoreLabel->setPosition(Vec2(540, 995));
    uiLayer->addChild(highScoreLabel);

	uiLayer->setPosition((visibleSize.width - 1080) / 2, (visibleSize.height - 1920) / 2);
	this->addChild(uiLayer);
	if(bgmId == cocos2d::experimental::AudioEngine::INVALID_AUDIO_ID)
	{
		bgmId = cocos2d::experimental::AudioEngine::play2d("bgm.mp3", true, isMusicOn ? 0.5f : 0.0f);
	}

	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyReleased = [&](EventKeyboard::KeyCode keyCode, Event* event)
	{
		if (keyCode == EventKeyboard::KeyCode::KEY_BACK)
		{
			auto pauseLayer = this->getChildByName("QuitLayer");
			if(pauseLayer)
			{
				static_cast<QuitLayer*>(pauseLayer)->disappear();
			}
			else
			{
                auto settingLayer = this->getChildByName("SettingLayer");
                if(settingLayer)
                {
                    static_cast<SettingLayer*>(settingLayer)->disappear();
                }
                else
                {
                    auto quitLayer = QuitLayer::createLayer();
                    quitLayer->setName("QuitLayer");
                    this->addChild(quitLayer, 10);
                }
			}
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	this->runAction(Sequence::create(DelayTime::create(15.0f), CallFunc::create([this]() {AdmobHelper::showAd();}), nullptr));
    this->schedule(schedule_selector(MenuScene::updatePlayService), 2.0f);
#endif
    return true;
}

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
void MenuScene::updatePlayService(float dt)
{
    if(!AdmobHelper::isSignIn())
        return;
    if(preHighScore > -1)
    {
        if(preHighScore > highScore)
        {
            highScore = preHighScore;
            highScoreLabel->setString(to_string(highScore));
            UserDefault::getInstance()->setLongForKey("HighScore", highScore);
        }
        else
        {
            AdmobHelper::submitScore(highScore);
        }
        this->unschedule(schedule_selector(MenuScene::updatePlayService));
    }
    else
    {
        AdmobHelper::getHighScore();
    }
}
#endif