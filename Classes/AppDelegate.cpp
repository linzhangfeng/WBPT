#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "ZJH.h"
#include "Scene/Hall/HallScene.hpp"

#include "Scene/Load/GameLoad.hpp"
#include "Scene/Load/AgreementLayer.h"
#include "JsUtils/JsUtils.h"
#include "extensions/cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

using namespace CocosDenshion;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    log("AppDelegate_time_test0:%ld",time(NULL));
        FileUtils::getInstance()->addSearchPath(Utils::getSavePath());
    FileUtils::getInstance()->addSearchPath(Utils::getSavePath()+"js/");
    
#if COCOS2D_DEBUG == 1
    FileUtils::getInstance()->addSearchPath("/Users/admin/Desktop/self_svn/majiang/");
    FileUtils::getInstance()->addSearchPath("/Users/admin/Desktop/self_svn/majiang2/");
    FileUtils::getInstance()->addSearchPath("/Users/admin/Desktop/self_svn/poker/");
    FileUtils::getInstance()->addSearchPath("/Users/admin/Desktop/WBPT/scripts/public/");
//    FileUtils::getInstance()->addSearchPath("/Users/admin/Desktop/khd/WBPT/scripts/");
#endif

    FileUtils::getInstance()->addSearchPath("/mnt/shared/Other/wanbei_script/");
    FileUtils::getInstance()->addSearchPath("/mnt/shared/wanbei_script/");
    
    FileUtils::getInstance()->addSearchPath("res/");
    FileUtils::getInstance()->addSearchPath("js/");
    FileUtils::getInstance()->createDirectory(Utils::getSavePath());
    FileUtils::getInstance()->addSearchPath("res/GameHall/record/");
    FileUtils::getInstance()->addSearchPath("res/GameHall/img/");
    //预加载音乐
    
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("ZJH", Rect(0, 0, 1080, 1920));
#else
        glview = GLViewImpl::create("ZJH");
#endif
        director->setOpenGLView(glview);
    }
    
    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 30);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //处理8.0.2版本的登陆不了问题
    log("ppppp 11111111111111 ====== %lf\n", PlatformHelper::dealLow_8Version());
    
    if(PlatformHelper::dealLow_8Version() >= 8.0f && PlatformHelper::dealLow_8Version() <= 8.2f)
    {
        //不做任何事情
        
    }else
    {
        Size size = cocos2d::Director::getInstance()->getWinSize();
        Size _size = glview->getFrameSize();
        glview->setFrameSize(_size.height, _size.width);
        PlatformHelper::cConViewH();

    }
    
#endif
    
    // Set the design resolution
    glview->setDesignResolutionSize(1080, 1920, ResolutionPolicy::EXACT_FIT);

    register_all_packages();
//    ZJHModel::getInstance()->pLuaguaDict = __Dictionary::createWithContentsOfFile("Language/string.xml");
//    ZJHModel::getInstance()->pLuaguaDict->retain();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (0)
    {
        std::string url = UserDefault::getInstance()->getStringForKey("default_use_url","http://wb.boxwan.cn/chess/");
        Manifest::_baseUrl = url;
        CCHttpAgent::getInstance()->setUrl(url);
        CCHttpAgent::getInstance()->setImageUploadUrl(url);
    }
    
    if (1)
    {
        Manifest::_baseUrl ="http://wb.boxwan.cn/chess/";
        CCHttpAgent::getInstance()->setUrl("http://wb.boxwan.cn/chess/");
        CCHttpAgent::getInstance()->setImageUploadUrl("http://wb.boxwan.cn/chess/");
    }
    
    if (0)
    {
        Manifest::_baseUrl ="http://188.188.1.111:20084/chess/";
        CCHttpAgent::getInstance()->setUrl("http://188.188.1.111:20006/chess/");
        CCHttpAgent::getInstance()->setImageUploadUrl("http://188.188.1.111:20006/chess/");
    }
    
    if (0)
    {
        Manifest::_baseUrl ="http://188.188.1.53:9700/chess-service/";
        CCHttpAgent::getInstance()->setUrl("http://188.188.1.53:9700/chess-service/");
        CCHttpAgent::getInstance()->setImageUploadUrl("http://188.188.1.53:9700/chess-service/");
    }
    
    if (1)
    {
        Manifest::_baseUrl ="http://188.188.1.202:20084/chess/";
        CCHttpAgent::getInstance()->setUrl("http://188.188.1.202:20006/chess/");
        CCHttpAgent::getInstance()->setImageUploadUrl("http://188.188.1.202:20006/chess/");
    }
    
    if (0)
    {
        Manifest::_baseUrl ="http://188.188.1.202:20084/chess/";
        CCHttpAgent::getInstance()->setUrl("http://112.124.35.138:20002/");
        CCHttpAgent::getInstance()->setImageUploadUrl("http://188.188.1.202:20006/chess/");
    }
    
