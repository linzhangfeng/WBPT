//
//  FriendLayer.cpp
//  wanMain
//
//  Created by apple on 16/6/23.
//
//

#include "MyDaiLiLayer.h"

MyDaiLiLayer::MyDaiLiLayer()
{
    
}

bool MyDaiLiLayer::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", [=](){
        this->disappear();
    });
    setTitle("我的代理");

    setBackground(Color4B(0x1d, 0x1d, 0x1e, 255));
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    BG = ImageView::create("Mine/me_daili_bg.png");
    BG->setContentSize(visibleSize);
    BG->ignoreContentAdaptWithSize(false);
    BG->setAnchorPoint(Vec2(0, 0));
    BG->setPosition(Vec2(0, 0));
    this->addChild(BG);
    
    CCHttpAgent::getInstance()->sendHttpPost(NULL,"invite/isQuotaDraw", "" ,"MyDaiLi");
    this->schedule(schedule_selector(MyDaiLiLayer::postData), 0.0f);
    
    loading = Loading::startLoading(this, Director::getInstance()->getWinSize() / 2);
    loading->maskSwllowTouch();
    
    return true;
}

void MyDaiLiLayer::postData(float dt)
{
    CCHttpPacket* packet = CCHttpAgent::getInstance()->packets["MyDaiLi"];
    if (packet && packet->status != 0)
    {
        loading->removeFromParent();
        unschedule(schedule_selector(MyDaiLiLayer::postData));
        
        if (packet->status != 3)
        {
            return;
        }
        packet->status = 0;
        
        if (packet->resultIsOK())
        {
            int isDraw = packet->recvVal["resultMap"]["isDraw"].asInt();
            if(isDraw == 0)
            {
                //没有加入代理
                noJoinDaiLiView();
            }else
            {
                //有加入代理
                haveJoinView(packet->recvVal["resultMap"]);
            }
        
        }else
        {
            PlatformHelper::showToast(packet->recvVal["message"].asCString());
        }
        
        unschedule(schedule_selector(MyDaiLiLayer::postData));
        CCHttpAgent::getInstance()->packets["MyDaiLi"] = NULL;
        
        delete packet;
    }
}

void MyDaiLiLayer::noJoinDaiLiView()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float H = visibleSize.height - getHeadH();

    ImageView*icon = ImageView::create("Mine/me_daili_icon1.png");
    icon->setPosition(Vec2(visibleSize.width/2, H - 300));
    BG->addChild(icon);
    
    Text *title1 = Text::create("请输入代理发你的邀请码", "", 45);
    title1->setPosition(Vec2(visibleSize.width/2, H - 680));
    title1->setTextColor(Color4B(0xdf, 0xce, 0xad, 255));
    BG->addChild(title1);
    
    textField2 = EditBox::create(Size(960, 120), "Mine/bar_sendtxt.png");
    textField2->setMaxLength(15);
    textField2->setPlaceHolder("请输入收到的邀请码");
    textField2->setFontSize(45);
    textField2->setPlaceholderFontSize(40);
    textField2->setInputMode(EditBox::InputMode::NUMERIC);
    textField2->setPosition(Vec2(visibleSize.width/2, H/2+70));
    textField2->setFontColor(Color3B(0x66, 0x66, 0x66));
    textField2->setPlaceholderFontColor(Color3B(0xb2, 0xb2, 0xb2));
    BG->addChild(textField2);
    
    Button *sureBtn = Button::create("Mine/btn_yellow_big.png","Mine/btn_yellow_big_check.png");
    sureBtn->setScale9Enabled(true);
    sureBtn->setContentSize(Size(960, 130));
    sureBtn->setPosition(Vec2(visibleSize.width/2, H/2 - 130));
    BG->addChild(sureBtn);
    sureBtn->addClickEventListener(CC_CALLBACK_1(MyDaiLiLayer::sureJoinTipOn, this));
    
    Text *sureTitle = Text::create("确认", "", 40);
    sureTitle->setPosition(Vec2(sureBtn->getContentSize().width/2, sureBtn->getContentSize().height/2));
    sureTitle->setTextColor(Color4B(0x65, 0x40, 0x02, 255));
    sureBtn->addChild(sureTitle);
    
}

