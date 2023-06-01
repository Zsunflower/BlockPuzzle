#ifndef TILE_BlOCK_H
#define TILE_BlOCK_H

#include "cocos2d.h"
#include "Tile.h"

USING_NS_CC;

enum class TILE_TYPE
{
	X1,
	X2,
	X3,
	X4
};

class TileBlock : public TileBase
{
	Vec2 _coordinate;
	TILE_TYPE _block_type;
	int _tileId;
public:
	static TileBlock* create();
	static TileBlock* createWithId(int tileId);
	void setCoordinate(const Vec2 coordinate)
	{
		_coordinate = coordinate;
	}
	Vec2 getCoordinate()
	{
		return _coordinate;
	}
	void setTileId(int tileId)
	{
		_tileId = tileId;
	}
	void setTileType(TILE_TYPE type);
	TILE_TYPE getTileType() const
	{
		return _block_type;
	}
    friend void saveTileForKey(const char *key, const TileBlock *tileBlock);
    friend TileBlock* getTileForKey(const char *key);
};

void saveTileForKey(const char *key, const TileBlock *tileBlock);
TileBlock* getTileForKey(const char *key);

#endif //!TILE_BlOCK_H
