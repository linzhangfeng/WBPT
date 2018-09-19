//
//  RoomInviteFriendLayer.cpp
//  ZJH
//
//  Created by hyhy on 16/7/27.
//
//

#include "RoomInviteFriendLayer.h"

void RoomInviteFriendLayer::show()
{
    Size winSize = Director::getInstance()->getWinSize();
    this->setPosition(Vec2(winSize.width, 0));
    this->runAction(MoveTo::create(0.17, Vec2(0, 0)));
    this->setVisible(true);
}

void RoomInviteFriendLayer::hide()
{
    Size winSize = Director::getInstance()->getWinSize();
    this->runAction(Sequence::create(MoveTo::create(0.17, Vec2(winSize.width, 0)),
                                     RemoveSelf::create(),
                                     NULL));
}

bool RoomInviteFriendLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    m_hasMore = false;
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan =CC_CALLBACK_2(RoomInviteFriendLayer::onTouchBegan,this);
    touchListener->setSwallowTouches(true);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    Size winSize = Director::getInstance()->getWinSize();
    
    LayerColor *titleBg = LayerColor::create(Color4B(15, 15, 15, 255));
    titleBg->setContentSize(Size(winSize.width, 90));
    titleBg->ignoreAnchorPointForPosition(false);
    titleBg->setAnchorPoint(Vec2(0, 1));
    titleBg->setPosition(Vec2(0, winSize.height));
    this->addChild(titleBg);
    
    LayerColor *bottomBg = LayerColor::create(Color4B(0, 0, 0, 255));
    bottomBg->setContentSize(Size(winSize.width, 100));
    bottomBg->setAnchorPoint(Vec2(0, 0));
    bottomBg->setPosition(Vec2(0, 0));
    this->addChild(bottomBg);
    
    LayerColor *contentBg = LayerColor::create(Color4B(29, 29, 30, 255));
    contentBg->setContentSize(Size(winSize.width, winSize.height - titleBg->getContentSize().height - bottomBg->getContentSize().height));
    contentBg->setAnchorPoint(Vec2(0, 0));
    contentBg->setPosition(Vec2(0, bottomBg->getContentSize().height));
    contentBg->setName("content_bg");
    this->addChild(contentBg);
    
    Text* titlelabel = Text::create("邀请好友", "AmericanTypewriter", 36);
    titlelabel->setColor(Color3B(223, 206, 173));
    titlelabel->setPosition(titleBg->getContentSize() / 2);
    titleBg->addChild(titlelabel);
    
    Button *backBtn = Button::create("Head/title-icon1.png", "Head/title-icon1_press.png");
    backBtn->setPosition(Vec2(50, titleBg->getContentSize().height / 2));
    backBtn->setScale(0.85);
    backBtn->setName("back_button");
    backBtn->addClickEventListener(CC_CALLBACK_1(RoomInviteFriendLayer::buttonClicked, this));
    titleBg->addChild(backBtn);
    
    LayerColor *inviteBg = LayerColor::create(Color4B(240, 203, 91, 255));
    inviteBg->setContentSize(Size(bottomBg->getContentSize().width - 80, bottomBg->getContentSize().height - 30));
    inviteBg->ignoreAnchorPointForPosition(false);
    inviteBg->setAnchorPoint(Vec2(0.5, 0.5));
    inviteBg->setPosition(bottomBg->getContentSize() / 2);
    bottomBg->addChild(inviteBg);
    
    Widget *inviteBtn = Widget::create();
    inviteBtn->setContentSize(inviteBg->getContentSize());
    inviteBtn->setAnchorPoint(Vec2(0.5, 0.5));
    inviteBtn->setPosition(inviteBg->getPosition());
    inviteBtn->setName("invite_button");
    inviteBtn->setTouchEnabled(true);
    inviteBtn->addClickEventListener(CC_CALLBACK_1(RoomInviteFriendLayer::buttonClicked, this));
    bottomBg->addChild(inviteBtn);
    
    Text* inviteLabel = Text::create("邀请", "AmericanTypewriter", 40);
    inviteLabel->setColor(Color3B(102, 65, 0));
    inviteLabel->setPosition(bottomBg->getContentSize() / 2);
    bottomBg->addChild(inviteLabel);
    
    return true;
}