void MyDaiLiLayer::sureJoinOn(Ref*ref)
{
    Json::Value json1;
    json1["inviteCode"] = textField2->getText();
    
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
        CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
        
        if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)return;
        
        if (loginPacket->status != 3)
        {
            return;
        }
        
        if (loginPacket->resultIsOK())
        {
            BG->removeAllChildren();
            Json::Value json = loginPacket->recvVal["resultMap"];
            haveJoinView(json);
            
        }else
        {
            PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
        }
    },"invite/quotaInviteCode",json1.toStyledString(),"JoinDaiLi");

}

void MyDaiLiLayer::sureJoinTipOn(Ref*ref)
{
//    Json::Value json1;
//    sureJoinTipView(json1);

    Json::Value json1;
    json1["inviteCode"] = textField2->getText();
    
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
        CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
        
        if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)return;
        
        if (loginPacket->status != 3)
        {
            return;
        }
        
        if (loginPacket->resultIsOK())
        {
            sureJoinTipView(loginPacket->recvVal["resultMap"]);
            
        }else
        {
            PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
        }
    },"invite/getQuotaByCode",json1.toStyledString(),"Join2");
 
 
}

void MyDaiLiLayer::sureJoinTipView(Json::Value json)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    Layout *mask = Layout::create();
    mask->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    mask->setBackGroundColor(Color3B(0x00, 0x00, 0x00));
    mask->setBackGroundColorOpacity(150);
    mask->setContentSize(visibleSize);
    Director::getInstance()->getRunningScene()->addChild(mask, 1000);
    
    ImageView * bg = ImageView::create("Public/bar_window_yellow.png");
    bg->setTouchEnabled(true);
    bg->setSwallowTouches(true);
    bg->setScale9Enabled(true);
    bg->setContentSize(Size(720, 470));
    bg->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    mask->addChild(bg);
    
    //name
    Text *title = Text::create("确认绑定", "", 45);
    title->setAnchorPoint(Vec2(0.5, 1));
    title->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height - 40));
    title->setTextColor(Color4B(0xf0, 0xcb, 0x5c, 255));
    bg->addChild(title);
    
    RichText *nameTip = RichText::create();
    nameTip->setAnchorPoint(Vec2(0.5, 0));
    nameTip->setPosition(Vec2(bg->getContentSize().width/2 , 250));
    nameTip->ignoreContentAdaptWithSize(false);
    nameTip->setContentSize(Size(bg->getContentSize().width - 60, bg->getContentSize().height - 150));
    
    RichElementText* re1 = RichElementText::create(1, Color3B(0xd9, 0xd9, 0xd9), 255, "即将绑定的代理：", "", 40);
    RichElementText* re2 = RichElementText::create(2, Color3B(0xf0, 0xcb, 0x5c), 255, json["quotaName"].asString() , "", 40);
    
    nameTip->pushBackElement(re1);
    nameTip->pushBackElement(re2);
    bg->addChild(nameTip);
    
    Button *cacelBut = Button::create("Mine/btn_yellow_small.png", "Mine/btn_yellow_small_check.png");
    cacelBut->setScale9Enabled(true);
    cacelBut->setAnchorPoint(Vec2(0, 0));
    cacelBut->setContentSize(Size(300 , 85));
    cacelBut->setPosition(Vec2(40, 40));
    bg->addChild(cacelBut);
    cacelBut->addClickEventListener([=](Ref*ref){
        mask->removeFromParentAndCleanup(true);
    });
    
    Text *titleCancel = Text::create("取消", ".SFUIDisplay-Semibold", 40);
    titleCancel->setTextColor(Color4B(0x60,0x41,0x10, 255));
    titleCancel->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    titleCancel->setPosition(Vec2(cacelBut->getContentSize().width/2, cacelBut->getContentSize().height/2));
    cacelBut->addChild(titleCancel);
    
    Button *sureBut = Button::create("Mine/btn_yellow_small.png", "Mine/btn_yellow_small_check.png");
    sureBut->setScale9Enabled(true);
    sureBut->setAnchorPoint(Vec2(1, 0));
    sureBut->setContentSize(Size(300 , 85));
    sureBut->setPosition(Vec2(bg->getContentSize().width - 40, 40));
    bg->addChild(sureBut);
    sureBut->addClickEventListener([=](Ref*ref){
        mask->removeFromParentAndCleanup(true);
        sureJoinOn(NULL);
    });
    
    Text*titleSure = Text::create("确认绑定", ".SFUIDisplay-Semibold", 40);
    titleSure->setTextColor(Color4B(0x60,0x41,0x10, 255));
    titleSure->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    titleSure->setPosition(Vec2(sureBut->getContentSize().width/2, sureBut->getContentSize().height/2));
    sureBut->addChild(titleSure);

}

