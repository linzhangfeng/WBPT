//
//  LoadLayer.cpp
//  ZJH
//
//  Created by apple on 16/7/8.
//
//

#include "LoadLayer.hpp"
#include "../Loading/Loading.h"
#include "../../Model/MsgModel.h"
#include "../Load/GameLoad.hpp"

bool LoadLayer::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }

    ZJHModel::getInstance()->UserKey = "";
    setHead(0);
    setName("LoadLayer");
    setCanReturn(false);
    
    setBackground(Color4B(0x1d, 0x1d, 0x1e, 255));
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float H = visibleSize.height - getHeadH();
    
    ImageView *bg = ImageView::create("Mine/load_BG.png");
    bg->setContentSize(visibleSize);
    bg->ignoreContentAdaptWithSize(false);
    bg->setAnchorPoint(Vec2(0, 0));
    bg->setPosition(Vec2(0, 0));
    this->addChild(bg);
    
    Button *weiBtn = Button::create("Mine/load_weiXin_Btn1.png","Mine/load_weiXin_Btn2.png", "Mine/load_weiXin_Btn2.png");
    weiBtn->setAnchorPoint(Vec2(0.5, 0));
    weiBtn->setPosition(Vec2(visibleSize.width/2, 670));
    this->addChild(weiBtn);
    weiBtn->addClickEventListener([=](Ref* sender){
        
        LoadLayer::loadByWeiXin();
    });

    PlatformHelper::registWeiXinLoginId();
    //为了检测是否装了微信，注册应用宝微信的ID
    if(PlatformHelper::checkIsInstallWeiXin() == false)
    {
        weiBtn->setVisible(false);
        
    }
    
    Button *phoneBtn = Button::create("Mine/load_phone_btn1.png","Mine/load_phone_btn2.png", "Mine/load_phone_btn2.png");
    phoneBtn->setAnchorPoint(Vec2(0.5, 0));
    phoneBtn->setPosition(Vec2(visibleSize.width/2, 515));
    this->addChild(phoneBtn);
    phoneBtn->addClickEventListener([=](Ref* sender){
        
        ZJHModel::getInstance()->gotoView(VIEW_PHONE_LOAD);
    });

    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LoadLayer::thirdWXLoginPost), "wx_login_code", NULL);
    
    
    userAgreementView();
    
    runAction(Sequence::create(DelayTime::create(0),CallFunc::create([=]{
        PlatformHelper::sdkDoForVoid_C("initSDK");
        ZJHModel::getInstance()->initPhoneData();
        CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
            this->dealAppUpdateLoadView();
        },"version/clientGameVersion", "","check_update_Load");
    }), NULL));
    
    return true;
}

void LoadLayer::dealAppUpdateLoadView()
{
    //处理平台的升级
    if (getReferenceCount() == 0 || getReferenceCount() > 10)
    {
        return;
    }
    CCHttpPacket *checkUpdatePacket = CCHttpAgent::getInstance()->packets["check_update_Load"];
    if (checkUpdatePacket && checkUpdatePacket->status != 0)
    {
        if (checkUpdatePacket->status != 3)
        {
            return;
        }
        
        checkUpdatePacket->status = 0;
        if (checkUpdatePacket->resultIsOK())
        {
            int isForce = checkUpdatePacket->recvVal["resultMap"]["version"]["force"].asInt();
            int curCode = atoi(checkUpdatePacket->recvVal["resultMap"]["version"]["code"].asString().c_str());
            if (curCode > ZJHModel::getInstance()->vcode)
            {
                //提示升级需要增加限制,一天提示一次
                if (isForce == 0)
                {
                    PlatformHelper::showUpdateDialog(checkUpdatePacket->recvVal["resultMap"]["version"]);
                }
                else
                {
                    double lastTime = UserDefault::getInstance()->getDoubleForKey("update_tips",0.0);
                    long long currentTime = Utils::getCurrentSecend();
                    double interval = (currentTime-lastTime)*0.001/60/60;
                    
                    log("＝＝＝ lastTime[%f]==currentTime[%lld]==interval[%f]",lastTime,currentTime, interval);
                    if (interval > 24)
                    {
                        UserDefault::getInstance()->setDoubleForKey("update_tips", currentTime);
                        PlatformHelper::showUpdateDialog(checkUpdatePacket->recvVal["resultMap"]["version"]);
                    }
                }
            }
        }
        CCHttpAgent::getInstance()->packets["check_update_Load"] = NULL;
        delete checkUpdatePacket;
    }
    
}

