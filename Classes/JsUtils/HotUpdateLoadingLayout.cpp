//
//  HotUpdateLoadingLayout.cpp
//  ZJH
//
//  Created by mac on 2017/5/20.
//  用来接收升级进度
//

#include "HotUpdateLoadingLayout.hpp"
#include "HotUpdateWithNormalView.hpp"
bool HotUpdateLoadingLayout::init(){
    if (!Layout::init()) {
        return false;
    }
    
    Size size = Director::getInstance()->getWinSize();
    auto glview = cocos2d::Director::getInstance()->getOpenGLView();
    if(size.width < size.height)
    {
        glview->setDesignResolutionSize(1080, 1920, ResolutionPolicy::EXACT_FIT);
    }else{
        glview->setDesignResolutionSize(1920, 1080, ResolutionPolicy::EXACT_FIT);
    }
    SpriteFrameCache::getInstance()->removeSpriteFrames();
    Director::getInstance()->getTextureCache()->removeAllTextures();
    _loadLayer = nullptr;
    
    auto winSize = Director::getInstance()->getWinSize();
    _loadLayer = Layer::create();
    addChild(_loadLayer);
    
    Sprite *loadText = Sprite::create("Loading/loading_txt.png");
    loadText->setPosition(Vec2(winSize.width / 2, winSize.height / 2 ));
    _loadLayer->addChild(loadText);
    
    Sprite *dots = Sprite::create("Loading/loading_icon.png");
    dots->setPosition(Vec2(winSize.width / 2, winSize.height / 2 +140));
    _loadLayer->addChild(dots);
    dots->runAction(RepeatForever::create(RotateBy::create(3, 360)));
    
    Color3B c =Color3B(255,230,129);
    _tip = Text::create("", "a", 32);
    _tip->setPosition( Vec2(winSize.width*0.5f,winSize.height / 2 - 300) );
    _tip->setColor(c);
    _loadLayer->addChild(_tip);
    
    _loadingBg = ImageView::create("Loading/loading_bar_bg.png");
    _loadingBg->setPosition(Vec2(winSize.width / 2, winSize.height / 2 - 160));
    _loadLayer->addChild(_loadingBg);
    
    _loading = LoadingBar::create("Loading/loading_bar_fg.png");
    _loading->setPosition(Vec2(winSize.width / 2, winSize.height / 2 - 160));
    _loading->setDirection(LoadingBar::Direction::LEFT);
    _loading->setPercent(0);
    _loadingBg->setVisible(false);
    
    _loadLayer->addChild(_loading);
    _progress = Label::createWithSystemFont("", "a", 32);
    _progress->setPosition( Vec2(winSize.width*0.5f,winSize.height / 2 - 210) );
    
    _progress->setColor(c);
    _loadLayer->addChild(_progress);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(HotUpdateLoadingLayout::notificationCallback), "notify_game_update_process", NULL);
    tips = "游戏更新中(%d/%d)...";
    return true;
}

void HotUpdateLoadingLayout::notificationCallback(cocos2d::Ref *process)
{
    if (process == NULL) {
        return;
    }
    
    __String *str = dynamic_cast<__String *>(process);
    Json::Value json = Utils::ParseJsonStr(str->getCString());
    int gameId = json["gameId"].asInt();
    if(watchGameId != gameId){
        return;
    }
    int currentIndex = json["currenIndex"].asInt();
    int total = json["total"].asInt();
    int event = json["event"].asInt();

    float percent = json["percent"].asDouble();

    if(event == 1){//进度
        _tip->setString(StringUtils::format(tips.c_str(),currentIndex,total) );
        auto str = StringUtils::format("%.2f%%", percent);
        _loading->setPercent(percent);
        _loadingBg->setVisible(true);
        _loadLayer->setVisible(true);
        this->_progress->setString(str);
    }else if(event == 2){ //成功
        _tip->setString("资源更新成功");
        removeFromParent();
    }else {//失败
       _tip->setString("资源更新失败请重试");
       _tip->setTouchEnabled(true);
        _tip->addClickEventListener([=](Ref*){
            _tip->setTouchEnabled(false);
            HotUpdateWithNormalView::hotUpdateModule(watchGameId,this->getParent(),false);
        });
    }
    
}


void HotUpdateLoadingLayout::onExit()
{
    __NotificationCenter::getInstance()->removeAllObservers(this);
    Layout::onExit();
}
