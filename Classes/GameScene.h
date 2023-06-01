#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Block.h"
#include "ui/UIButton.h"
#include "audio/include/AudioEngine.h"
#include "AdmobHelper.h"
#include "Menu.h"
#include <algorithm>

template <typename T>
std::string to_string(T value)
{
	std::ostringstream ss;
	ss << value;
	return ss.str();
}

class GameScene : public cocos2d::Scene
{
	TileBase *_boardBackground[10][10];
	TileBlock *_boardBlock[10][10];
	vector<Vec2> _highlightBlocks;
    std::vector<int> _rows_highlight, _column_highlight;
	Block *block1, *block2, *block3;
	int _numBlocks;
	Size visibleSize;
	Vec2 origin;
	Label *_scoreLabel, *_highScoreLabel;
	int64_t _score;
	unsigned int _scoreFactor;
    bool isHighScored;
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
	void loadGame();
	void createBoard();
	void createBlocks();
	void processExplosion();
	void updateScore();
	void showPlusScore(std::string message, int row, int column, Color4B textColor = Color4B(0xFB, 0xB9, 0x17, 0xFF));
	bool isBlockCanOnBoard(Block *block);
    void highLightRowsAndColumnsFilled();
    void turnOffRowsAndColumnsFilled();
	void releaseBlockToBoard(Block *block);
	void processTileBlockType(TileBlock *tileBlock);
	bool isGameOver();
	void gameOver();
	void blockTouchCallback(Ref *ref, Widget::TouchEventType type);
	inline bool isValidCoordinate(int x, int y);
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
	~GameScene();
};

#endif // __GAME_SCENE_H__