void LoadLayer::exitTipView()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    Layout *mask = Layout::create();
    mask->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    mask->setBackGroundColor(Color3B(0x00, 0x00, 0x00));
    mask->setBackGroundColorOpacity(200);
    mask->setContentSize(visibleSize);
    this->addChild(mask);
    
    ImageView * bg = ImageView::create("Public/bar_window_yellow.png");
    bg->setTouchEnabled(true);
    bg->setSwallowTouches(true);
    bg->setScale9Enabled(true);
    bg->setContentSize(Size(900, 400));
    bg->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    mask->addChild(bg);
    
    Text *title = Text::create("如果您不同意用户协议无法进入游戏！", ".SFUIDisplay-Semibold", 40);
    title->setTextColor(Color4B(0xf0, 0xcb, 0x5c, 255));
    title->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    title->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height/2+50));
    bg->addChild(title);
    
    Button *cacelBut = Button::create("Mine/btn_yellow_small.png", "Mine/btn_yellow_small.png");
    cacelBut->setScale9Enabled(true);
    cacelBut->setAnchorPoint(Vec2(0, 0));
    cacelBut->setContentSize(Size(300 , 85));
    cacelBut->setPosition(Vec2(50, 40));
    bg->addChild(cacelBut);
    cacelBut->addClickEventListener([=](Ref*ref){
        
        Director::getInstance()->end();
        exit(0);
        
    });
    
    Text *titleCancel = Text::create("退出游戏", ".SFUIDisplay-Semibold", 40);
    titleCancel->setTextColor(Color4B(0x60,0x41,0x10, 255));
    titleCancel->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    titleCancel->setPosition(Vec2(cacelBut->getContentSize().width/2, cacelBut->getContentSize().height/2));
    cacelBut->addChild(titleCancel);
    
    Button *tongBut = Button::create("Mine/btn_yellow_small.png", "Mine/btn_yellow_small_check.png");
    tongBut->setScale9Enabled(true);
    tongBut->setAnchorPoint(Vec2(1, 0));
    tongBut->setContentSize(Size(300 , 85));
    tongBut->setPosition(Vec2(bg->getContentSize().width - 50, 40));
    bg->addChild(tongBut);
    tongBut->addClickEventListener([=](Ref*ref){
        mask->removeFromParent();
    });
    
    Text *titleTong = Text::create("我同意协议", ".SFUIDisplay-Semibold", 40);
    titleTong->setTextColor(Color4B(0x60,0x41,0x10, 255));
    titleTong->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    titleTong->setPosition(Vec2(tongBut->getContentSize().width/2, tongBut->getContentSize().height/2));
    tongBut->addChild(titleTong);
}
void LoadLayer::userAgreementView()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Layout *agreeBg = Layout::create();
    agreeBg->setPosition(Vec2(0, 40));
    agreeBg->setContentSize(Size(visibleSize.width, 185));
    this->addChild(agreeBg);
    
    Button *tongBtn = Button::create("Mine/Load_tongyi_1.png","Mine/Load_tongyi_1.png", "Mine/Load_tongyi_1.png");
    tongBtn->setAnchorPoint(Vec2(0, 1));
    tongBtn->setPosition(Vec2(300, agreeBg->getContentSize().height-8));
    agreeBg->addChild(tongBtn);
    tongBtn->addClickEventListener([=](Ref* sender){
        exitTipView();
    });

    ImageView * selectImage = ImageView::create("Mine/Load_tongyi_2.png");
    selectImage->setPosition(Vec2(tongBtn->getContentSize().width/2, tongBtn->getContentSize().height/2));
    tongBtn->addChild(selectImage);
    
    Text *left = Text::create("我同意《用户服务协议》", "", 45);
    left->setTouchEnabled(true);
    left->setAnchorPoint(Vec2(0, 1));
    left->setTextColor(Color4B(0xb4, 0x9c, 0x71, 255));
    left->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
    left->setPosition(Vec2(370, agreeBg->getContentSize().height - 5));
    agreeBg->addChild(left);
    left->addClickEventListener(CC_CALLBACK_1(LoadLayer::webAgreePost, this));
    
    auto line = Layout::create();
    line->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    line->setBackGroundColor(Color3B(0xb4, 0x9c, 0x71));
    line->setPosition(Vec2(370, agreeBg->getContentSize().height - 60));
    line->setContentSize(Size(475, 4));
    agreeBg->addChild(line);
    
    Text *down1 = Text::create("抵制不良游戏，拒绝盗版游戏。注意自我保护，谨防受骗上当，", "", 35);
    down1->setAnchorPoint(Vec2(0.5, 0));
    down1->setTextColor(Color4B(0x59, 0x59, 0x59, 255));
    down1->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    down1->setPosition(Vec2(visibleSize.width/2, 60));
    agreeBg->addChild(down1);

    Text *down2 = Text::create("适度游戏益脑，沉迷游戏伤身。合理安排时间，享受健康生活。", "", 35);
    down2->setAnchorPoint(Vec2(0.5, 0));
    down2->setTextColor(Color4B(0x59, 0x59, 0x59, 255));
    down2->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    down2->setPosition(Vec2(visibleSize.width/2, 0));
    agreeBg->addChild(down2);
}

