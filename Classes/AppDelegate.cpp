#include "AppDelegate.h"
#include "Menu.h"

#define USE_AUDIO_ENGINE 1
// #define USE_SIMPLE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(1080, 1920);
static cocos2d::Size screenSize = cocos2d::Size(800, 1024);
Vec2 ORIGIN = Vec2();
Size VISIBLE_SIZE = Size();
int64_t highScore = 0;
bool isMusicOn = false;
bool isSoundOn = true;
int64_t preHighScore = -1;
bool isAutoSignIn = true;
int bgmId = AudioEngine::INVALID_AUDIO_ID;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
std::string packageName;
#endif

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
	// set OpenGL context attributes: red,green,blue,alpha,depth,stencil
	GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8 };

	GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
	return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching()
{
	// initialize director
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if (!glview)
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		glview = GLViewImpl::createWithRect("BlockPuzzle", cocos2d::Rect(0, 0, screenSize.width, screenSize.height));
#else
		glview = GLViewImpl::create("BlockPuzzle");
#endif
		director->setOpenGLView(glview);
	}
	// turn on display FPS
	//director->setDisplayStats(true);

	// set FPS. the default value is 1.0/60 if you don't call this
	director->setAnimationInterval(1.0f / 60);

	// Set the design resolution
	glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_HEIGHT);
	// if the frame's height is larger than the height of medium size.

	register_all_packages();

	auto frameSize = glview->getFrameSize();
	auto scale = MIN(frameSize.width / designResolutionSize.width, frameSize.height / designResolutionSize.height);
	ORIGIN.x = (frameSize.width - designResolutionSize.width * scale);
	ORIGIN.y = (frameSize.height - designResolutionSize.height * scale);

	isMusicOn = UserDefault::getInstance()->getBoolForKey("isMusicOn", false);
	isSoundOn = UserDefault::getInstance()->getBoolForKey("isSoundOn", true);
	isAutoSignIn = UserDefault::getInstance()->getBoolForKey("isAutoSignIn", true);

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (isAutoSignIn)
	{
		AdmobHelper::signIn();
	}
	packageName = AdmobHelper::getPackageName();
	AdmobHelper::hideAd();
#endif

	FileUtils::getInstance()->addSearchPath("res");
	FileUtils::getInstance()->addSearchPath("sounds");
	FileUtils::getInstance()->addSearchPath("fonts");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("blocks.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("xscore.plist");

	auto scene = MenuScene::createScene();
	director->runWithScene(scene);

	return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground()
{
	Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
	AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
	SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
	Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
	AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
	SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}
