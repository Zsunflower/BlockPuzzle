//
// Created by CUONG on 11/10/2017.
//

#include "GameOverLayer.h"
#include "AdmobHelper.h"
#include "GameScene.h"
#include "Menu.h"

USING_NS_CC;

GameOverLayer * GameOverLayer::createLayer()
{
	return GameOverLayer::create();
}

bool GameOverLayer::init()
{
	if (!BaseDialog::init())
	{
		return false;
	}

	auto bg = Sprite::create("game_over_board.png");
	bg->setPosition(uiLayer->getContentSize() / 2);
	uiLayer->addChild(bg);

	auto reloadBtn = Button::create("reloadBtn.png");
	reloadBtn->setPosition(Vec2(432, 130));
	reloadBtn->addTouchEventListener([this](Ref *ref, Widget::TouchEventType type)
	{
		if (type != Widget::TouchEventType::ENDED)
			return;
		PLAY_EFFECT("button_clicked.mp3");
		Director::getInstance()->replaceScene(GameScene::createScene());
	});
	bg->addChild(reloadBtn);

	auto homedBtn = Button::create("homeBtn.png");
	homedBtn->setPosition(Vec2(225, 322));
	homedBtn->addTouchEventListener([this](Ref *ref, Widget::TouchEventType type)
	{
		if (type != Widget::TouchEventType::ENDED)
			return;
		PLAY_EFFECT("button_clicked.mp3");
		Director::getInstance()->replaceScene(TransitionCrossFade::create(0.25f, MenuScene::createScene()));
	});
	bg->addChild(homedBtn);

	auto lbBtn = Button::create("lbBtn1.png");
	lbBtn->setPosition(Vec2(640, 322));
	lbBtn->addTouchEventListener([this](Ref *ref, Widget::TouchEventType type)
	{
		if (type != Widget::TouchEventType::ENDED)
			return;
		PLAY_EFFECT("button_clicked.mp3");
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		AdmobHelper::showLeaderBoard();
#endif
	});
	bg->addChild(lbBtn);

	_scoreLabel = Label::createWithTTF("", "score.ttf", 100, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	_scoreLabel->setPosition(Vec2(432, 655));
	bg->addChild(_scoreLabel);

	_highScoreLabel = Label::createWithTTF("", "score.ttf", 100, Size::ZERO, TextHAlignment::LEFT, TextVAlignment::CENTER);
	_highScoreLabel->setTextColor(Color4B(0xFB, 0xAE, 0x17, 0xFF));
	_highScoreLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	_highScoreLabel->setPosition(Vec2(432, 509));
	bg->addChild(_highScoreLabel);

	auto preview = Sprite::create("preview.png");
	preview->setPosition(Vec2(432, -68));
	preview->runAction(RepeatForever::create(Blink::create(0.75f, 2)));
	bg->addChild(preview);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = [&](Touch *touch, Event *event)
	{
		return true;
	};
	touchListener->onTouchMoved = [this](Touch *touch, Event *event)
	{
		uiLayer->setVisible(false);
		bgLayer->setOpacity(0x00);
	};
	touchListener->onTouchEnded = [this](Touch *touch, Event *event)
	{
		uiLayer->setVisible(true);
		bgLayer->setOpacity(120);
	};
	touchListener->onTouchCancelled = [this](Touch *touch, Event *event)
	{
		uiLayer->setVisible(true);
		bgLayer->setOpacity(120);
	};

	touchListener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	uiLayer->setPosition((visibleSize.width - 1080) / 2, visibleSize.height * 1.5f);
	auto action = MoveTo::create(0.2f, Vec2((visibleSize.width - 1080) / 2, (visibleSize.height - 1920) / 2));
	uiLayer->runAction(action);
	this->addChild(uiLayer);

	return true;
}

void GameOverLayer::disappear()
{
	auto action = MoveTo::create(0.2f, Vec2((visibleSize.width - 1080) / 2, visibleSize.height * 1.5f));
	uiLayer->runAction(Sequence::create(action, CallFunc::create([this]() {removeFromParentAndCleanup(true); }), nullptr));
}

void GameOverLayer::setScores(int64_t score, int64_t highScore)
{
	_scoreLabel->setString(to_string(score));
	_highScoreLabel->setString(to_string(highScore));
}