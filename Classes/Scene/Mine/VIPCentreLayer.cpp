//
//  VIPCentreLayer.cpp
//  ZJH
//
//  Created by apple on 16/7/1.
//
//

#include "VIPCentreLayer.hpp"
#include "VIPBuyTipLayer.hpp"

void VIPCentreLayer::postVIPInfoRequest(Json::Value _json)
{
    infoList = _json;
    //1、获取VIP信息
    Json::Value json;
    json["userId"] = infoList["id"];
    
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
        CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
        
        if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)return;
        
        if (loginPacket->status != 3)
        {
            PlatformHelper::showToast("网络链接失败，请稍后再试");
            return;
        }
        
        if (loginPacket->resultIsOK())
        {
            Json::Value data = loginPacket->recvVal["resultMap"]["vipInfo"];
            vipInfoList = data;
            
            log("VIP信息中心：＝＝＝ [%s]",data.toStyledString().c_str());
            showView(data);
        }
    },"vip/getVipInfo",json.toStyledString(),"VipInfo");
}

bool VIPCentreLayer::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    setTitle("会员中心");
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", [=](){
        this->disappear();
    });
    setBackground(Color4B(0x14, 0x14, 0x15, 255));
    
    return true;
}

void VIPCentreLayer::showView(Json::Value data)
{
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    float H = visibleSize.height - getHeadH();

    auto list1 = Layout::create();
    list1->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    list1->setBackGroundColor(Color3B(0x1d, 0x1d, 0x1e));
    list1->ignoreAnchorPointForPosition(false);
    list1->setAnchorPoint(Vec2(0, 1));
    list1->setPosition(Vec2(0,  H));
    list1->setContentSize(Size(visibleSize.width, 570));
    this->addChild(list1);
    
    //头像
    ImageView *portrit = ImageView::create();
    portrit->setContentSize(Size(320, 320));
    portrit->cocos2d::ui::Widget::ignoreContentAdaptWithSize(false);
    portrit->setAnchorPoint(Vec2(0.5, 1));
    portrit->setPosition(Vec2(visibleSize.width/2 , 570-25));
    list1->addChild(portrit);
    string vipName;
    if (ZJHModel::getInstance()->vipLv == 1)
    {
        portrit->loadTexture("Mine/vip@2x.png");
        vipName = "VIP";
    }else if(ZJHModel::getInstance()->vipLv == 2)
    {
        portrit->loadTexture("Mine/guiZu@2x.png");
        vipName = "贵族";
    }else if (ZJHModel::getInstance()->vipLv == 3)
    {
        portrit->loadTexture("Mine/ZuZhang@2x.png");
        vipName = "族长";
    }
    Text *name = Text::create(vipName, ".SFUIDisplay-Semibold", 50);
    name->setAnchorPoint(Vec2(0.5, 1));
    name->setPosition(Vec2(visibleSize.width/2,  570 - 370));
    name->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    name->setTextColor(Color4B(0xe6, 0xc7, 0x94, 255));
    list1->addChild(name);
    
    //VIP状态描述
    Text *vipText = Text::create("", ".SFUIDisplay-Semibold", 50);
     vipText->setAnchorPoint(Vec2(0.5, 1));
    vipText->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    vipText->setPosition(Vec2(name->getPositionX(),  name->getPositionY() - 65));
    vipText->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    vipText->setTextColor(Color4B(255, 255, 255, 255));
    list1->addChild(vipText);
    
    
    //剩余时间的计算
    struct timeval nowTimeval;
    gettimeofday(&nowTimeval, NULL);
    time_t time_sec ;
    time_sec = nowTimeval.tv_sec;
    time_t time_sec1 = data["endTime"].asDouble()/1000;
    long long intervalDay = (time_sec1 - time_sec)/(24*60*60);
    char interval[100];
    struct tm * test = localtime(&time_sec1);
    int year = test->tm_year + 1900;
    int mon = test->tm_mon+1;
    int day = test->tm_mday;
    printf("%d %d %d", year, mon, day);
    
    if (intervalDay < 0)
    {
        intervalDay = 0;
    }
    sprintf(interval, "剩余%lld天", intervalDay);
    vipText->setString(interval);
    
    sprintf(interval, "截至%d年%d月%d日", year, mon, day);
    
    Text *vipEnd = Text::create(interval, ".SFUIDisplay-Semibold", 50);
    vipEnd->setAnchorPoint(Vec2(0.5, 1));
    vipEnd->setPosition(Vec2(visibleSize.width/2 ,  vipText->getPositionY() - 65));
    vipEnd->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    vipEnd->setTextColor(Color4B(255, 255, 255, 255));
    list1->addChild(vipEnd);
    
    for (int i = 0 ; i < 2; i++)
    {
        Button *buyBtn = Button::create("Mine/btn_yellow_small.png","Mine/btn_yellow_small_check.png");
        buyBtn->setScale9Enabled(true);
        buyBtn->setContentSize(Size(485, 122));
        this->addChild(buyBtn);
        buyBtn->addTouchEventListener(CC_CALLBACK_2(VIPCentreLayer::buyVipFun, this));
        
        Text *title = Text::create("", ".SFUIDisplay-Semibold", 40);
        title->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
        title->setPosition(Vec2(buyBtn->getContentSize().width/2 ,  buyBtn->getContentSize().height/2));
        title->setTextColor(Color4B(0x66, 0x41, 0x06, 255));
        buyBtn->addChild(title);
        
        switch (i) {
            case 0:
                buyBtn->setPosition(Vec2(280, H - 650));
                title->setString("加购");
                break;
            case 1:
                buyBtn->setPosition(Vec2(visibleSize.width -280, H - 650));
                title->setString("升级");
                break;
        }
    }
    
    
    for (int i = 0; i < 2; i++)
    {
        auto list2 = Layout::create();
        list2->setTouchEnabled(true);
        list2->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
        list2->setBackGroundColor(Color3B(0x1d, 0x1d, 0x1e));
        list2->ignoreAnchorPointForPosition(false);
        list2->setAnchorPoint(Vec2(0, 1));
        list2->setContentSize(Size(visibleSize.width, 130));
        this->addChild(list2);
        
        ImageView *left = ImageView::create("Public/detail_arrow.png");
        left->setAnchorPoint(Vec2(1, 0.5));
        left->setPosition(Vec2(visibleSize.width - 45, 65));
        list2->addChild(left);
        
        ImageView *icon;
        Text *rightT;
        
        Text *text = Text::create("", ".SFUIDisplay-Semibold", 40);
        text->setTextColor(Color4B(0x67, 0x67, 0x67, 255));
        switch (i)
        {
            case 0:
                
                list2->setPosition(Vec2(0, H - 735));
        
                icon = ImageView::create("Mine/view_icon16.png");
                text->setString("会员特权");
                list2->addTouchEventListener(CC_CALLBACK_2(VIPCentreLayer::teQaunFun, this));
                break;
            case 1:
                
                left->setVisible(false);
                
                list2->setPosition(Vec2(0, H - 885));
                
                icon = ImageView::create("Mine/view_icon7.png");
                text->setString("保险箱");
                
                rightT = Text::create("敬请期待", ".SFUIDisplay-Semibold", 40);
                rightT->setTextColor(Color4B(0x67, 0x67, 0x67, 255));
                rightT->setAnchorPoint(Vec2(1, 0.5));
                rightT->setPosition(Vec2(visibleSize.width - 90, 65));
                list2->addChild(rightT);
                
                break;
            default:
                break;
        }
        icon->setAnchorPoint(Vec2(0, 0.5));
        icon->setPosition(Vec2(55, list2->getContentSize().height/2));
        list2->addChild(icon);
        
        text->setAnchorPoint(Vec2(0, 0.5));
        text->setPosition(Vec2(150, icon->getPositionY()));
        list2->addChild(text);
    }
}

void VIPCentreLayer::teQaunFun(Ref *pSender, Widget::TouchEventType type)
{
    if (Widget::TouchEventType::ENDED == type)
    {
       ZJHModel::getInstance()->gotoView(VIEW_TE_QUAN);
    }
}

void VIPCentreLayer::buyVipFun(Ref *pSender, Widget::TouchEventType type)
{
    if (Widget::TouchEventType::ENDED == type)
    {
        Size winSize = Director::getInstance()->getWinSize();
        VIPBuyTipLayer *tip = VIPBuyTipLayer::create();
        tip->setPosition(Vec2(winSize.width, 0));
        this->addChild(tip, 100);
        
        tip->runAction(MoveTo::create(0.3, Vec2(0,0)));
    }
}