void MyDaiLiLayer::haveJoinView(Json::Value json)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float H = visibleSize.height - getHeadH();
    
    //name
    Text *Title = Text::create("你绑定的代理是", "", 45);
    Title->setAnchorPoint(Vec2(0.5, 1));
    Title->setPosition(Vec2(visibleSize.width/2, H - 150));
    Title->setTextColor(Color4B(0xdf, 0xce, 0xad, 255));
    BG->addChild(Title);
    
    //头像
    Sprite *userPortrit = Utils::createCircleAvatar(json["image"].asString() , "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(280, 280));
    userPortrit->setPosition(Vec2(visibleSize.width/2, H - 400));
    BG->addChild(userPortrit, 20);

    //name
    Text *name = Text::create(json["inviteUserName"].asString(), "", 45);
    name->setAnchorPoint(Vec2(0.5, 1));
    name->setPosition(Vec2(visibleSize.width/2, H - 600));
    name->setTextColor(Color4B(0xdf, 0xce, 0xad, 255));
    BG->addChild(name);
    
    Button *daiBtn = Button::create("Mine/btn_yellow_big.png","Mine/btn_yellow_big_check.png");
    daiBtn->setScale9Enabled(true);
    daiBtn->setContentSize(Size(960, 130));
    daiBtn->setPosition(Vec2(visibleSize.width/2, H/2));
    BG->addChild(daiBtn);
    daiBtn->addClickEventListener(CC_CALLBACK_1(MyDaiLiLayer::daiLiOn, this));
    
    Text *daiTitle = Text::create("我也要成为代理", "", 40);
    daiTitle->setPosition(Vec2(daiBtn->getContentSize().width/2, daiBtn->getContentSize().height/2));
    daiTitle->setTextColor(Color4B(0x65, 0x40, 0x02, 255));
    daiBtn->addChild(daiTitle);
}

void MyDaiLiLayer::daiLiOn(Ref*ref)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    Layout *daiLi = Layout::create();
    daiLi->setTouchEnabled(true);
    daiLi->setSwallowTouches(true);
    daiLi->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    daiLi->setBackGroundColor(Color3B(0x00, 0x00, 0x00));
    daiLi->setBackGroundColorOpacity(150);
    daiLi->setContentSize(visibleSize);
    daiLi->addClickEventListener([=](Ref*ref){
        
        daiLi->removeFromParentAndCleanup(true);
        
    });
    this->addChild(daiLi, 100);
    
    ImageView * bg = ImageView::create("Public/bar_window_yellow.png");
    bg->setTouchEnabled(true);
    bg->setSwallowTouches(true);
    bg->setScale9Enabled(true);
    bg->setContentSize(Size(visibleSize.width - 250, 400));
    bg->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    daiLi->addChild(bg);
    
    Text *title = Text::create("提示", "", 50);
    title->setTextColor(Color4B(0xd9, 0xd9, 0xd9, 255));
    title->setAnchorPoint(Vec2(0.5, 0.5));
    title->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    title->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height - 65));
    bg->addChild(title);
    
    Text *title2 = Text::create("请联系你的代理开通", "", 45);
    title2->setTextColor(Color4B(0xd9, 0xd9, 0xd9, 255));
    title2->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    title2->setPosition(Vec2(bg->getContentSize().width/2, bg->getContentSize().height/2));
    bg->addChild(title2);
    
    Button *cancel = Button::create("Public/cancel_icon_yellow.png","Public/cancel_icon_yellow_check.png");
    cancel->setPosition(Vec2(bg->getContentSize().width-15, bg->getContentSize().height-15));
    cancel->addClickEventListener([=](Ref*ref){
        
        daiLi->removeFromParentAndCleanup(true);
        
    });
    bg->addChild(cancel);

}