void LoadLayer::webAgreePost(Ref*ref)
{
    showAgreementWebView(CCHttpAgent::getInstance()->url + "notice/userNotice");
}

void LoadLayer::showAgreementWebView(string url)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    Layout *bgLayout = Layout::create();
    bgLayout->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    bgLayout->setBackGroundColor(Color3B(0x00, 0x00, 0x00));
    bgLayout->setBackGroundColorOpacity(200);
    bgLayout->setTouchEnabled(true);
    bgLayout->setSwallowTouches(true);
    bgLayout->setContentSize(visibleSize);
    bgLayout->addClickEventListener([=](Ref*ref){
        bgLayout->removeFromParent();
        
    });
    
    this->addChild(bgLayout);
    
    ImageView * bg = ImageView::create("Public/bar_window_yellow.png");
    bg->setAnchorPoint(Vec2(0.5, 0.5));
    bg->setTouchEnabled(true);
    bg->setSwallowTouches(true);
    bg->setScale9Enabled(true);
    bg->setContentSize(Size(visibleSize.width - 100, visibleSize.height - 400));
    bg->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    bgLayout->addChild(bg);
    
    float H = bg->getContentSize().height;
    float bgW = bg->getContentSize().width;
    
    Button *cancel = Button::create("Public/cancel_icon_yellow.png","Public/cancel_icon_yellow_check.png");
    cancel->setPosition(Vec2(bgW, H));
    cancel->addClickEventListener([=](Ref*ref){
        bgLayout->removeFromParent();
    });
    bg->addChild(cancel);
    
    Text *title = Text::create("用户协议", "", 40);
    title->setAnchorPoint(Vec2(0.5, 1));
    title->setTextColor(Color4B(0xf1, 0xcb, 0x5b, 255));
    title->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    title->setPosition(Vec2(bgW/2, H - 35));
    bg->addChild(title);
    
    cocos2d::experimental::ui::WebView * webView = cocos2d::experimental::ui::WebView::create();
    webView->setColor(Color3B(0x1d, 0x1d, 0x1e));
    webView->setAnchorPoint(Vec2(0, 0));
    webView->setPosition(Vec2(10, 10));
    webView->setContentSize(Size(bgW - 20, H - 110));
    
    webView->loadURL(url);
    webView->setScalesPageToFit(true);
    bg->addChild(webView);
    
}

