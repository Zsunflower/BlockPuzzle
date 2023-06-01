#include "Tile.h"


TileBase * TileBase::create()
{
	TileBase * ret = new (std::nothrow) TileBase();
	if(ret && ret->init())
	{
		//ret->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

bool TileBase::init()
{
	if(!Node::init())
	{
		return false;
	}
	setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	return true;
}

bool TileBase::initWithSpriteFrameNames(const std::string & backgroundSpriteName, const std::string & foregroundSpriteName)
{
	SpriteFrame *backgroundSprite = SpriteFrameCache::getInstance()->getSpriteFrameByName(backgroundSpriteName);
	SpriteFrame *foregroundSprite = SpriteFrameCache::getInstance()->getSpriteFrameByName(foregroundSpriteName);
	if(backgroundSprite == nullptr || foregroundSprite == nullptr)
		return false;
	_backgroundSprite = Sprite::createWithSpriteFrame(backgroundSprite);
	_foregroundSprite = Sprite::createWithSpriteFrame(foregroundSprite);
	this->addChild(_backgroundSprite);
	this->addChild(_foregroundSprite);
	_contentSize = Size(MAX(_backgroundSprite->getContentSize().width, _foregroundSprite->getContentSize().width),
						MAX(_backgroundSprite->getContentSize().height, _foregroundSprite->getContentSize().height));
	return true;
}

void TileBase::setBackgroundEnable(bool isEnabled)
{
	_backgroundSprite->setVisible(isEnabled);
}

void TileBase::setForegroundEnable(bool isEnabled)
{
	_foregroundSprite->setVisible(isEnabled);
}
