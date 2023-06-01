#include "Block.h"

Block * Block::create(int id, int tileId)
{
	Block* block = new (std::nothrow) Block();
	if(block && block->initWidthId(id, tileId))
	{
		block->_randomeIndex = -1;
		block->_tile_type = TILE_TYPE::X1;
		block->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
		block->setTouchEnabled(true);
		block->autorelease();
		return block;
	}
	CC_SAFE_DELETE(block);
	return nullptr;
}

Block* getBlockForKey(const char *key)
{
	std::string data = UserDefault::getInstance()->getStringForKey(key, "null");
	if(data == "null")
		return nullptr;
	int id, tileId, randomeIndex, tileType;
	Vec2 originPos;
	sscanf(data.c_str(), "%d,%d,%d,%d,%f,%f", &id, &tileId, &randomeIndex, &tileType, &originPos.x, &originPos.y);
    if(id == -1)
		return nullptr;
	auto block = Block::create(id, tileId);
	if(randomeIndex != -1)
	{
		block->_randomeIndex = randomeIndex;
		block->_blocks[randomeIndex]->setTileType(TILE_TYPE(tileType));
	}
    block->_tile_type = TILE_TYPE(tileType);
    block->_originPos = originPos;
    block->setPosition(originPos);
    return block;
}

void saveBlockForKey(const char *key, const Block *block)
{
	char data[50];
	if(block == nullptr)
		sprintf(data, "%d,%d,%d,%d,%f,%f", -1, -1, -1, -1, 0.0f, 0.0f);
	else
		sprintf(data, "%d,%d,%d,%d,%f,%f", block->_id, block->_tileId, block->_randomeIndex, int(block->_tile_type), block->_originPos.x, block->_originPos.y);
	UserDefault::getInstance()->setStringForKey(key, data);
}

bool Block::initWidthId(int id, int tileId)
{
	if(!Widget::init())
		return false;
	int i, j;
	int imax, jmax;
	setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_id = id;
	_tileId = tileId != 0 ? tileId : std::rand() % NUM_TILE + 1;
	imax = jmax = -1;
	char tileFrameName[20];
	sprintf(tileFrameName, "%d.png", _tileId);
	for(i = 0; i < 5; ++i)
	{
		for(j = 0; j < 5; ++j)
			if(BLOCKS[_id][4 - i][j])
			{
				auto item = TileBlock::create();
				item->setTileId(_tileId);
				item->initWithSpriteFrameNames("blockbg.png", tileFrameName);
				item->setCoordinate(Vec2(j, i));
				item->setPosition(BLOCK_STEP * j + HALF_BLOCK_STEP, BLOCK_STEP * i + HALF_BLOCK_STEP);
				this->addChild(item);
				_blocks.push_back(item);
				imax = imax < i ? i : imax;
				jmax = jmax < j ? j : jmax;
			}
	}
	++imax; ++jmax;
	_contentSize = Size(BLOCK_STEP * jmax + (jmax + 1) * STEP, BLOCK_STEP * imax + (imax + 1) * STEP);
	_size = Vec2(jmax, imax);
	for(auto &item : _blocks)
	{
		_offset.push_back(item->getCoordinate() - _blocks[0]->getCoordinate());
	}
	return true;
}

const vector<Vec2>& Block::getListOffset()
{
	return _offset;
}

Vec2 Block::getFirstItemPosition()
{
	return this->convertToWorldSpace(_blocks[0]->getPosition());
}

Vec2 Block::getFirstItemCoordinate()
{
	return _blocks[0]->getCoordinate();
}

void Block::setOriginPos(Vec2 originPosition)
{
	_originPos = originPosition;
}

Vec2 Block::getOriginPos()
{
	return _originPos;
}

Size Block::getBlockSize() const
{
	return _size;
}

vector<TileBlock*>& Block::getAllBlocks()
{
	return _blocks;
}

void Block::setRandomType(TILE_TYPE type)
{
	_randomeIndex = std::rand() % _blocks.size();
	_tile_type = type;
	_blocks[_randomeIndex]->setTileType(_tile_type);
}