//
//  wanBeiShop.cpp
//  ZJH
//
//  Created by apple on 16/6/30.
//
//

#include "TuiJianRenLayer.hpp"
#include "ShareAppInviteLayer.hpp"
#include "../../MyGUI/ImageByUrl.h"

bool TuiJianRenLayer::init()
{
    if (!Widget::init())
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float H = visibleSize.height - 270;
    
    this->setContentSize(Size(visibleSize.width, H));
    
    loadView();
    
    return true;
}

void TuiJianRenLayer::loadView()
{
    //2:请求
    CCHttpAgent::getInstance()->sendHttpPost(NULL,"invite/isDraw", "" ,"tuijianren");
    this->schedule(schedule_selector(TuiJianRenLayer::postData), 0.0f);
    
    loading = Loading::startLoading(this, Director::getInstance()->getWinSize() / 2);
    loading->maskSwllowTouch();
}

void TuiJianRenLayer::postData(float dt)
{
    CCHttpPacket* packet = CCHttpAgent::getInstance()->packets["tuijianren"];
    if (packet && packet->status != 0)
    {
        loading->removeFromParent();
        unschedule(schedule_selector(TuiJianRenLayer::postData));
        
        if (packet->status != 3)
        {
            return;
        }
        packet->status = 0;
        if (packet->resultIsOK())
        {
            Json::Value data = packet->recvVal["resultMap"];
            showPageView(data);
        }else
        {
            PlatformHelper::showToast(packet->recvVal["message"].asCString());
            unschedule(schedule_selector(TuiJianRenLayer::postData));
        }
        
        CCHttpAgent::getInstance()->packets["tuijianren"] = NULL;
        delete packet;
    }
}

void TuiJianRenLayer::showPageView(Json::Value json)
{
    this->removeAllChildren();
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float H = this->getContentSize().height;
    
    string imageUrl = json["img"].asString();
    ImageByUrl* barBg = ImageByUrl::create();
    barBg->load(visibleSize.width, 385, imageUrl, "Mine/invite_banner.png");
    barBg->image->setTouchEnabled(true);
    barBg->setAnchorPoint(Vec2(0, 1));
    barBg->setPosition(Vec2(0, H));
    this->addChild(barBg);
    
    Layout *textLayout = Layout::create();
    textLayout->setAnchorPoint(Vec2(0, 1));
    textLayout->setPosition(Vec2(0, H - 400));
    textLayout->setContentSize(Size(Vec2(visibleSize.width, 130)));
    this->addChild(textLayout);
    
    string descStr = json["activeDesc"].asString();
    LabelTTF *descText = LabelTTF::create(descStr, "", 36, Size(visibleSize.width - 20, 130), TextHAlignment::LEFT);
    descText->setAnchorPoint(Vec2(0, 0.5));
    descText->setPosition(Vec2(20, textLayout->getContentSize().height/2));
    descText->setColor(Color3B(0xe6, 0xe6, 0xe6));
    textLayout->addChild(descText);
    
    if (json["isDraw"].asInt() == 0)
    {
        isNO = true;
        showNoPage2(json);
        
    }else if(json["isDraw"].asInt() == 1)
    {
        isNO = false;
        showPage2IsDraw(json);
    }
}

void TuiJianRenLayer::showNoPage2(Json::Value json)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float H = this->getContentSize().height;
    
//    ImageView *cardIcon = ImageView::create("Mine/invite_crad_big.png");
//    cardIcon->setAnchorPoint(Vec2(0.5, 1));
//    cardIcon->setPosition(Vec2(visibleSize.width/2, H - 580));
//    this->addChild(cardIcon);
    
    Text *text1 = Text::create("输入推荐人推荐码", ".SFUIDisplay-Semibold", 45);
    text1->setAnchorPoint(Vec2(0.5, 1));
    text1->setPosition(Vec2(visibleSize.width/2, H - 700));
    text1->setTextColor(Color4B(0xfd, 0xe4, 0x5f, 255));
    this->addChild(text1);
    
    textField2 = EditBox::create(Size(960, 120), "Mine/bar_sendtxt.png");
    textField2->setAnchorPoint(Vec2(0.5, 1));
    textField2->setMaxLength(15);
    textField2->setPlaceholderFontSize(40);
    textField2->setInputMode(EditBox::InputMode::NUMERIC);
    textField2->setPosition(Vec2(visibleSize.width/2, H - 800));
    textField2->setFontColor(Color3B(0x66, 0x66, 0x66));
    textField2->setPlaceholderFontColor(Color3B(0xb2, 0xb2, 0xb2));
    textField2->setFontSize(45);
    this->addChild(textField2);
    
    __String *moneyNum = __String::createWithFormat("%d", json["num"].asInt());
    
    string moneyBuf;
    if (json["type"].asInt() == 1)
    {
        moneyBuf = "金币";
    }else if(json["type"].asInt() == 2)
    {
        moneyBuf = "钻石";
    }else if(json["type"].asInt() == 3)
    {
        moneyBuf = "张房卡";
    }
    string content = "你和你的推荐人都能获得" + (string)moneyNum->getCString() + moneyBuf + "奖励";
    
    Text *text2 = Text::create(content, ".SFUIDisplay-Semibold", 36);
    text2->setAnchorPoint(Vec2(0.5, 1));
    text2->setPosition(Vec2(visibleSize.width/2, H - 1000));
    text2->setTextColor(Color4B(0xb3, 0xb3, 0xb3, 255));
    this->addChild(text2);
    
    Button *sureBtn = Button::create("Mine/btn_yellow_big.png","Mine/btn_yellow_big_check.png");
    sureBtn->setScale9Enabled(true);
    sureBtn->setAnchorPoint(Vec2(0.5, 1));
    sureBtn->setContentSize(Size(960, 130));
    sureBtn->setPosition(Vec2(visibleSize.width/2, H - 1086));
    this->addChild(sureBtn);
    sureBtn->addClickEventListener(CC_CALLBACK_1(TuiJianRenLayer::sureOn, this));
    
    Text *sureTitle = Text::create("确认", ".SFUIDisplay-Semibold", 40);
    sureTitle->setPosition(Vec2(sureBtn->getContentSize().width/2, sureBtn->getContentSize().height/2));
    sureTitle->setTextColor(Color4B(0x65, 0x40, 0x02, 255));
    sureBtn->addChild(sureTitle);
}

