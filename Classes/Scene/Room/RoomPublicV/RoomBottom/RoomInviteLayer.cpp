//
//  RoomInviteLayer.cpp
//  ZJH
//
//  Created by hyhy on 16/7/26.
//
//

#include "RoomInviteLayer.h"
#include "RoomInviteFriendLayer.h"

void RoomInviteLayer::show()
{
    Size winSize = Director::getInstance()->getWinSize();
    this->setPosition(Vec2(0, -winSize.height));
    this->runAction(MoveTo::create(0.17, Vec2(0, 0)));
    this->setVisible(true);
}

void RoomInviteLayer::hide()
{
    Size winSize = Director::getInstance()->getWinSize();
    this->runAction(Sequence::create(MoveTo::create(0.17, Vec2(0, -winSize.height)),
                                     RemoveSelf::create(),
                                     NULL));
}

bool RoomInviteLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan =CC_CALLBACK_2(RoomInviteLayer::onTouchBegan,this);
    touchListener->setSwallowTouches(true);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    return true;
}

void RoomInviteLayer::myInit(Json::Value &data)
{
    Size winSize = Director::getInstance()->getWinSize();
    Size contentSize = Size(winSize.width, 440);
    m_inviteCode = data["gameRoom"]["code"].asString();
    m_roomId = data["gameRoom"]["id"].asInt();
    m_jsonData = data;
    char buff[256];
    
    LayerColor *mask = LayerColor::create(Color4B(0, 0, 0, 50));
    mask->setContentSize(Size(winSize.width, winSize.height * 2));
    this->addChild(mask);
    
    LayerColor *contentBg = LayerColor::create(Color4B(36, 36, 36, 255));
    contentBg->setContentSize(contentSize);
    contentBg->setName("content_bg");
    this->addChild(contentBg);
    
    Text* titlelabel = Text::create("邀请", "AmericanTypewriter", 32);
    titlelabel->setPosition(Vec2(contentSize.width / 2, contentSize.height - 44));
    titlelabel->setColor(Color3B(223, 206, 173));
    contentBg->addChild(titlelabel);
    
    Button *closeBtn = Button::create("Public/icon_cancel.png");
    closeBtn->setZoomScale(0);
    closeBtn->setPosition(Vec2(winSize.width - 45, titlelabel->getPositionY()));
    closeBtn->addClickEventListener([=](Ref *ref)
                                    {
                                        this->hide();
                                    });
    contentBg->addChild(closeBtn);
    
    //邀请码
    LayerColor *codeBg = LayerColor::create(Color4B(20, 20, 21, 255));
    codeBg->setContentSize(Size(winSize.width, 64));
    codeBg->setPosition(Vec2(0, 270));
    contentBg->addChild(codeBg);
    
    snprintf(buff, sizeof(buff), "房间号(%s)", m_inviteCode.c_str());
    Text* codeLabel = Text::create(buff, "AmericanTypewriter", 32);

    codeLabel->setAnchorPoint(Vec2(0, 0.5));
    codeLabel->setPosition(Vec2(150, codeBg->getContentSize().height / 2));
    codeLabel->setColor(Color3B(223, 206, 173));
    codeBg->addChild(codeLabel);
    
    
    LayerColor *copyBg = LayerColor::create(Color4B(240, 203, 91, 255));
    copyBg->setContentSize(Size(180, 70));
    copyBg->ignoreAnchorPointForPosition(false);
    copyBg->setAnchorPoint(Vec2(0.5, 0.5));
    copyBg->setPosition(Vec2(winSize.width - 110, codeBg->getContentSize().height / 2));
    codeBg->addChild(copyBg);
    
    Widget *copyBtn = Widget::create();
    copyBtn->setContentSize(copyBg->getContentSize());
    copyBtn->setAnchorPoint(Vec2(0.5, 0.5));
    copyBtn->setPosition(copyBg->getPosition());
    copyBtn->setTouchEnabled(true);
    copyBtn->addClickEventListener([=](Ref *sender)
                                   {
                                       PlatformHelper::sdkDoForVoid_C("copyToPasteboard", m_inviteCode);
                                       PlatformHelper::showToast("复制成功");
                                   });
    codeBg->addChild(copyBtn);
    
    Text* copyTip = Text::create("复制房间号", "AmericanTypewriter", 30);
    copyTip->setColor(Color3B(102, 65, 0));
    copyTip->setPosition(copyBtn->getContentSize() / 2);
    copyBtn->addChild(copyTip);
    
    Size btnSize = Size(95, 95);
    std::string imageFile[3] = {
        "RoomHead/notification.png",
        "RoomHead/windows_share_icon2.png",
        "RoomHead/windows_share_icon4.png",
    };
    std::string btnNames[3] = {
        "wanbei",
        "wechat",
        "qq",
    };
    std::string names[3] = {
        "玩呗好友",
        "微信",
        "QQ好友",
    };
    Vec2 btnPos[3] = {
        Vec2(contentSize.width * 0.191, 155),
        Vec2(contentSize.width * 0.5, 155),
        Vec2(contentSize.width * 0.809, 155),
    };
    for (int i = 0; i < 3; i++) {
        Button *btn = Button::create(imageFile[i]);
        btn->setScale(btnSize.width / btn->getContentSize().width, btnSize.height / btn->getContentSize().height);
        btn->setPosition(btnPos[i]);
        btn->setName(btnNames[i]);
        btn->addClickEventListener(CC_CALLBACK_1(RoomInviteLayer::buttonClicked, this));
        contentBg->addChild(btn);
        
        Text *name = Text::create(names[i], "AmericanTypewriter", 24);
        name->setPosition(Vec2(btn->getPositionX(), btn->getPositionY() - 85));
        contentBg->addChild(name);
    }
}

