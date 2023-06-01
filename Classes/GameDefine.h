#ifndef GAME_DEFINE_H
#define GAME_DEFINE_H

#include "audio/include/AudioEngine.h"
#include "cocos2d.h"
#define PLAY_EFFECT(x) experimental::AudioEngine::play2d(x, false, isSoundOn ? 1.0f : 0.0f)

extern int64_t highScore;
extern int64_t preHighScore;
extern bool isMusicOn;
extern bool isSoundOn;
extern bool isAutoSignIn;
extern int bgmId;
extern cocos2d::Vec2 ORIGIN;
extern cocos2d::Size VISIBLE_SIZE;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
extern std::string packageName;
#endif

#endif