void TuiJianRenLayer::showPage2IsDraw(Json::Value json)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float H = this->getContentSize().height;
    
    //    ImageView *cardIcon = ImageView::create("Mine/invite_crad_big.png");
    //    cardIcon->setAnchorPoint(Vec2(0.5, 1));
    //    cardIcon->setPosition(Vec2(visibleSize.width/2, H - 120));
    //    this->addChild(cardIcon);
    
    __String *bufStr = __String::createWithFormat("%d", json["num"].asInt());
    string moneyBuf;
    if (json["type"].asInt() == 1)
    {
        moneyBuf = "金币";
    }else if(json["type"].asInt() == 2)
    {
        moneyBuf = "钻石";
    }else if(json["type"].asInt() == 3)
    {
        moneyBuf = "张房卡";
    }
    string content = "已获得" + (string)bufStr->getCString() + moneyBuf + "奖励";
    
    Text *text1 = Text::create(content, ".SFUIDisplay-Semibold", 60);
    text1->setAnchorPoint(Vec2(0.5, 1));
    text1->setTextHorizontalAlignment(TextHAlignment::CENTER);
    text1->setPosition(Vec2(visibleSize.width/2, H - 800));
    text1->setTextColor(Color4B(0xf1, 0xcb, 0x5b, 255));
    this->addChild(text1);
    
    RichText *title1 = RichText::create();
    title1->ignoreContentAdaptWithSize(true);
    title1->setContentSize(Size(visibleSize.width/2, 100));
    RichElementText* re1 = RichElementText::create(1, Color3B(0xb3, 0xb3, 0xb3), 255, "你的推荐人：", "", 50);
    RichElementText* re2 = RichElementText::create(2, Color3B(0xf1, 0xcb, 0x5b), 255, json["inviteUserName"].asString(), "", 50);
    title1->pushBackElement(re1);
    title1->pushBackElement(re2);
    title1->setPosition(Vec2(visibleSize.width/2, H - 900));
    this->addChild(title1);
}

void TuiJianRenLayer::sureOn(Ref *pSender)
{
    Json::Value json;
    json["inviteCode"] = textField2->getText();
    
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
            Json::Value data = loginPacket->recvVal["resultMap"];
            loadView();
            isNO = false;
            
            PlatformHelper::showToast("领取奖励成功！");
            
            __NotificationCenter::getInstance()->postNotification("inviteSuccess");
            __NotificationCenter::getInstance()->postNotification("money_change");
            
        }else
        {
            PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
        }
    },"invite/inviteCode", json.toStyledString(),"inviteCode");
}

void TuiJianRenLayer::inviteCodeInto(string code)
{
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
            Json::Value data = loginPacket->recvVal["resultMap"];
            if (data["isDraw"].asInt() == 0)
            {
                //未领取
                Json::Value json;
                json["inviteCode"] = code;
                
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
                        Json::Value data = loginPacket->recvVal["resultMap"];
                        loadView();
                        isNO = false;
                        
                        PlatformHelper::showToast("领取奖励成功！");
                        
                        __NotificationCenter::getInstance()->postNotification("inviteSuccess");
                        __NotificationCenter::getInstance()->postNotification("money_change");
                        
                    }else
                    {
                        PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
                    }
                },"invite/inviteCode", json.toStyledString(),"webInviteCode");
                
                
            }else if(data["isDraw"].asInt() == 1)
            {
                //领取过的
                PlatformHelper::showToast("你已经领取过奖励了");
            }
        }else
        {
            PlatformHelper::showToast(loginPacket->recvVal["message"].asCString());
        }
    },"invite/isDraw", "","isDrawWeb");

}
