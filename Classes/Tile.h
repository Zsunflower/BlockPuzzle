#ifndef TILE_H
#define TILE_H

#include "cocos2d.h"

USING_NS_CC;

class TileBase : public Node
{
protected:
	Sprite *_backgroundSprite{nullptr}, *_foregroundSprite{nullptr};
public:
	static TileBase* create();
	bool init() override;
	bool initWithSpriteFrameNames(const std::string &backgroundSpriteName, const std::string &foregroundSpriteName);
	void setBackgroundEnable(bool isEnabled);
	void setForegroundEnable(bool isEnabled);
	bool isBackgroundEnable() const
	{
		return _backgroundSprite->isVisible();
	}
	bool isForegroundEnable() const
	{
		return _foregroundSprite->isVisible();
	}
};

#endif // !TILE_H

