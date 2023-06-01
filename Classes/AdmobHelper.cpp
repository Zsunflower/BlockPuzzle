/*
Copyright (c) 2014 Mudafar
GPLv3
*/

#include "cocos2d.h"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "AdmobHelper.h"
#include "audio/include/AudioEngine.h"

bool AdmobHelper::isAdShowing = true;

#include "platform/android/jni/JniHelper.h"
#include "GameDefine.h"
#include <jni.h>

const char* AppActivityClassName = "org/cocos2dx/cpp/AppActivity";

void AdmobHelper::hideAd()
{
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "hideAd", "()V"))
	{

		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
		isAdShowing = false;
	}
}

void AdmobHelper::showAd()
{
	cocos2d::log("=============== show ads ====================");
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "showAd", "()V"))
	{

		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
		isAdShowing = true;
	}
}

void AdmobHelper::showInterstitialAd()
{
	cocos2d::log("=============== display showInterstitialAd ====================");
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "showInterstitialAd", "()V"))
	{

		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
}

void AdmobHelper::signIn()
{
	cocos2d::log("=============== signIn ====================");
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "signIn", "()V"))
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
}

void AdmobHelper::signOut()
{
	cocos2d::log("=============== signOut ====================");
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "signOut", "()V"))
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
}

bool AdmobHelper::isSignIn()
{
	return cocos2d::JniHelper::callStaticBooleanMethod(AppActivityClassName, "isSignedIn");
}

bool AdmobHelper::isSignOut()
{
	return cocos2d::JniHelper::callStaticBooleanMethod(AppActivityClassName, "isSignedOut");
}

void AdmobHelper::getHighScore()
{
	 cocos2d::JniHelper::callStaticVoidMethod(AppActivityClassName, "getHighScore");
}

void  AdmobHelper::showLeaderBoard()
{
	cocos2d::log("=============== showLeaderBoard ====================");
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "showLeaderBoard", "()V"))
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
}

std::string AdmobHelper::getPackageName()
{
	cocos2d::log("=============== getPackageName ====================");
	return cocos2d::JniHelper::callStaticStringMethod(AppActivityClassName, "myGetPackageName");
}

void AdmobHelper::submitScore(int64_t score)
{
	cocos2d::JniHelper::callStaticVoidMethod(AppActivityClassName, "submitScore", score);
}

extern "C" {

	void Java_org_cocos2dx_cpp_AppActivity_callbackOnAdLoaded(JNIEnv *env, jobject thiz)
	{
		cocos2d::log("=============== Java_org_cocos2dx_cpp_AppActivity_callbackOnAdLoaded ====================");
	}

	void Java_org_cocos2dx_cpp_AppActivity_callbackOnAdFailedToLoad(JNIEnv *env, jobject thiz)
	{
		cocos2d::log("=============== Java_org_cocos2dx_cpp_AppActivity_callbackOnAdFailedToLoad ====================");
	}

	void Java_org_cocos2dx_cpp_AppActivity_callbackOnAdOpened(JNIEnv *env, jobject thiz)
	{
		cocos2d::log("=============== Java_org_cocos2dx_cpp_AppActivity_callbackOnAdOpened ====================");
		cocos2d::Director::getInstance()->stopAnimation();
		cocos2d::experimental::AudioEngine::pauseAll();
	}

	void Java_org_cocos2dx_cpp_AppActivity_callbackOnAdLeftApplication(JNIEnv *env, jobject thiz)
	{
		cocos2d::log("=============== Java_org_cocos2dx_cpp_AppActivity_callbackOnAdLeftApplication ====================");
	}

	void Java_org_cocos2dx_cpp_AppActivity_callbackOnAdClosed(JNIEnv *env, jobject thiz)
	{
		cocos2d::log("=============== Java_org_cocos2dx_cpp_AppActivity_callbackOnAdClosed ====================");
		cocos2d::Director::getInstance()->startAnimation();
		cocos2d::experimental::AudioEngine::resumeAll();
	}

void Java_org_cocos2dx_cpp_AppActivity_callbackOnReciveScore(JNIEnv *env, jobject thiz, jlong score)
{
	preHighScore = (int64_t) score;
	cocos2d::log("=============== Recived score %d ====================", int(preHighScore));
}

}

#endif