bool RoomInviteLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    LayerColor *contentBg = dynamic_cast<LayerColor *>(this->getChildByName("content_bg"));
    if (contentBg->boundingBox().containsPoint(this->convertToNodeSpace(touch->getLocation())))
    {
        //
    }
    else
    {
        this->hide();
        event->stopPropagation();
    }
    return true;
}

void RoomInviteLayer::buttonClicked(cocos2d::Ref *sender)
{
    Button *btn = dynamic_cast<Button *>(sender);
    std::string btnName = btn->getName();
    if (btnName == "wanbei")
    {
        Json::Value json;
        json["pageNo"] = 1;
        json["pageSize"] = 50;
        CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag)
                                                 {
                                                     CCHttpPacket *packet = CCHttpAgent::getInstance()->packets[tag];
                                                     if (packet) {
                                                         if (getReferenceCount() == 0 || getReferenceCount() > 10)
                                                         {
                                                             //nothing
                                                         }
                                                         else
                                                         {
                                                             if (packet->status == 3) {
                                                                 packet->status = 0;
                                                                 if (packet->resultIsOK())
                                                                 {
                                                                     RoomInviteFriendLayer *layer = RoomInviteFriendLayer::create();
                                                                     layer->myInit(m_roomId, packet->recvVal["resultMap"]);
                                                                     Director::getInstance()->getRunningScene()->addChild(layer, Z_ORDER_IN_GAME_LAYER);
                                                                     layer->show();
                                                                 }
                                                                 else
                                                                 {
                                                                     PlatformHelper::showToast(packet->recvVal["message"].asString().c_str());
                                                                 }
                                                                 
                                                             }
                                                             //
                                                         }
                                                         
                                                         //删除packet
                                                         CCHttpAgent::getInstance()->packets.erase(tag);
                                                         delete packet;
                                                         this->hide();
                                                     }
                                                 },
                                                 "friend/listFriends", json.toStyledString(), "invite_friend");
    }
    else if (btnName == "wechat")
    {
        PlatformHelper::WeChatShare(m_jsonData["gameRoom"]["shareTitle"].asString().c_str(), m_jsonData["gameRoom"]["shareContent"].asString().c_str(), m_jsonData["gameRoom"]["shareUrl"].asString().c_str(), "");
        this->hide();
    }
    else if (btnName == "qq")
    {
        PlatformHelper::showToast("敬请期待");
    }
}
