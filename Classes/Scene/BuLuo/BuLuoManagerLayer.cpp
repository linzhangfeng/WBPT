//
//  FriendLayer.cpp
//  wanMain
//
//  Created by apple on 16/6/23.
//
//

#include "BuLuoManagerLayer.h"

bool BuLuoManagerLayer::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", [=](){
        this->disappear();
    });
    setTitle("俱乐部管理");
    
    setBackground(Color4B(0x1d, 0x1d, 0x1e, 255));
    
    CCHttpAgent::getInstance()->sendHttpPost(NULL,"tribe/info", "" ,"buluo");
    this->schedule(schedule_selector(BuLuoManagerLayer::postData), 0.0f);
    
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(BuLuoManagerLayer::updateName), "buluo_name_change", NULL);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(BuLuoManagerLayer::updatePortrit), "buluo_potrite_change", NULL);
    
    
    return true;
}

void BuLuoManagerLayer::postData(float dt)
{
    CCHttpPacket* packet = CCHttpAgent::getInstance()->packets["buluo"];
    if (packet && packet->status != 0)
    {
        if (packet->status != 3)
        {
            return;
        }
        packet->status = 0;
        
        if (packet->resultIsOK())
        {
            Json::Value data = packet->recvVal["resultMap"]["tribe"];
            showInfoView(data);
            
            jsonData = data;
            
            log("俱乐部：＝＝＝ [%s]",data.toStyledString().c_str());
            
        }else
        {
            PlatformHelper::showToast(packet->recvVal["message"].asCString());
            
        }
        
        unschedule(schedule_selector(BuLuoManagerLayer::postData));
        CCHttpAgent::getInstance()->packets["buluo"] = NULL;
        delete packet;
    }
}

void BuLuoManagerLayer::updatePortrit(Ref*ref)
{
    __String *str = (__String*)ref;
    string tempStr = str->getCString();
    
    userPortrit->removeFromParent();
    
    float bgH = infoBg->getContentSize().height;
    userPortrit = Utils::createCircleAvatar(tempStr, "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(160,160));
    userPortrit->setPosition(Vec2(185, bgH/2));
    infoBg->addChild(userPortrit);
    
    jsonData["logo"] = tempStr;
}

void BuLuoManagerLayer::updateName(Ref*ref)
{
    __String *str = (__String*)ref;
    string tempStr = str->getCString();
    char buf[128];
    
    Utils::parseName(20, buf, tempStr.c_str());
    nameText->setString(buf);
    jsonData["name"] = tempStr;
    
}

void BuLuoManagerLayer::showInfoView(Json::Value json)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float H = visibleSize.height - getHeadH();
    
    infoBg = ImageView::create("Mine/my_buluo_BG.png");
    infoBg->setContentSize(Size(visibleSize.width, 335));
    infoBg->ignoreContentAdaptWithSize(false);
    infoBg->setAnchorPoint(Vec2(0, 1));
    infoBg->setPosition(Vec2(0, H));
    this->addChild(infoBg);
    infoBg->setTouchEnabled(true);
    infoBg->addClickEventListener([=](Ref*ref){
       
        ZJHModel::getInstance()->gotoView(VIEW_ZUZHANG_BULUO_INFO, jsonData);
        
    });
    
    float bgH = infoBg->getContentSize().height;
    
    ImageView *arrow = ImageView::create("Mine/me_comeback.png");
    arrow->ignoreContentAdaptWithSize(false);
    arrow->setAnchorPoint(Vec2(1, 0.5));
    arrow->setPosition(Vec2(visibleSize.width - 65 , bgH/2));
    infoBg->addChild(arrow);
    
    //头像
    userPortrit = Utils::createCircleAvatar(json["logo"].asString() , "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(160,160));
    userPortrit->setPosition(Vec2(185, bgH/2));
    infoBg->addChild(userPortrit);

    //name
    nameText = Text::create(json["name"].asString(), "", 44);
    nameText->setAnchorPoint(Vec2(0, 1));
    nameText->setPosition(Vec2(325, bgH - 95));
    nameText->setTextColor(Color4B(0xe1, 0xcd, 0xad, 255));
    infoBg->addChild(nameText);
    char buf[128];
    sprintf(buf, "状态:有效（族长会员剩余%d天）", json["leftDays"].asInt());
    
    Text *states = Text::create(buf, "", 44);
    states->setAnchorPoint(Vec2(0, 1));
    states->setPosition(Vec2(325, bgH - 160));
    states->setTextColor(Color4B(0xb4, 0xb4, 0xb5, 255));
    infoBg->addChild(states);
    
    showBuLuoGongNeng();

}

void BuLuoManagerLayer::showBuLuoGongNeng()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float H = visibleSize.height - getHeadH() - 340;
    
    Vec2 btnPos[3] = {
        Vec2(130 + 80, H - 140),
        Vec2(visibleSize.width / 2, H - 140),
        Vec2(visibleSize.width - 130 - 80, H - 140),
    };
    
    for (int i = 0; i < 3; i++)
    {
        Button *btn;
        switch (i)
        {
            case 0:
                btn = Button::create("Mine/buluo_tuiguangyaoqingma.png", "Mine/buluo_tuiguangyaoqingma.png", "Mine/buluo_tuiguangyaoqingma.png");
                btn->setName("tuiGuang");
                break;
            case 1:
                btn = Button::create("Mine/buluo_guanli.png", "Mine/buluo_guanli.png", "Mine/buluo_guanli.png");
                btn->setName("chengyuan");
                break;
            case 2:
                btn = Button::create("Mine/buluo_group.png", "Mine/buluo_group.png", "Mine/buluo_group.png");
                btn->setName("group");
                break;
        }
        btn->setPosition(btnPos[i%3]);
        this->addChild(btn);
        btn->addClickEventListener(CC_CALLBACK_1(BuLuoManagerLayer::clickBtnGoTo, this));
    }
}

void BuLuoManagerLayer::clickBtnGoTo(Ref*ref)
{
    Button *btn = (Button *)ref;
    
    if (btn->getName() == "tuiGuang")
    {
        ZJHModel::getInstance()->gotoView(VIEW_BULUO_TUI_GUANG, jsonData);
        
    }else if (btn->getName() == "chengyuan")
    {
        PlatformHelper::showToast("敬请期待");
        
    }else if (btn->getName() == "group")
    {
        PlatformHelper::showToast("敬请期待");
    }

}
