/*
Copyright (c) 2014 Mudafar
GPLv3
*/


#ifndef  __ADMOB_HELPER_H_
#define  __ADMOB_HELPER_H_

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include <string>
#include "GameDefine.h"

class AdmobHelper
{
public:
	static void hideAd();
	static void showAd();
 	static void showInterstitialAd();
	static bool isAdShowing;
	static void signIn();
	static void signOut();
	static bool isSignIn();
	static bool isSignOut();
	static void getHighScore();
	static void showLeaderBoard();
    static std::string getPackageName();
	static void submitScore(int64_t score);
};

#endif // __ADMOB_HELPER_H_

#endif