#else
    std::string server = PlatformHelper::sdkDoForStr_C("server");
    if (server == "release")
    {
        std::string url = UserDefault::getInstance()->getStringForKey("default_use_url","http://wb.boxwan.cn/chess/");
        Manifest::_baseUrl = url;
        CCHttpAgent::getInstance()->setUrl(url);
        CCHttpAgent::getInstance()->setImageUploadUrl(url);
    }
    else if (server == "debug_111")
    {
        Manifest::_baseUrl ="http://188.188.1.111:20084/chess/";
        CCHttpAgent::getInstance()->setUrl("http://188.188.1.111:20006/chess/");
        CCHttpAgent::getInstance()->setImageUploadUrl("http://188.188.1.111:20006/chess/");
    }
    else if (server == "debug_xiao")
    {
        Manifest::_baseUrl ="http://188.188.1.53:9700/chess-service/";
        CCHttpAgent::getInstance()->setUrl("http://188.188.1.53:9700/chess-service/");
        CCHttpAgent::getInstance()->setImageUploadUrl("http://188.188.1.53:9700/chess-service/");
    }
    else if (server == "debug_he")
    {
        Manifest::_baseUrl ="http://188.188.1.202:20084/chess/";
        CCHttpAgent::getInstance()->setUrl("http://188.188.1.34:8080/chess-service/");
        CCHttpAgent::getInstance()->setImageUploadUrl("http://188.188.1.202:20006/chess/");
    }
    else
    {
        Manifest::_baseUrl ="http://188.188.1.202:20084/chess/";
        CCHttpAgent::getInstance()->setUrl("http://188.188.1.202:20006/chess/");
        CCHttpAgent::getInstance()->setImageUploadUrl("http://188.188.1.202:20006/chess/");
    }
    
    std::string version = PlatformHelper::sdkDoForStr_C("version");
    if (version != "")
    {
        ZJHModel::getInstance()->versionIOS = version;
        ZJHModel::getInstance()->vname = version;
    }
    
    int vercode = PlatformHelper::sdkDoForInt_C("vercode");
    if (vercode != 0)
    {
        ZJHModel::getInstance()->vcode = vercode;
    }
#endif
//    JsUtils::getInstance();
//    JsUtils::initLoad();
//    JsUtils::runScript("slwh/scripts/main.js");
//    director->runWithScene(HelloWorld::createScene());
//    return true;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
//    Sound::getInstance()->StopBgSound();
//    
    bool haveReadAgreement = UserDefault::getInstance()->getBoolForKey("have_read_agreement", false);
    
    if (haveReadAgreement) {
//        director->runWithScene(GameLoad::createScene());
        ZJHModel::getInstance()->gotoGameLoad(true);
    } else {
        director->runWithScene(AgreementLayer::createScene());
    }
//    director->runWithScene(GameHallScene::createScene());
#else
    ZJHModel::getInstance()->gotoGameLoad(true);
//        director->runWithScene(HelloWorld::createScene());
//    director->runWithScene(GameHallScene::createScene());
    
#endif
      log("AppDelegate_time_test1:%ld",time(NULL));
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    
    auto director = Director::getInstance();
    director->stopAnimation();
    director->getEventDispatcher()->dispatchCustomEvent("game_on_hide");
    
    ZJHModel::getInstance()->isPause = 1;
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->stopAllEffects();
    
    ZJHModel::getInstance()->pauseTime = (int)time(NULL);
    if(ZJHModel::getInstance()->enterGame)ZJHModel::getInstance()->gamePuase = true;
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    auto director = Director::getInstance();
    director->startAnimation();
    director->getEventDispatcher()->dispatchCustomEvent("game_on_show");
    ZJHModel::getInstance()->GameResum();
}
