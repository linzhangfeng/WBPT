//
//房间信息改版
//

#include "RoomInformation.h"

//房间信息展示
void RoomInformation::show()
{
    Size winSize = Director::getInstance()->getWinSize();
    this->setPosition(Vec2(0, 0));
    this->runAction(MoveTo::create(0.17, Vec2(0, 0)));
    this->setVisible(true);
}


//房间信息隐藏
void RoomInformation::hide()
{
    Size winSize = Director::getInstance()->getWinSize();
    this->runAction(Sequence::create(MoveTo::create(0.17, Vec2(winSize.width, 0)),
                                     RemoveSelf::create(),NULL));
}

bool RoomInformation::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(RoomInformation::onTouchBegan,this);
    touchListener->setSwallowTouches(true);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    return true;
}


//房间信息展示
void RoomInformation::myInit(Json::Value &data)
{
    Size winSize = Director::getInstance()->getWinSize();
    Size contentSize = Size(winSize.width, 780);
    //阴影背景
    LayerColor *mask = LayerColor::create(Color4B(0, 0, 0, 50));
    mask->setContentSize(Size(winSize.width*2, winSize.height));
    mask->setPositionX(-winSize.width);
    this->addChild(mask);
    
    
    Size bgSize = Size(613,480);
    //背景框
    auto contentBg = Scale9Sprite::create("RoomPublicH/dialog/room_dialog_small.png");
    contentBg->setAnchorPoint(Vec2(0.5, 0.5));
    contentBg->setPosition(winSize / 2);
    contentBg->setContentSize(Size(bgSize.width, bgSize.height));
    contentBg->setName("content_bg");
    this->addChild(contentBg);
    
    //标题图片
    auto titleSp = ImageView::create("RoomPublicH/dialog/dialog_roominfo_title.png");
    titleSp->setScale9Enabled(true);
    titleSp->setAnchorPoint(Vec2(0.5, 1));
    titleSp->setPosition(Vec2(bgSize.width/2, bgSize.height-10));
    contentBg->addChild(titleSp);
    
    
    //关闭按钮
    auto closeBtn = Button::create("RoomPublicH/dialog/dialog_close.png", "RoomPublicH/dialog/dialog_close_press.png");
    closeBtn->ignoreContentAdaptWithSize(false);
    closeBtn->setScale9Enabled(true);
    closeBtn->setPosition(Vec2(bgSize.width - 65, bgSize.height - 69-20));
    closeBtn->setTag(ALERT_DIALOG_BUTTON_CLOSE);
    closeBtn->addClickEventListener([=](Ref *ref)
                                    {
                                        this->removeFromParentAndCleanup(true);
                                    });
    contentBg->addChild(closeBtn);
        
   
    std::string nickName = data["gameRoom"]["user"]["nickName"].asString();
    std::string roomName = data["gameRoom"]["roomName"].asString();
    std::string code = data["gameRoom"]["code"].asString();
    std::string roomType = data["way"]["name"].asString();
    
    char buff[258];
    
    //房间名称
    Utils::parseName(6, buff, roomName.c_str());
    auto roomNameLabel = this->createRichText("房间名称：", buff, 0);
    roomNameLabel->setAnchorPoint(Vec2(0, 1));
    roomNameLabel->setPosition(Vec2(64+22, bgSize.height-133-35));
    contentBg->addChild(roomNameLabel);
    
    //邀请码
    auto codeText = this->createRichText("邀请码：", code.c_str(), 1);
    codeText->setAnchorPoint(Vec2(0, 1));
    codeText->setPosition(Vec2(351+22, bgSize.height-133-35));
    contentBg->addChild(codeText);
    
    //房主
    Utils::parseName(10, buff, nickName.c_str());
    auto userNameLabel = this->createRichText("房主：", buff, 0);
    userNameLabel->setAnchorPoint(Vec2(0, 1));
    userNameLabel->setPosition(Vec2(64+22, bgSize.height-181-35));
    contentBg->addChild(userNameLabel);
    
    //局数
    auto timeLabel = this->createRichText("局数：", "8/90", 0);
    timeLabel->setAnchorPoint(Vec2(0, 1));
    timeLabel->setPosition(Vec2(64+22, bgSize.height-231-35));
    contentBg->addChild(timeLabel);
    
    int onlineCount = data["gameRoom"]["onlineCount"].asInt();
    int againstNumber = data["gameRoom"]["againstNumber"].asInt();
    snprintf(buff, sizeof(buff), "%d/%d人", onlineCount, againstNumber);
    //人数
    auto teamLabel = this->createRichText("人数：", buff, 0);
    teamLabel->setAnchorPoint(Vec2(0, 1));
    teamLabel->setPosition(Vec2(351+22, bgSize.height-231-35));
    contentBg->addChild(teamLabel);
    
    //娱乐类型
    std::string gameName = data["gameRoom"]["gameName"].asString();
    int bottomNote = data["gameRoom"]["bottomNote"].asInt();
    int takeIn = data["gameRoom"]["takeIn"].asInt();
    snprintf(buff, sizeof(buff), "%s (底注%d) (初始带入%d)", gameName.c_str(), bottomNote, takeIn);
    
    auto typeName = this->createRichText("娱乐类型：", "", 0);
    typeName->setAnchorPoint(Vec2(0, 1));
    typeName->setPosition(Vec2(64+22, bgSize.height-281-35));
    contentBg->addChild(typeName);
    auto typeLabel = this->createRichText("", buff, 0);
    typeLabel->setAnchorPoint(Vec2(0, 1));
    typeLabel->setPosition(Vec2(64+22, bgSize.height-313-35));
    contentBg->addChild(typeLabel);
    
//    txt = Text::create("对局时长:", "AmericanTypewriter", 28);
//    txt->setAnchorPoint(Vec2(0, 0.5));
//    txt->setPosition(Vec2(15, 38));
//    bg->addChild(txt);
    
//    int durationTime = data["gameRoom"]["duration"].asInt();
//    snprintf(buff, sizeof(buff), "%d分钟", durationTime);
//    Text *durationLabel = Text::create(buff, "AmericanTypewriter", 28);
//    durationLabel->setAnchorPoint(Vec2(0, 0.5));
//    durationLabel->setPosition(Vec2(150, 38));
//    bg->addChild(durationLabel);

}

//生成对应的富文本控件
RichText * RoomInformation::createRichText(const std::string &label, const std::string &content, int type){
    auto richText = RichText::create();
    auto richLabel = RichElementText::create(1, Color3B(0x4d, 0x2d, 0x1e), 255, label, "AmericanTypewriter", 22);
    RichElementText *richContent;
    if (type == 0){
        richContent = RichElementText::create(2, Color3B(0xaf, 0x5a, 0x22), 255, content, "AmericanTypewriter", 22);
    }else{
        richContent = RichElementText::create(2, Color3B(0x22, 0xa6, 0xeb), 255, content, "AmericanTypewriter", 22);
    }
    
    richText->pushBackElement(richLabel);
    richText->pushBackElement(richContent);
    return richText;
}

bool RoomInformation::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
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
