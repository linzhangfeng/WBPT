//
//  邀请新版
//
//

#include "RoomInvite.h"
#include "RoomInviteFriend.h"

void RoomInvite::show()
{
    Size winSize = Director::getInstance()->getWinSize();
    this->setPosition(Vec2(winSize.width, 0));
    this->runAction(MoveTo::create(0.17, Vec2(0, 0)));
    this->setVisible(true);
}

void RoomInvite::hide()
{
    Size winSize = Director::getInstance()->getWinSize();
    this->runAction(Sequence::create(MoveTo::create(0.17, Vec2(winSize.width, 0)),
                                     RemoveSelf::create(),
                                     NULL));
}

bool RoomInvite::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan =CC_CALLBACK_2(RoomInvite::onTouchBegan,this);
    touchListener->setSwallowTouches(true);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    return true;
}

void RoomInvite::myInit(Json::Value &data)
{
    Size winSize = Director::getInstance()->getWinSize();
    Size contentSize = Size(winSize.width, 440);
    m_inviteCode = data["gameRoom"]["code"].asString();
    m_roomId = data["gameRoom"]["id"].asInt();
    m_jsonData = data;
    char buff[256];
    
    LayerColor *mask = LayerColor::create(Color4B(0, 0, 0, 50));
    mask->setContentSize(Size(winSize.width*2, winSize.height));
    mask->setPositionX(-winSize.width);
    this->addChild(mask);
    
    auto contentBg = Sprite::create("RoomPublicH/dialog/room_dialog_mid.png");
    contentBg->setName("content_bg");
    this->addChild(contentBg);
    contentBg->setPosition(winSize.width -282, 325);
    
    auto title = Sprite::create("RoomPublicH/dialog/dialog_invite_title.png");
    title->setAnchorPoint(Vec2(0.5, 1));
    title->setPosition(Vec2(contentBg->getContentSize().width/2, contentBg->getContentSize().height - 49));
    contentBg->addChild(title);
    
    Button *closeBtn = Button::create("RoomPublicH/menu_close.png");
   // closeBtn->setZoomScale(0);
    closeBtn->setPosition(Vec2(-10, contentBg->getContentSize().height/2));
    closeBtn->addClickEventListener([=](Ref *ref)
                                    {
                                        this->hide();
                                    });
    contentBg->addChild(closeBtn,1);
    
    //邀请码
    auto inCodeBg = ui::Scale9Sprite::create("RoomPublicH/dialog/dialog_invite_item_bg.png");
    inCodeBg->setPreferredSize(Size(496,125));
    inCodeBg->setAnchorPoint(Vec2(1, 1));
    inCodeBg->setPosition(Vec2(contentBg->getContentSize().width - 23, contentBg->getContentSize().height - 105));
    contentBg->addChild(inCodeBg);
    
    snprintf(buff, sizeof(buff), "邀请码(%s)", m_inviteCode.c_str());
    Text* codeLabel = Text::create(buff, "AmericanTypewriter", 30);
    codeLabel->setAnchorPoint(Vec2(0, 0.5));
    codeLabel->setPosition(Vec2(42, inCodeBg->getContentSize().height / 2));
    codeLabel->setTextColor(Color4B(0x4d, 0x2d, 0x1e, 255));
    inCodeBg->addChild(codeLabel);
    
    auto copyBtn = Button::create("RoomPublicH/dialog/dialog_code_copy.png");
    copyBtn->setAnchorPoint(Vec2(1, 0.5));
    copyBtn->setPosition(Vec2(inCodeBg->getContentSize().width-24, inCodeBg->getContentSize().height / 2));
    copyBtn->setScale9Enabled(true);
    copyBtn->addClickEventListener([=](Ref *sender)
                                   {
                                       PlatformHelper::sdkDoForVoid_C("copyToPasteboard", m_inviteCode);
                                       PlatformHelper::showToast("复制成功");
                                   });
    inCodeBg->addChild(copyBtn);
    
    Size btnSize = Size(95, 95);
    std::string imageFile[3] = {
        "RoomPublicH/dialog/invite_wanbei.png",
        "RoomPublicH/dialog/invite_weixin.png",
        "RoomPublicH/dialog/invite_qq.png",
    };
    std::string btnNames[3] = {
        "wanbei",
        "wechat",
        "qq",
    };
    std::string names[3] = {
        "邀请玩呗好友",
        "邀请微信好友",
        "邀请QQ好友",
    };
    Vec2 btnPos[3] = {
        Vec2(540, 407),
        Vec2(540, 300),
        Vec2(540, 195),
    };
    for (int i = 0; i < 3; i++) {
        Button *btn = Button::create("RoomPublicH/dialog/dialog_invite_item_bg.png", "RoomPublicH/dialog/dialog_invite_item_bg.png");
        btn->setAnchorPoint(Vec2(1, 1));
        btn->setScale9Enabled(true);
        btn->setContentSize(Size(496,94));
        btn->setPosition(btnPos[i]);
        btn->setName(btnNames[i]);
        btn->addClickEventListener(CC_CALLBACK_1(RoomInvite::buttonClicked, this));
        contentBg->addChild(btn);
        
        auto icon = Sprite::create(imageFile[i]);
        icon->setAnchorPoint(Vec2(0, 0.5));
        icon->setPosition(Vec2(20, 47));
        btn->addChild(icon);
        
        Text *name = Text::create(names[i], "AmericanTypewriter", 24);
        name->setTextColor(Color4B(0xaf, 0x5a, 0x22, 255));
        name->setAnchorPoint(Vec2(0,0.5));
        name->setPosition(Vec2(120,47));
        btn->addChild(name);
        
        icon = Sprite::create("RoomPublicH/dialog/invite_more.png");
        icon->setAnchorPoint(Vec2(1, 0.5));
        icon->setPosition(Vec2(btn->getContentSize().width - 25,47));
        btn->addChild(icon);

    }
}

bool RoomInvite::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    auto contentBg = dynamic_cast<Node *>(this->getChildByName("content_bg"));
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

void RoomInvite::buttonClicked(cocos2d::Ref *sender)
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
                                                                     auto layer = RoomInviteFriend::create();
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
