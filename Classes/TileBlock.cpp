#include "TileBlock.h"


TileBlock * TileBlock::create()
{
	TileBlock * ret = new (std::nothrow) TileBlock();
	if(ret && ret->init())
	{
		ret->_block_type = TILE_TYPE::X1;
		ret->setCascadeOpacityEnabled(true);
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

TileBlock* TileBlock::createWithId(int tileId)
{
    TileBlock * ret = new (std::nothrow) TileBlock();
    char tileFrameName[20];
    sprintf(tileFrameName, "%d.png", tileId);
    if(ret && ret->init() && ret->initWithSpriteFrameNames("blockbg.png", tileFrameName))
    {
        ret->_tileId = tileId;
        ret->_block_type = TILE_TYPE::X1;
        ret->setCascadeOpacityEnabled(true);
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void TileBlock::setTileType(TILE_TYPE type)
{
	_block_type = type;
	if(_block_type == TILE_TYPE::X1)
		return;
	std::string labelString;
	switch(_block_type)
	{
		case TILE_TYPE::X2:
			labelString = "x2.png";
			break;
		case TILE_TYPE::X3:
			labelString = "x3.png";
			break;
		case TILE_TYPE::X4:
			labelString = "x4.png";
			break;
		default:
			break;
	}
	auto label = Sprite::createWithSpriteFrameName(labelString);
	this->addChild(label, 1);
}

void saveTileForKey(const char *key, const TileBlock *tileBlock)
{
    char data[50];
    if(tileBlock == nullptr)
        sprintf(data, "%d,%d", -1, -1);
    else
        sprintf(data, "%d,%d", tileBlock->_tileId, int(tileBlock->_block_type));
    UserDefault::getInstance()->setStringForKey(key, data);
}

TileBlock* getTileForKey(const char *key)
{
    std::string data = UserDefault::getInstance()->getStringForKey(key, "null");
    if(data == "null")
        return nullptr;
    int id, type;
    sscanf(data.c_str(), "%d,%d", &id, &type);
    if(id == -1)
        return nullptr;
    auto tileBlock = TileBlock::createWithId(id);
    tileBlock->setTileType(TILE_TYPE(type));
	return tileBlock;
}