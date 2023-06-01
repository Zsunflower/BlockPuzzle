#ifndef BLOCK_H
#define BLOCK_H

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "TileBlock.h"
#include "BlockDefine.h"
#include "GameDefine.h"
#include <vector>

USING_NS_CC;
using namespace cocos2d::ui;
using std::vector;

class Block: public Widget
{
	vector<TileBlock*> _blocks;
	int _id, _tileId, _randomeIndex;
	TILE_TYPE _tile_type;
	vector<Vec2> _offset;
	Vec2 _originPos;
	Size _size;
public:
	static Block* create(int id, int tileId = 0);
	friend Block* getBlockForKey(const char *key);
	friend void saveBlockForKey(const char *key, const Block *block);
	bool initWidthId(int id, int tileId);
	const vector<Vec2>& getListOffset();
	Vec2 getFirstItemPosition();
	Vec2 getFirstItemCoordinate();
	void setOriginPos(const Vec2 originPosition);
	Vec2 getOriginPos();
	Size getBlockSize() const;
	vector<TileBlock*>& getAllBlocks();
	void setRandomType(TILE_TYPE type);
};

Block* getBlockForKey(const char *key);
void saveBlockForKey(const char *key, const Block *block);

#endif //!BLOCK_H
