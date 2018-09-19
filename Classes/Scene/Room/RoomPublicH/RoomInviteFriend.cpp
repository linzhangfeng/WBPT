//
//  RoomInviteFriend.cpp
//  ZJH
//
//  Created by hyhy on 16/7/27.
//
//

#include "RoomInviteFriend.h"

void RoomInviteFriend::show()
{
    Size winSize = Director::getInstance()->getWinSize();
    this->setPosition(Vec2(winSize.width, 0));
    this->runAction(MoveTo::create(0.17, Vec2(0, 0)));
    this->setVisible(true);
}

void RoomInviteFriend::hide()
{
    Size winSize = Director::getInstance()->getWinSize();
    this->runAction(Sequence::create(MoveTo::create(0.17, Vec2(winSize.width, 0)),
                                     RemoveSelf::create(),
                                     NULL));
}

bool RoomInviteFriend::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    m_hasMore = false;
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan =CC_CALLBACK_2(RoomInviteFriend::onTouchBegan,this);
    touchListener->setSwallowTouches(true);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    Size winSize = Director::getInstance()->getWinSize();
    
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
    closeBtn->setPosition(Vec2(-10, contentBg->getContentSize().height/2));
    closeBtn->addClickEventListener([=](Ref *ref)
                                    {
                                        this->hide();
                                    });
    contentBg->addChild(closeBtn,1);
    
    auto inviteBtn = Button::create("RoomPublicH/dialog/invite_confirm.png");
    inviteBtn->setAnchorPoint(Vec2(0.5, 0));
    inviteBtn->setScale9Enabled(true);
    inviteBtn->setPosition(Vec2(contentBg->getContentSize().width/2,30));
    inviteBtn->setName("invite_button");
    inviteBtn->setTouchEnabled(true);
    inviteBtn->addClickEventListener(CC_CALLBACK_1(RoomInviteFriend::buttonClicked, this));
    contentBg->addChild(inviteBtn);
    
    return true;
}

void RoomInviteFriend::myInit(int roomId, Json::Value &data)
{
    m_roomId = roomId;
    m_hasMore = data["hasMore"].asBool();
    auto contentBg = dynamic_cast<Node *>(this->getChildByName("content_bg"));
    Size winSize = Director::getInstance()->getWinSize();
    Size contentSize = contentBg->getContentSize();
    Size itemSize = Size(495, 78);
    
    ListView *resultList = ListView::create();
    resultList->setDirection(ScrollView::Direction::VERTICAL);
    resultList->setBounceEnabled(false);
    resultList->setContentSize(Size(495,422));
    resultList->setAnchorPoint(Vec2(0, 0));
    resultList->setPosition(Vec2(45, 115));
    resultList->setItemsMargin(10);
    resultList->setScrollBarEnabled(false);
    contentBg->addChild(resultList);
    
    for (int i = 0; i < data["list"].size(); i++) {
        Widget *item = Widget::create();
        item->setContentSize(itemSize);
        item->setTouchEnabled(true);
        item->addClickEventListener(CC_CALLBACK_1(RoomInviteFriend::listItemClicked, this));
        item->setTag(data["list"][i]["user"]["id"].asInt());
        
        auto itemBg = ui::Scale9Sprite::create("RoomPublicH/dialog/dialog_invite_item_bg.png");
        itemBg->setContentSize(itemSize);
        itemBg->setPosition(item->getContentSize().width/2, item->getContentSize().height/2);
        item->addChild(itemBg);
        
        Sprite *userAvatar = Utils::createCircleAvatar(data["list"][i]["user"]["image"].asString(), "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(64, 64));
        userAvatar->setAnchorPoint(Vec2(0, 0.5));
        userAvatar->setPosition(Vec2(10, itemSize.height / 2));
        item->addChild(userAvatar);
        
        std::string nickName = data["list"][i]["user"]["nickName"].asString();
        char nameBuf[128];
        Utils::parseName(8, nameBuf, nickName.c_str());
        Text* text = Text::create(nameBuf, "AmericanTypewriter", 28);
        text->setTextColor(Color4B(0x4d, 0x2d, 0x1e, 255));
        text->setAnchorPoint(Vec2(0, 0.5));
        text->setPosition(Vec2(90, itemSize.height * 0.5));
        item->addChild(text);
        
        Sprite *unselected = Sprite::create("RoomPublicH/dialog/invite_uncheck.png");
        unselected->setPosition(Vec2(425, itemSize.height / 2));
        item->addChild(unselected);
        
        Sprite *selected = Sprite::create("RoomPublicH/dialog/invite_check.png");
        selected->setPosition(unselected->getPosition());
        selected->setName("selected_sprite");
        item->addChild(selected);
        selected->setVisible(false);
        
//        auto line = Sprite::create("RoomPublic/common/line.png");
//        line->setAnchorPoint(Vec2(0,0.5));
//        line->setPosition(Vec2(20,1));
//        item->addChild(line);
        
        resultList->addChild(item);
    }
    resultList->refreshView();
}

bool RoomInviteFriend::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
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

    return true;
}

void RoomInviteFriend::buttonClicked(cocos2d::Ref *ref)
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

void RoomInviteFriend::listItemClicked(cocos2d::Ref *ref)
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
