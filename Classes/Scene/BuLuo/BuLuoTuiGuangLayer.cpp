//
//  FriendLayer.cpp
//  wanMain
//
//  Created by apple on 16/6/23.
//
//

#include "BuLuoTuiGuangLayer.h"
#include "../Mine/ShareAppInviteLayer.hpp"

BuLuoTuiGuangLayer::BuLuoTuiGuangLayer()
{
    
}

bool BuLuoTuiGuangLayer::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", [=](){
        this->disappear();
    });
    setTitle("俱乐部推广");
    
    setBackground(Color4B(0x1d, 0x1d, 0x1e, 255));
    
    return true;
}

void BuLuoTuiGuangLayer::showInfoView(Json::Value json)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float H = visibleSize.height - getHeadH();
    
    //头像
    Sprite *userPortrit = Utils::createCircleAvatar(json["logo"].asString() , "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(260,260));
    userPortrit->setPosition(Vec2(visibleSize.width/2 , H - 290));
    this->addChild(userPortrit);

    //name
    Text *name = Text::create(json["name"].asString(), "", 44);
    name->setAnchorPoint(Vec2(0.5, 1));
    name->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    name->setPosition(Vec2(visibleSize.width/2, H - 430));
    name->setTextColor(Color4B(0xe1, 0xcd, 0xad, 255));
    this->addChild(name);
    
    char buf[128];
    sprintf(buf, "俱乐部族长：%s", json["header"]["nickName"].asString().c_str());
    
    Text *nick = Text::create(buf, "", 44);
    nick->setAnchorPoint(Vec2(0.5, 1));
    //nick->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
    nick->setPosition(Vec2(visibleSize.width/2, H - 560));
    nick->setTextColor(Color4B(0x9a, 0x9a, 0x9a, 255));
    this->addChild(nick);
    
    Text *title1 = Text::create("邀请码：", "", 44);
    title1->setAnchorPoint(Vec2(0, 1));
    title1->setPosition(Vec2(190, H - 650));
    title1->setTextColor(Color4B(0x9a, 0x9a, 0x9a, 255));
    this->addChild(title1);
    
    auto codeBg = Layout::create();
    codeBg->ignoreAnchorPointForPosition(false);
    codeBg->setAnchorPoint(Vec2(0.5, 1));
    codeBg->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    codeBg->setBackGroundColor(Color3B(0x26, 0x26, 0x29));
    codeBg->setPosition(Vec2(visibleSize.width/2, H - 650));
    codeBg->setContentSize(Size(270, 80));
    this->addChild(codeBg);
    
    codeText = Text::create(json["inviteCode"].asString() , "", 44);
    codeText->setPosition(Vec2(codeBg->getContentSize().width/2, codeBg->getContentSize().height/2));
    codeText->setTextColor(Color4B(255, 255, 255, 255));
    codeBg->addChild(codeText);
    
    auto desBg = Layout::create();
    desBg->ignoreAnchorPointForPosition(false);
    desBg->setAnchorPoint(Vec2(0.5, 0));
    desBg->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    desBg->setBackGroundColor(Color3B(0xff, 0xff, 0xff));
    desBg->setPosition(Vec2(visibleSize.width/2, 600));
    desBg->setContentSize(Size(800, 240));
    this->addChild(desBg);
    
    auto text3 = LabelTTF::create("邀请你加入我的玩呗俱乐部，在玩呗娱乐社区［俱乐部］栏目输入以上邀请码可加入我的俱乐部", "", 40,
                                  Size(720, 185), TextHAlignment::LEFT);
    text3->setAnchorPoint(Vec2(0, 1));
    text3->setColor(Color3B(0x99, 0x99, 0x99));
    text3->setPosition(Vec2(30, desBg->getContentSize().height - 30));
    desBg->addChild(text3);
    
    Layout *shareBg = Layout::create();
    shareBg->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    shareBg->setBackGroundColor(Color3B(0x1d, 0x1d, 0x1e));
    shareBg->setContentSize(Size(visibleSize.width, 210));
    this->addChild(shareBg);
    
    auto line = Layout::create();
    line->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    line->ignoreAnchorPointForPosition(false);
    line->setAnchorPoint(Vec2(0, 1));
    line->setBackGroundColor(Color3B(0, 0, 0));
    line->setPosition(Vec2(0, 210));
    line->setContentSize(Size(visibleSize.width, 3));
    shareBg->addChild(line);
    
    Button *shareBtn = Button::create("Public/btn_yellow.png","Public/btn_yellow.png", "Public/btn_yellow.png");
    shareBtn->setScale9Enabled(true);
    shareBtn->setContentSize(Size(960, 115));
    shareBtn->setPosition(Vec2(visibleSize.width/2, 105));
    shareBg->addChild(shareBtn);
    shareBtn->setTitleText("分享推广邀请码");
    shareBtn->setTitleFontSize(40);
    shareBtn->setTitleColor(Color3B(0x60, 0x41, 0x0a));
    shareBtn->setTitleFontName("");
    shareBtn->addClickEventListener([=](Ref*ref){
        
        ShareAppInviteLayer *share = ShareAppInviteLayer::create();
        share->setPosition(Vec2::ZERO);
        share->setShareTypeView(3, json);
        this->addChild(share, 100);
    });
}