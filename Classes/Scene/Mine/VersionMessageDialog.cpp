//
//  VersionMessageDialog.cpp
//  ZJH
//
//  Created by Jack on 16/9/26.
//
//
#include "VersionMessageDialog.hpp"

bool VersionMessageDialog::init(){
    if (!Layer::init()) {
        return false;
    }
    Size size = Director::getInstance()->getWinSize();
    ignoreAnchorPointForPosition(false);
    setContentSize(Size(986,737));
    setAnchorPoint(Vec2(0.5,0.5));
    setPosition(Vec2(size.width/2,size.height/2));
    
    LayerColor* topBg = LayerColor::create(Color4B(0xf8,0xf8,0xf8, 255));
    topBg->setContentSize(Size(986,737));
    topBg->setAnchorPoint(Vec2(0,0));
    topBg->setPosition(Vec2(0,0));
    this->addChild(topBg);

    
    string versionTitle = "版本信息";
    Text* textGameName = Text::create(versionTitle, "AmericanTypewriter", 58);
    textGameName->setColor(Color3B(0x82,0x82, 0x82));
    textGameName->setAnchorPoint(Vec2(0.5, 1));
    textGameName->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height-50));
    this->addChild(textGameName);

    versionTitle = "版本号:";
    Text* textVersion = Text::create(versionTitle, "AmericanTypewriter", 46);
    textVersion->setColor(Color3B(0x64,0x64, 0x64));
    textVersion->setAnchorPoint(Vec2(0, 0));
    textVersion->setPosition(Vec2(100, 500));
    this->addChild(textVersion);
    
    versionTitle = "";
    stringstream stream;
    stream<<ZJHModel::getInstance()->vcode;
    versionTitle += "vname:"+ZJHModel::getInstance()->vname + ";vcode:" + stream.str();
    Text* textVersionName = Text::create(versionTitle, "AmericanTypewriter", 46);
    textVersionName->setColor(Color3B(0x6b,0xa8, 0x00));
    textVersionName->setAnchorPoint(Vec2(0, 0));
    textVersionName->setPosition(Vec2(378, 500));
    this->addChild(textVersionName);
    
    versionTitle = "服务器地址:";
    Text* server = Text::create(versionTitle, "AmericanTypewriter", 46);
    server->setColor(Color3B(0x64,0x64, 0x64));
    server->setAnchorPoint(Vec2(0, 0));
    server->setPosition(Vec2(100, 400));
    this->addChild(server);

    versionTitle = CCHttpAgent::getInstance()->url;
    LabelTTF *serverAddr = LabelTTF::create(versionTitle, ".SFUIDisplay-Semibold", 36, Size(490, 0), TextHAlignment::LEFT);
    serverAddr->setColor(Color3B(0x64,0x64, 0x64));
    serverAddr->setAnchorPoint(Vec2(0, 1));
    serverAddr->setPosition(Vec2(378, this->getContentSize().height - 285));
    this->addChild(serverAddr);

    versionTitle = "渠道号:";
    Text* channel = Text::create(versionTitle, "AmericanTypewriter", 46);
    channel->setColor(Color3B(0x64,0x64, 0x64));
    channel->setAnchorPoint(Vec2(0, 0));
    channel->setPosition(Vec2(100, 260));
    this->addChild(channel);

    versionTitle = ZJHModel::getInstance()->channel_id;
    Text* channelId = Text::create(versionTitle, "AmericanTypewriter", 46);
    channelId->setColor(Color3B(0x6b,0xa8, 0x00));
    channelId->setAnchorPoint(Vec2(0, 0));
    channelId->setPosition(Vec2(378, 260));
    this->addChild(channelId);

    Button *sureBtn = Button::create("Public/btn_yellow.png","Public/btn_yellow.png");
    sureBtn->setScale9Enabled(true);
    sureBtn->setAnchorPoint(Vec2(0.5,0));
    sureBtn->setContentSize(Size(801, 120));
    sureBtn->setPosition(Vec2(this->getContentSize().width/2, 70));
    this->addChild(sureBtn);
    sureBtn->setTitleText("关闭");
    sureBtn->setTitleFontSize(44);
    sureBtn->setTitleColor(Color3B(0x6b,0xa8, 0x00));
    sureBtn->setTitleFontName(".SFUIDisplay-Semibold");
//    sureBtn->addClickEventListener(CC_CALLBACK_1(VersionMessageDialog::dismiss, this));
    sureBtn->addClickEventListener([=](Ref* ref){
        this->removeFromParentAndCleanup(true);
    });
    return true;
}

void VersionMessageDialog::show(){

}

void VersionMessageDialog::dismiss(Ref* ref){
    VersionMessageDialog* dialog = (VersionMessageDialog*)ref;
    dialog->getParent()->removeFromParentAndCleanup(true);
}