//微信登录
void LoadLayer::loadByWeiXin()
{
    string str = UserDefault::getInstance()->getStringForKey("wx_login_openid");
    if (str!="") {
        __Dictionary *dic = __Dictionary::create();
        dic->setObject(__String::create(str), "wx_login_openid");
        LoadLayer::thirdWXLoginPost(dic);
    } else {
        PlatformHelper::weiXinLoad();
    }
}

void LoadLayer::thirdWXLoginPost(Ref* pSender)
{
    __String *dic = (__String*)pSender;
    string code = dic->getCString();
    auto size = Director::getInstance()->getVisibleSize();
    Loading *loading = Loading::startLoading(Director::getInstance()->getRunningScene(), size / 2);
    loading->maskSwllowTouch();
    
    //微信登录请求
    Json::Value json2;
    json2["platFrom"] = 1;
    json2["thirdType"] = 3;
    json2["loginType"] = 3;
    json2["userFrom"] = 1;
    json2["thirdId"] = "";
    json2["code"] = code;
    
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
        
        CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
        if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)return;
        loading->removeFromParent();
        if (loginPacket->status != 3)
        {
            PlatformHelper::showToast("网络链接失败，请稍后再试");
            return;
        }
        
        if (loginPacket->resultIsOK())
        {
            bool result = loginPacket->recvVal["resultMap"].isNull();
            if(result){
                //需要处理登录返回数据有问题时重新切换域名登录
                CCHttpAgent::getInstance()->packets[tag] = NULL;
                delete loginPacket;
                CCHttpAgent::getInstance()->checkChangeURL(CCHttpAgent::getInstance()->url);
                loadByWeiXin();
                return;
            }
            
            //纪录微信登陆的时间
            long long currentTime = Utils::getCurrentSecend();
            UserDefault::getInstance()->setDoubleForKey("WXThreeDay_ReLoad", currentTime);
            
            Json::Value data = loginPacket->recvVal["resultMap"];
            
            ZJHModel::getInstance()->setUserType(data["userType"].asInt());
            ZJHModel::getInstance()->UserKey = data["userKey"].asString();
            ZJHModel::getInstance()->uid = data["id"].asInt();
            ZJHModel::getInstance()->nick = data["nickName"].asString();
            ZJHModel::getInstance()->portrit = data["thumbImage"].asString();
            ZJHModel::getInstance()->pwdOK = data["pwdOK"].asBool();
            ZJHModel::getInstance()->setPhoneAndPwd(data["phone"].asString(), ZJHModel::getInstance()->pwd);
            ZJHModel::getInstance()->vipLv = data["vipLv"].asInt();
            
            MsgModel::getInstance()->ip = data["im"]["ipAddress"].asString();
            MsgModel::getInstance()->port = data["im"]["port"].asInt();
            
            UserDefault::getInstance()->setIntegerForKey("thirdtype", 3);
            UserDefault::getInstance()->setStringForKey("wx_login_openid", data["thirdId"].asString());
            
            if (ZJHModel::getInstance()->isGameLoad)
            {
                Director::getInstance()->replaceScene(GameLoad::createScene());
                
            }else
            {
                
                __NotificationCenter::getInstance()->postNotification("account_change");
 //             MyNotification::getInstance()->postNotify("account_change");
                this->disappear();
            }
            
            
        }else
        {
            PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
        }
    }, "user/thirdLogin", json2.toStyledString(), "weiXinLoad");
}