void RoomInviteFriendLayer::myInit(int roomId, Json::Value &data)
{
    m_roomId = roomId;
    m_hasMore = data["hasMore"].asBool();
    LayerColor *contentBg = dynamic_cast<LayerColor *>(this->getChildByName("content_bg"));
    Size winSize = Director::getInstance()->getWinSize();
    Size contentSize = contentBg->getContentSize();
    Size itemSize = Size(winSize.width, 130);
    
    ListView *resultList = ListView::create();
    resultList->setDirection(ScrollView::Direction::VERTICAL);
    resultList->setBounceEnabled(false);
    resultList->setContentSize(contentSize);
    resultList->setAnchorPoint(Vec2(0, 0));
    resultList->setPosition(Vec2(0, 0));
    resultList->setItemsMargin(0);
    resultList->setScrollBarEnabled(false);
    contentBg->addChild(resultList);
    
    for (int i = 0; i < data["list"].size(); i++) {
        Widget *item = Widget::create();
        item->setContentSize(itemSize);
        item->setTouchEnabled(true);
        item->addClickEventListener(CC_CALLBACK_1(RoomInviteFriendLayer::listItemClicked, this));
        item->setTag(data["list"][i]["user"]["id"].asInt());
        
        Sprite *userAvatar = Utils::createCircleAvatar(data["list"][i]["user"]["image"].asString(), "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(100, 100));
        userAvatar->setPosition(Vec2(100, itemSize.height / 2));
        item->addChild(userAvatar);
        
        std::string nickName = data["list"][i]["user"]["nickName"].asString();
        char nameBuf[128];
        Utils::parseName(8, nameBuf, nickName.c_str());
        Text* text = Text::create(nameBuf, "AmericanTypewriter", 32);
        text->setAnchorPoint(Vec2(0, 0.5));
        text->setPosition(Vec2(190, itemSize.height * 0.75));
        item->addChild(text);
        
        Sprite *unselected = Sprite::create("RoomBottom/icon_waiting.png");
        unselected->setPosition(Vec2(itemSize.width - 65, itemSize.height / 2));
        unselected->setScale(0.8);
        item->addChild(unselected);
        
        Sprite *selected = Sprite::create("RoomBottom/icon_waiting_check.png");
        selected->setPosition(unselected->getPosition());
        selected->setName("selected_sprite");
        selected->setScale(0.8);
        item->addChild(selected);
        selected->setVisible(false);
        
        LayerColor *line = LayerColor::create(Color4B(170, 170, 170, 255));
        line->setContentSize(Size(itemSize.width - 60, 1));
        line->ignoreAnchorPointForPosition(false);
        line->setAnchorPoint(Vec2(0.5, 0));
        line->setPosition(Vec2(itemSize.width / 2, 0));
        item->addChild(line);
        
        resultList->addChild(item);
    }
    resultList->refreshView();
}

bool RoomInviteFriendLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    return true;
}

void RoomInviteFriendLayer::buttonClicked(cocos2d::Ref *ref)
{
    Widget *sender = dynamic_cast<Widget *>(ref);
    if (sender) {
        std::string senderName = sender->getName();
        if (senderName == "back_button")
        {
            this->hide();
        }
        else if (senderName == "invite_button")
        {
            if (m_selection.size() == 0) {
                PlatformHelper::showToast("请选择好友");
                return;
            }
            
            std::string userList = "";
            char buff[64];
            for (std::set<int>::const_iterator i = m_selection.begin(); i != m_selection.end(); i++) {
                snprintf(buff, sizeof(buff), "%d,", *i);
                userList = userList + buff;
            }
            Json::Value json;
            json["userlist"] = userList;
            json["roomId"] = m_roomId;
            CCHttpAgent::getInstance()->sendHttpPost(NULL, "duiju/invite/add", json.toStyledString());
            
            PlatformHelper::showToast("邀请已发送");
            this->hide();
        }
    }
}

void RoomInviteFriendLayer::listItemClicked(cocos2d::Ref *ref)
{
    Widget *item = dynamic_cast<Widget *>(ref);
    Sprite *selectedSp = dynamic_cast<Sprite *>(item->getChildByName("selected_sprite"));
    selectedSp->setVisible(!selectedSp->isVisible());
    int tag = item->getTag();
    
    if (selectedSp->isVisible()) {
        m_selection.insert(tag);
    } else {
        m_selection.erase(tag);
    }
}