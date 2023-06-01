#include "GameScene.h"
#include "PauseLayer.h"
#include "GameOverLayer.h"

USING_NS_CC;
using namespace cocos2d::experimental;

const Vec2 BOARD_ORIGIN = Vec2(23.0f, 603.0f);

const Vec2 BOARD_BLOCK_ORIGIN = Vec2(75.0f, 272.0f);

Scene *GameScene::createScene()
{
	return GameScene::create();
}

static float getActualScaleFactor(Node *node)
{
	if (node->getParent() != nullptr)
		return node->getScale() * getActualScaleFactor(node->getParent());
	return node->getScale();
}

static Size getActualContentSize(Node *node)
{
	return node->getContentSize() * getActualScaleFactor(node);
}

bool GameScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	LayerColor *bg = LayerColor::create(Color4B(0x2C, 0x35, 0x39, 0xFF));
    this->addChild(bg);
	auto cup = Sprite::create("cup.png");
	cup->setScale(0.75f);
	cup->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	cup->setPosition(ORIGIN.x + 540.0f, ORIGIN.y + BOARD_ORIGIN.y + 1033 + 20);
	this->addChild(cup);

	_highScoreLabel = Label::createWithTTF(to_string(highScore), "score.ttf", 80, Size::ZERO,
		TextHAlignment::LEFT, TextVAlignment::CENTER);
	_highScoreLabel->setTextColor(Color4B(0x00, 0x80, 0x80, 0xFF));
	_highScoreLabel->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_highScoreLabel->setPosition(cup->getPositionX() + 120, cup->getPositionY());
	this->addChild(_highScoreLabel);

	_scoreLabel = Label::createWithTTF("0", "score.ttf", 80, Size::ZERO, TextHAlignment::RIGHT,
		TextVAlignment::CENTER);
	_scoreLabel->setTextColor(Color4B(0x73, 0x6A, 0xFF, 0xFF));
	_scoreLabel->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
	_scoreLabel->setPosition(cup->getPositionX() - 120, cup->getPositionY());
	this->addChild(_scoreLabel);

	auto pauseBtn = Button::create("pause.png");
	pauseBtn->setPosition(Vec2(visibleSize.width - 60 - pauseBtn->getContentSize().width / 2,
		visibleSize.height - 40 - pauseBtn->getContentSize().height / 2));
	pauseBtn->addTouchEventListener([&](Ref *ref, Widget::TouchEventType type)
	{
		if (type != Widget::TouchEventType::ENDED)
			return;
		PLAY_EFFECT("button_clicked.mp3");
		auto pasueLayer = PauseLayer::createLayer();
		pasueLayer->setName("PauseLayer");
		this->addChild(pasueLayer, 10);
	});
	this->addChild(pauseBtn, 9);

	block1 = block2 = block3 = nullptr;
	_numBlocks = 0;
	_score = 0;
	_scoreFactor = 1;
	isHighScored = false;
	std::srand(time(nullptr));
	createBoard();
	loadGame();

	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyReleased = [&](EventKeyboard::KeyCode keyCode, Event *event)
	{
		if (keyCode == EventKeyboard::KeyCode::KEY_BACK)
		{
			auto overLayer = this->getChildByName("GameOverLayer");
			if (overLayer)
				return;
			auto pauseLayer = this->getChildByName("PauseLayer");
			if (pauseLayer)
			{
				static_cast<PauseLayer *>(pauseLayer)->disappear();
			}
			else
			{
				Director::getInstance()->replaceScene(
					TransitionCrossFade::create(0.25f, MenuScene::createScene()));
			}
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	this->runAction(Sequence::create(DelayTime::create(15.0f), CallFunc::create([this]()
	{
		AdmobHelper::showAd();
	}), nullptr));
#endif

	return true;
}

void GameScene::loadGame()
{
	std::string isSaved = UserDefault::getInstance()->getStringForKey("SaveGame", "null");
	if (isSaved == "null" || isSaved == "no")
	{
		createBlocks();
		int i, j;
		for (i = 0; i < 10; ++i)
		{
			for (j = 0; j < 10; ++j)
			{
				saveTileForKey(to_string(10 * i + j).c_str(), nullptr);
			}
		}
		UserDefault::getInstance()->setLongForKey("currentHighScore", highScore);
		UserDefault::getInstance()->setLongForKey("currentScore", 0);
		if (highScore == 0)
			isHighScored = true;
	}
	else
	{
		//load board
		int i, j;
		for (i = 0; i < 10; ++i)
		{
			for (j = 0; j < 10; ++j)
			{
				_boardBlock[i][j] = getTileForKey(to_string(10 * i + j).c_str());
				if (_boardBlock[i][j])
				{
					_boardBlock[i][j]->setPosition(
						ORIGIN.x + BOARD_ORIGIN.x + BLOCK_STEP * j + HALF_BLOCK_STEP,
						ORIGIN.y + BOARD_ORIGIN.y + BLOCK_STEP * i + HALF_BLOCK_STEP);
                    _boardBlock[i][j]->setBackgroundEnable(false);
					this->addChild(_boardBlock[i][j], 2);
				}
			}
		}
		//load blocks
		block1 = getBlockForKey("Block_1");
		block2 = getBlockForKey("Block_2");
		block3 = getBlockForKey("Block_3");
		if (block1)
		{
			++_numBlocks;
			block1->setScale(0.5f);
			block1->addTouchEventListener(CC_CALLBACK_2(GameScene::blockTouchCallback, this));
			this->addChild(block1, 3);
		}
		if (block2)
		{
			++_numBlocks;
			block2->setScale(0.5f);
			block2->addTouchEventListener(CC_CALLBACK_2(GameScene::blockTouchCallback, this));
			this->addChild(block2, 3);
		}
		if (block3)
		{
			++_numBlocks;
			block3->setScale(0.5f);
			block3->addTouchEventListener(CC_CALLBACK_2(GameScene::blockTouchCallback, this));
			this->addChild(block3, 3);
		}
		if (!block1 && !block2 && !block3)
			createBlocks();
		_score = UserDefault::getInstance()->getLongForKey("currentScore", 0);
		_scoreLabel->setString(to_string(_score));
		auto currentHighScore = UserDefault::getInstance()->getLongForKey("currentHighScore", 0);
		_highScoreLabel->setString(to_string(currentHighScore));
		if (_score > currentHighScore || currentHighScore == 0)
			isHighScored = true;
	}
	UserDefault::getInstance()->setStringForKey("SaveGame", "yes");
}

void GameScene::createBoard()
{
	int i, j;
	for (j = 0; j < 10; ++j)
	{
		for (i = 0; i < 10; ++i)
		{
			auto tileBg = TileBase::create();
			tileBg->initWithSpriteFrameNames("bg.png", "shadow.png");
			tileBg->setForegroundEnable(false);
			tileBg->setPosition(ORIGIN.x + BOARD_ORIGIN.x + BLOCK_STEP * i + HALF_BLOCK_STEP,
				ORIGIN.y + BOARD_ORIGIN.y + BLOCK_STEP * j + HALF_BLOCK_STEP);
			this->addChild(tileBg, 1);
			_boardBackground[j][i] = tileBg;
			_boardBlock[j][i] = nullptr;
		}
	}
}

void GameScene::createBlocks()
{
	Size size1, size2, size3;
	Vec2 pos1, pos2, pos3;
	float spaceDistance;
	block1 = Block::create(std::rand() % NUM_BLOCK_TYPE);
	block2 = Block::create(std::rand() % NUM_BLOCK_TYPE);
	block3 = Block::create(std::rand() % NUM_BLOCK_TYPE);
	if (isGameOver())
		while (true)
		{
			log("======== Recreate block ==========");
			block3 = Block::create(std::rand() % NUM_BLOCK_TYPE);
			if (isBlockCanOnBoard(block3))
				break;
		}
	if (std::rand() % 5 == 4)
	{
		int blockIndex = std::rand() % 3 + 1;
		Block *block;
		int blockType = std::rand() % 3 + 2;
		if (blockIndex == 1)
			block = block1;
		else if (blockIndex == 2)
			block = block2;
		else
			block = block3;
		if (blockType == 2)
			block->setRandomType(TILE_TYPE::X2);
		else if (blockType == 3)
			block->setRandomType(TILE_TYPE::X3);
		else
			block->setRandomType(TILE_TYPE::X4);
	}
	block1->setScale(0.5f);
	block2->setScale(0.5f);
	block3->setScale(0.5f);
	block1->addTouchEventListener(CC_CALLBACK_2(GameScene::blockTouchCallback, this));
	block2->addTouchEventListener(CC_CALLBACK_2(GameScene::blockTouchCallback, this));
	block3->addTouchEventListener(CC_CALLBACK_2(GameScene::blockTouchCallback, this));

	size1 = getActualContentSize(block1);
	size2 = getActualContentSize(block2);
	size3 = getActualContentSize(block3);
	spaceDistance = (930 - size1.width - size2.width - size3.width) / 2;
	pos1 = Vec2(ORIGIN.x + BOARD_BLOCK_ORIGIN.x + size1.width / 2,
		ORIGIN.y + BOARD_BLOCK_ORIGIN.y + 129.5f - size1.height / 2);
	pos2 = Vec2(ORIGIN.x + BOARD_BLOCK_ORIGIN.x + size1.width + spaceDistance + size2.width / 2,
		ORIGIN.y + BOARD_BLOCK_ORIGIN.y + 129.5f - size2.height / 2);
	pos3 = Vec2(ORIGIN.x + BOARD_BLOCK_ORIGIN.x + size1.width + size2.width + 2 * spaceDistance +
		size3.width / 2, ORIGIN.y + BOARD_BLOCK_ORIGIN.y + 129.5f - size3.height / 2);

	block1->setPosition(pos1 + Vec2(visibleSize.width, 0.0f));
	block2->setPosition(pos2 + Vec2(visibleSize.width, 0.0f));
	block3->setPosition(pos3 + Vec2(visibleSize.width, 0.0f));
	block1->setOriginPos(pos1);
	block2->setOriginPos(pos2);
	block3->setOriginPos(pos3);

	this->addChild(block1, 3);
	this->addChild(block2, 3);
	this->addChild(block3, 3);
	_numBlocks = 3;
	auto moveBy = MoveBy::create(0.25f, Vec2(-visibleSize.width, 0.0f));
	block1->runAction(moveBy->clone());
	block2->runAction(moveBy->clone());
	block3->runAction(moveBy->clone());
	saveBlockForKey("Block_1", block1);
	saveBlockForKey("Block_2", block2);
	saveBlockForKey("Block_3", block3);
}

void GameScene::processExplosion()
{
	int i, j;
	bool isFilled;
	vector<int> columns, rows;
	unsigned int localScore = 0;
	auto disappearAction = Sequence::create(Blink::create(0.2f, 2), RemoveSelf::create(), nullptr);
	for (i = 0; i < 10; ++i)
	{
		isFilled = true;
		for (j = 0; j < 10; ++j)
		{
			if (_boardBlock[i][j] == nullptr)
			{
				isFilled = false;
				break;
			}
		}
		if (isFilled)
			rows.push_back(i);
	}
	for (i = 0; i < 10; ++i)
	{
		isFilled = true;
		for (j = 0; j < 10; ++j)
		{
			if (_boardBlock[j][i] == nullptr)
			{
				isFilled = false;
				break;
			}
		}
		if (isFilled)
			columns.push_back(i);
	}
	for (auto row : rows)
	{
		for (i = 0; i < 10; ++i)
		{
			if (_boardBlock[row][i] != nullptr)
			{
				processTileBlockType(_boardBlock[row][i]);
				_boardBlock[row][i]->runAction(disappearAction->clone());
				_boardBlock[row][i] = nullptr;
				saveTileForKey(to_string(10 * row + i).c_str(), nullptr);
			}
		}
		PLAY_EFFECT("block_filled.mp3");
		localScore += 10;
	}
	for (auto column : columns)
	{
		for (i = 0; i < 10; ++i)
		{
			if (_boardBlock[i][column] != nullptr)
			{
				processTileBlockType(_boardBlock[i][column]);
				_boardBlock[i][column]->runAction(disappearAction->clone());
				_boardBlock[i][column] = nullptr;
				saveTileForKey(to_string(10 * i + column).c_str(), nullptr);
			}
		}
		PLAY_EFFECT("block_filled.mp3");
		localScore += 10;
	}
	if (localScore == 0)
		return;
	_score += localScore * _scoreFactor;
	if (_scoreFactor > 1)
		showPlusScore("+" + to_string(localScore * _scoreFactor), rows.size() > 0 ? rows.back() : 5,
			columns.size() > 0 ? columns.back() : 5);

	if (rows.size() + columns.size() > 1)
	{
		localScore = (rows.size() + columns.size()) * 15 * _scoreFactor;
		_score += localScore;
		if (_scoreFactor > 1)
			showPlusScore("+" + to_string(localScore), (rows.size() > 0 ? rows.back() : 5) + 1,
			(columns.size() > 0 ? columns.back() : 5) + 1,
				Color4B(0x4E, 0xE2, 0xEC, 0xFF));
		else
			showPlusScore("+" + to_string(localScore), rows.size() > 0 ? rows.back() : 5,
				columns.size() > 0 ? columns.back() : 5, Color4B(0x4E, 0xE2, 0xEC, 0xFF));
	}
	updateScore();
}

void GameScene::updateScore()
{
	_scoreLabel->setString(to_string(_score));
	UserDefault::getInstance()->setLongForKey("currentScore", _score);
}

void GameScene::showPlusScore(std::string message, int row, int column, Color4B textColor)
{
	auto plusScoreLabel = Label::createWithTTF(message, "plusfont.ttf", 100, Size::ZERO,
		TextHAlignment::CENTER, TextVAlignment::CENTER);
	plusScoreLabel->enableBold();
	plusScoreLabel->enableOutline(Color4B::WHITE, 6);
	plusScoreLabel->setTextColor(textColor);
	plusScoreLabel->setPosition(ORIGIN.x + BOARD_ORIGIN.x + column * BLOCK_STEP + HALF_BLOCK_STEP,
		ORIGIN.y + BOARD_ORIGIN.y + row * BLOCK_STEP + HALF_BLOCK_STEP);
	plusScoreLabel->setScale(0.2f);
	auto scaleMax = ScaleTo::create(0.4f, 1.0f);
	auto scaleMin = ScaleTo::create(0.1f, 0.05f);
	auto actions = Sequence::create(scaleMax, DelayTime::create(1.0f), scaleMin,
		RemoveSelf::create(), nullptr);
	this->addChild(plusScoreLabel, 5);
	plusScoreLabel->runAction(actions);
}

bool GameScene::isBlockCanOnBoard(Block *block)
{
	Size sizeBlock = block->getBlockSize();
	auto listOffsets = block->getListOffset();
	int i, j;
	int xAbs, yAbs;
	for (i = 0; i < 10; ++i)
	{
		for (j = 0; j < 10; ++j)
		{
			if (_boardBlock[i][j] == nullptr)
			{
				bool match = true;
				for (auto offset : listOffsets)
				{
					xAbs = j + offset.x;
					yAbs = i + offset.y;
					if (!isValidCoordinate(xAbs, yAbs) || _boardBlock[yAbs][xAbs] != nullptr)
					{
						match = false;
						break;
					}
				}
				if (match)
				{
					return true;
				}
			}
		}
	}
	return false;
}

void GameScene::releaseBlockToBoard(Block *block)
{
	size_t i;
	auto allBlocks = block->getAllBlocks();
	for (i = 0; i < allBlocks.size(); ++i)
	{
		auto tileBlock = allBlocks[i];
		auto coor = _highlightBlocks[i];
		tileBlock->retain();
		tileBlock->removeFromParentAndCleanup(false);
		tileBlock->setBackgroundEnable(false);
		tileBlock->setPosition(block->convertToWorldSpace(tileBlock->getPosition()));
		this->addChild(tileBlock, 2);
		tileBlock->release();
		_boardBlock[int(coor.y)][int(coor.x)] = tileBlock;
		_boardBackground[int(coor.y)][int(coor.x)]->setForegroundEnable(false);
		saveTileForKey(to_string(10 * int(coor.y) + int(coor.x)).c_str(), tileBlock);
	}
	_score += allBlocks.size() * _scoreFactor;
	_highlightBlocks.clear();
	updateScore();
	processExplosion();
	if (_score > highScore)
	{
		highScore = _score;
		UserDefault::getInstance()->setLongForKey("HighScore", highScore);
		if (!isHighScored)
		{
			isHighScored = true;
			PLAY_EFFECT("high_score.mp3");
		}
	}
	if (block == block1)
	{
		block1 = nullptr;
		saveBlockForKey("Block_1", nullptr);
	}
	else if (block == block2)
	{
		block2 = nullptr;
		saveBlockForKey("Block_2", nullptr);
	}
	else
	{
		block3 = nullptr;
		saveBlockForKey("Block_3", nullptr);
	}
	block->removeFromParentAndCleanup(true);
	--_numBlocks;
	if (_numBlocks == 0)
	{
		createBlocks();
	}
	else
	{
		if (isGameOver())
			this->runAction(Sequence::create(DelayTime::create(0.3f), CallFunc::create([this]()
		{
			gameOver();
		}),
				nullptr));
	}
}

void GameScene::processTileBlockType(TileBlock *tileBlock)
{
	auto tileType = tileBlock->getTileType();
	if (tileType == TILE_TYPE::X1)
		return;
	std::string xSpriteName;
	int multiFactor = 1;
	switch (tileType)
	{
	case TILE_TYPE::X2:
		xSpriteName = "x2.png";
		multiFactor = 2;
		break;
	case TILE_TYPE::X3:
		xSpriteName = "x3.png";
		multiFactor = 3;
		break;
	case TILE_TYPE::X4:
		xSpriteName = "x4.png";
		multiFactor = 4;
		break;
	default:
		break;
	}
	_scoreFactor *= multiFactor;
	auto xLabel = Sprite::createWithSpriteFrameName(xSpriteName);
	xLabel->setPosition(tileBlock->getPosition());
	this->addChild(xLabel, 5);
	auto moveAround = Revolve::create(10.0f, xLabel->getPosition(), 200);
	xLabel->runAction(Sequence::create(moveAround, CallFunc::create([&, multiFactor]()
	{
		_scoreFactor /= multiFactor;
	}), RemoveSelf::create(),
		nullptr));
}

bool GameScene::isGameOver()
{
	bool gameOver = true;
	if (block1 != nullptr && isBlockCanOnBoard(block1))
		gameOver = false;
	else if (block2 != nullptr && isBlockCanOnBoard(block2))
		gameOver = false;
	else if (block3 != nullptr && isBlockCanOnBoard(block3))
		gameOver = false;
	return gameOver;
}

void GameScene::gameOver()
{
	auto gameOverLayer = GameOverLayer::createLayer();
	gameOverLayer->setName("GameOverLayer");
	gameOverLayer->setScores(_score, highScore > 0 ? highScore : _score);
	this->addChild(gameOverLayer, 20);
	UserDefault::getInstance()->setStringForKey("SaveGame", "no");

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	AdmobHelper::submitScore(highScore);
	AdmobHelper::showInterstitialAd();
#endif
	PLAY_EFFECT("game_over.mp3");
}

void GameScene::highLightRowsAndColumnsFilled()
{
	int x, y, i;
	bool filled;
	vector<int> rows_searched, columns_searched;
	for (auto item : _highlightBlocks)
	{
		x = item.x;
		y = item.y;
		filled = true;
		if (std::find(rows_searched.begin(), rows_searched.end(), y) == rows_searched.end())
		{
			rows_searched.push_back(y);
			for (i = 0; i < 10; ++i)
			{
				if (_boardBlock[y][i] == nullptr &&
					std::find(_highlightBlocks.begin(), _highlightBlocks.end(), Vec2(i, y)) ==
					_highlightBlocks.end())
				{
					filled = false;
					break;
				}
			}
			if (filled)
				_rows_highlight.push_back(y);
		}
		filled = true;
		if (std::find(columns_searched.begin(), columns_searched.end(), x) == columns_searched.end())
		{
			columns_searched.push_back(x);
			for (i = 0; i < 10; ++i)
			{
				if (_boardBlock[i][x] == nullptr &&
					std::find(_highlightBlocks.begin(), _highlightBlocks.end(), Vec2(x, i)) ==
					_highlightBlocks.end())
				{
					filled = false;
					break;
				}
			}
			if (filled)
				_column_highlight.push_back(x);
		}
	}
	for (auto row : _rows_highlight)
	{
		for (i = 0; i < 10; ++i)
		{
			if (_boardBlock[row][i])
				_boardBlock[row][i]->setBackgroundEnable(true);   // _boardBlock[row][i]->setOpacity(0x7F);
		}
	}
	for (auto column : _column_highlight)
	{
		for (i = 0; i < 10; ++i)
		{
			if (_boardBlock[i][column])
				_boardBlock[i][column]->setBackgroundEnable(true); //_boardBlock[i][column]->setOpacity(0x7F);
		}
	}
}

void GameScene::turnOffRowsAndColumnsFilled()
{
	int i;
	for (auto row : _rows_highlight)
	{
		for (i = 0; i < 10; ++i)
		{
			if (_boardBlock[row][i])
				_boardBlock[row][i]->setBackgroundEnable(false); //_boardBlock[row][i]->setOpacity(0xFF);
		}
	}
	for (auto column : _column_highlight)
	{
		for (i = 0; i < 10; ++i)
		{
			if (_boardBlock[i][column])
				_boardBlock[i][column]->setBackgroundEnable(false); //_boardBlock[i][column]->setOpacity(0xFF);
		}
	}
	_rows_highlight.clear();
	_column_highlight.clear();
}

void GameScene::blockTouchCallback(Ref *ref, Widget::TouchEventType type)
{
	Block *touchBlock = static_cast<Block *>(ref);
	if (type == Widget::TouchEventType::BEGAN)
	{
		auto touchPos = touchBlock->getTouchBeganPosition();
		touchBlock->setPosition(Vec2(touchPos.x, touchPos.y + BLOCK_SIZE));
		touchBlock->setLocalZOrder(4);
        touchBlock->stopAllActions();
		touchBlock->setScale(1.0f);
		PLAY_EFFECT("block_clicked.mp3");
	}
	else if (type == Widget::TouchEventType::MOVED)
	{
		auto touchPos = touchBlock->getTouchMovePosition();
		touchBlock->setPosition(Vec2(touchPos.x, touchPos.y + BLOCK_SIZE));
		Vec2 firstPos = touchBlock->getFirstItemPosition();
		int x, y;
		for (auto &highlightBl : _highlightBlocks)
		{
			_boardBackground[int(highlightBl.y)][int(highlightBl.x)]->setForegroundEnable(false);
		}
		_highlightBlocks.clear();
		turnOffRowsAndColumnsFilled();

		x = firstPos.x > BOARD_ORIGIN.x + ORIGIN.x ? (firstPos.x - BOARD_ORIGIN.x - ORIGIN.x) /
			BLOCK_STEP : -1;
		y = firstPos.y > BOARD_ORIGIN.y + ORIGIN.y ? (firstPos.y - BOARD_ORIGIN.y - ORIGIN.y) /
			BLOCK_STEP : -1;
		if (isValidCoordinate(x, y))
		{
			bool match = true;
			int xAbs, yAbs;
			auto offsets = touchBlock->getListOffset();
			for (auto offset : offsets)
			{
				xAbs = x + offset.x;
				yAbs = y + offset.y;
				if (!isValidCoordinate(xAbs, yAbs) || _boardBlock[yAbs][xAbs] != nullptr)
				{
					match = false;
					break;
				}
			}
			if (match)
			{
				for (auto offset : offsets)
				{
					xAbs = x + offset.x;
					yAbs = y + offset.y;
					_boardBackground[yAbs][xAbs]->setForegroundEnable(true);
					_highlightBlocks.push_back(Vec2(xAbs, yAbs));
				}
				highLightRowsAndColumnsFilled();
			}
		}
	}
	else
	{
		if (!_highlightBlocks.empty())
		{
			Vec2 coor = _highlightBlocks[0];
			Vec2 offset = Vec2(ORIGIN.x + BOARD_ORIGIN.x + BLOCK_STEP * coor.x + HALF_BLOCK_STEP,
				ORIGIN.y + BOARD_ORIGIN.y + BLOCK_STEP * coor.y + HALF_BLOCK_STEP) -
				touchBlock->getFirstItemPosition();
			touchBlock->setPosition(touchBlock->getPosition() + offset);
			releaseBlockToBoard(touchBlock);
			PLAY_EFFECT("block_place.mp3");
		}
		else
		{
			auto moveTo = MoveTo::create(0.2f, touchBlock->getOriginPos());
			auto scaleTo = ScaleTo::create(0.2f, 0.5f);
			touchBlock->setLocalZOrder(3);
			touchBlock->runAction(Spawn::create(moveTo, scaleTo, nullptr));
			PLAY_EFFECT("block_wind.mp3");
		}
	}
}

inline bool GameScene::isValidCoordinate(int x, int y)
{
	return (x >= 0 && x < 10 && y >= 0 && y < 10);
}

GameScene::~GameScene()
{
}

