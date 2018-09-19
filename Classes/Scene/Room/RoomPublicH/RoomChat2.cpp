//
//  RoomChat2.cpp
//  ZJH
//
//  Created by pg on 16/9/6.
//
//

#include "RoomChat2.hpp"
#include "RoomChatBottom2.hpp"
#include "proto.h"
#include "../../../Model/MsgModel.h"
#include "../../../sqlite3/DataBase.h"
#include "../../Loading/Loading.h"
#include "../RoomPublicV/Utils/ChatUtils.hpp"

#define ss 0.67

void RoomChat2::show()
{
    stopAllActions();
    Size size = Director::getInstance()->getWinSize();
    runAction(MoveTo::create(0.20, m_nPosition));
    setVisible(true);
    
    auto chatBottom2 = getChildByName<RoomChatBottom2 *>("chat_bottom");
    chatBottom2->initFast();
    
    m_pTouchListener = EventListenerTouchOneByOne::create();
    m_pTouchListener->onTouchBegan =CC_CALLBACK_2(RoomChat2::onTouchBegan,this);
    m_pTouchListener->setSwallowTouches(true);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_pTouchListener, this);
}

void RoomChat2::hide()
{
    stopAllActions();
    Size size = Director::getInstance()->getWinSize();
    runAction(MoveTo::create(0.20, Vec2(getPositionX() - 1300,getPositionY())));
    setVisible(false);
    Director::getInstance()->getEventDispatcher()->removeEventListener(m_pTouchListener);
}

bool RoomChat2::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    setName("RoomChat2");
    
    Size size = Director::getInstance()->getWinSize();
    
    auto ChatBG = Sprite::create("RoomPublicH/dialog/dialog_chat_bg.png");
    ChatBG->setAnchorPoint(Vec2(0,0));
    ChatBG->setName("content_bg");
    ChatBG->setPosition(0, 0);
    setContentSize(ChatBG->getContentSize());
    
    auto title = Sprite::create("RoomPublicH/chat/chat_title.png");
    title->setAnchorPoint(Vec2(0.5, 1));
    title->setPosition(Vec2(ChatBG->getContentSize().width/2, ChatBG->getContentSize().height - 40));
    ChatBG->addChild(title);
    
    Button *closeBtn = Button::create("GameHall/createRoom/c1.png", "GameHall/createRoom/c2.png");
    // closeBtn->setZoomScale(0);
    closeBtn->setScale(0.6);
    closeBtn->setPosition(Vec2(ChatBG->getContentSize().width - 25, ChatBG->getContentSize().height - 25));
    closeBtn->addClickEventListener([=](Ref *ref)
                                    {
                                        this->hide();
                                    });
    ChatBG->addChild(closeBtn,1);
    
    Node* pBGNode = Node::create();
    pBGNode->addChild(ChatBG);
    pBGNode->setName("content_bg");
    pBGNode->setContentSize(getContentSize());
    addChild(pBGNode);
    
    m_nPosition = getPosition();
    setPositionX(getPositionX() - 1000);
    
    auto chat_bottom = RoomChatBottom2::create(ChatBG->getContentSize());
    chat_bottom->setName("chat_bottom");
    addChild(chat_bottom,1000);
    
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(RoomChat2::notificationChat), "RoomChat_reflash", NULL);
    
    curPage = 1;
    lastTime = 0;
    newTime = 0;
    hasMore = true;
    pageSize = 10;
    lastId = 0;
    initList();
    
    return true;
}

void RoomChat2::initList()
{
    removeChildByName("list");
    Size size = Director::getInstance()->getWinSize();
    
    auto listViewBg = ui::Scale9Sprite::create("RoomPublicH/dialog/dialog_invite_item_bg.png");
    listViewBg->setPreferredSize(Size(495, 463));
    listViewBg->setAnchorPoint(Vec2(0, 0));
    listViewBg->setPosition(Vec2(23, 95));
    addChild(listViewBg);
    
    ListView* listView = ListView::create();
    listView->setDirection(ui::ScrollView::Direction::VERTICAL);
    listView->setBounceEnabled(false);
    listView->setContentSize(Size(495, 463));
    listView->setAnchorPoint(Vec2(0, 0));
    listView->setPosition(Vec2(23, 95));
    listView->setScrollBarEnabled(false);
    listView->setName("list");
    addChild(listView);
    
    listView->setItemsMargin(5);
//    listView->setTouchEnabled(false);
}

void RoomChat2::getRecordData(bool init)
{
//    ListView* listView = (ListView*)getChildByName("list");
//    MsgModel::FriendInfo* friendInfo = MsgModel::getInstance()->friendList[MsgModel::getInstance()->curChatUID];
//    
//    std::vector<MsgModel::Chat*> chatList = DataBase::getInstance(ZJHModel::getInstance()->uid)->getChat(MsgModel::getInstance()->curChatUID, curPage, pageSize, lastId);
//
//    Vec2 p = listView->getInnerContainerPosition();
//    
//    if (chatList.size() < pageSize)hasMore = false;
//    for (int i = 0; i < chatList.size() ; i ++)
//    {
//        MsgModel::Chat* chat = chatList[i];
//        if (!chat)continue;
//        
//        if (newTime == 0) newTime = chat->createTime;
//        
//        long dtime = lastTime - chat->createTime;
//        if (lastTime != 0 && dtime >= 5 * 60 * 1000)
//        {
//            RoomChat2_Item* item = RoomChat2_Item::create();
//            item->initData2(chat->createTime);
//            
//            auto layout = Layout::create();
//            layout->setContentSize(item->getContentSize());
//            layout->addChild(item);
//            listView->insertCustomItem(layout, 0);
//        }
//        lastTime = chat->createTime;
//        
//        RoomChat2_Item* item = RoomChat2_Item::create();
//        item->initData(chat);
//        
//        auto layout = Layout::create();
//        layout->setContentSize(item->getContentSize());
//        layout->addChild(item);
//        listView->insertCustomItem(layout, 0);
//        
//        friendInfo->chatList.push_back(chat);//for delete this object in the view onExit
//        
//        if (i == chatList.size() - 1)
//        {
//            lastId = chat->id;
//        }
//    }
//    
//    if (!init)
//    {
//        listView->refreshView();
//        listView->setInnerContainerPosition(p);
//    }
}

void RoomChat2::notificationChat(Ref* msg)
{
    __String* str = (__String*)msg;
    if (str != NULL)
    {
        std::string s = str->getCString();
        if(s == "all")
        {
            initList();
            return;
        }
    }

    ListView* listView = (ListView*)getChildByName("list");
    
    if (listView == NULL)
    {
        return;
    }

    Vec2 p = listView->getInnerContainerPosition();
    
    MsgModel::Chat* chat = MsgModel::getInstance()->roomChatList[MsgModel::getInstance()->roomChatList.size() - 1];
    
    long long dtime = chat->createTime - newTime;
    if (newTime == 0 || dtime >= 5 * 60 * 1000)
    {
        RoomChat2_Item* item = RoomChat2_Item::create();
        item->initData2(chat->createTime,Size(listView->getContentSize().width , getContentSize().height));
        
        auto layout = Layout::create();
        layout->setContentSize(item->getContentSize());
        layout->addChild(item);
        listView->addChild(layout);
    }
    
    if(listView->getChildrenCount() > 100)listView->removeItem(0);
    
    newTime = chat->createTime;
    
    RoomChat2_Item* item = RoomChat2_Item::create();
    item->initData(chat,Size(listView->getContentSize().width , getContentSize().height));
    item->setPositionY(item->getPositionY());
    
    auto layout = Layout::create();
    layout->setContentSize(Size(item->getContentSize().width,item->getContentSize().height));
    layout->addChild(item);
    listView->addChild(layout);
    
    listView->refreshView();
    if(p.y >= -50)listView->jumpToBottom();
    
    lastTime = chat->createTime;
}

bool RoomChat2::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    auto *contentBg = getChildByName("content_bg");
    if (!contentBg->boundingBox().containsPoint(this->convertToNodeSpace(touch->getLocation())))
    {
        this->hide();
        event->stopPropagation();
    }
    return true;
}

void RoomChat2::btnCallback(Ref* ref)
{
    
}

void RoomChat2::onExit()
{
    Layer::onExit();
    __NotificationCenter::getInstance()->removeAllObservers(this);
}

//---------------RoomChat2_Item-------------------
bool RoomChat2_Item::init()
{
    if (!Layer::init())
    {
        return false;
    }
    return true;
}

void RoomChat2_Item::initData(MsgModel::Chat* chat,Size nChatSize)
{
    setChatSize(nChatSize);
    chatP = chat;
    
    Size size = nChatSize;
    
    int fromid = chat->fromId;
    
    MsgModel::FriendInfo* fi = MsgModel::getInstance()->friendList[fromid];
    
    Sprite* avatar = Utils::createCircleAvatar(fi->photo, "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(57,57));
    this->addChild(avatar);
    
    Layer* content_layer = Layer::create();
    content_layer->ignoreAnchorPointForPosition(false);
    addChild(content_layer);
    
    std::string content;
    if (chat->msgType == MESSAGE_TYPE::MSG_TYPE_IMAGE)
    {
        content = "[图片]";
        
        Size maxSize = Size(640, 400) * ss;
        content_layer->setContentSize(maxSize + Size(10,10));
        
        ImageView *image = ImageView::create();
        image->setContentSize(maxSize);
        image->setAnchorPoint(Vec2(0.0, 0.5));
        image->setPosition(Vec2(5, content_layer->getContentSize().height / 2));
        image->ignoreContentAdaptWithSize(false);
        image->setFillContentSizeEnabled(false);
        content_layer->addChild(image);
        
        if (Utils::hasImageFile(chat->content)) {
            image->loadTexture(Utils::getImageSavePath(chat->content));
            image->adaptRenderers();
            
            LayerColor* bg = LayerColor::create(Color4B(36, 36, 36,255));
            bg->ignoreAnchorPointForPosition(false);
            bg->setContentSize(image->getContentSize() + Size(10,10));
            bg->setAnchorPoint(Vec2(0.0f, 0.0f));
            bg->setPosition(Vec2::ZERO);
            content_layer->addChild(bg, -100);
            
            content_layer->setContentSize(bg->getContentSize());
            
        } else {
            auto loading = Loading::startLoading(image,image->getContentSize() / 2);
            CCHttpAgent::getInstance()->sendHttpImage(chat->content, Utils::getImageSavePath(chat->content), Utils::getImageSavePath(chat->content), [=](std::string tag)
                                                      {
                                                          if (loading)loading->removeFromParentAndCleanup(true);
                                                          
                                                          CCHttpPacket *packet = CCHttpAgent::getInstance()->packets[tag];
                                                          if (packet) {
                                                              CCHttpAgent::getInstance()->packets.erase(tag);
                                                              delete packet;
                                                          }
                                                          
                                                          if (image == NULL || image->getReferenceCount() <= 0 || image->getReferenceCount() > 10) return;
                                                          
                                                          if(Utils::hasImageFile(chat->content))
                                                          {
                                                              image->loadTexture(Utils::getImageSavePath(chat->content));
                                                              image->adaptRenderers();
                                                              LayerColor* bg = LayerColor::create(Color4B(36, 36, 36,255));
                                                              bg->ignoreAnchorPointForPosition(false);
                                                              bg->setContentSize(image->getContentSize() + Size(10,10));
                                                              bg->setAnchorPoint(Vec2(0.0f, 0.0f));
                                                              bg->setPosition(Vec2::ZERO);
                                                              content_layer->addChild(bg, -100);
                                                              
                                                              content_layer->setContentSize(bg->getContentSize());
                                                          }
                                                      });
        }
        
        image->setTouchEnabled(true);
        image->setSwallowTouches(false);
        
        image->addClickEventListener([=](Ref *ref)
                                     {
                                         Vec2 dvec2 = image->getTouchBeganPosition() - image->getTouchEndPosition();
                                         
                                         if (dvec2.length() <= 10)
                                         {
//                                             Json::Value json;
//                                             json["img"] = chat->content;
//                                             ZJHModel::getInstance()->gotoView(VIEW_SHOW_IMAGE,json);
                                         }

                                     });
    }
    else if (chat->msgType == MESSAGE_TYPE::MSG_TYPE_AUDIO)
    {
        soundfile = Utils::getSoundSavePath(chat->content);;
        
        __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(RoomChat2_Item::startPlayNotify), "chat_sound_play_start_notify", NULL);
        
        __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(RoomChat2_Item::stopPlayNotify), "chat_sound_play_over_notify", NULL);
        
        int time = chat->roomID;
        int baseLen = 100 + time * 10;
        baseLen = MIN(baseLen,300);
        
        Layout* _richText = Layout::create();
        _richText->setContentSize(Size(baseLen, 40));
        
        Size vs = _richText->getContentSize();
        
        content_layer->setContentSize(vs + Size(0,0));
        
        _richText->setAnchorPoint(Vec2(0.0, 0.0));
        _richText->setPosition(Vec2(0, 0));
        content_layer->addChild(_richText,1000);
        
//        LayerColor* bg = LayerColor::create(Color4B(36, 36, 36,255));
//        bg->ignoreAnchorPointForPosition(false);
//        bg->setContentSize(content_layer->getContentSize());
//        bg->setAnchorPoint(Vec2(0.0f, 0.0f));
//        bg->setPosition(Vec2::ZERO);
//        content_layer->addChild(bg, -100);
        
        auto itemBg = ui::Scale9Sprite::create("RoomPublicH/chat/chat_item_bg.png");
        itemBg->ignoreAnchorPointForPosition(false);
        itemBg->setContentSize(content_layer->getContentSize());
        itemBg->setAnchorPoint(Vec2(0.0f, 0.0f));
        itemBg->setPosition(Vec2::ZERO);
        content_layer->addChild(itemBg, -100);
        
        Director::getInstance()->getTextureCache()->addImage("Chat/soundeffect/dd_left_voice_1.png");
        Director::getInstance()->getTextureCache()->addImage("Chat/soundeffect/dd_left_voice_2.png");
        Director::getInstance()->getTextureCache()->addImage("Chat/soundeffect/dd_left_voice_3.png");
        
        chating = Sprite::create("Chat/soundeffect/dd_left_voice_3.png");
        chating->setScale(0.9);
        content_layer->addChild(chating,1000);
        
        auto text = Text::create(__String::createWithFormat("%d\"",time)->getCString(), "AmericanTypewriter", 20);
        text->setColor(Color3B(0xaf, 0x5a, 0x22));
        content_layer->addChild(text,1000);
        
        if (fromid == ZJHModel::getInstance()->uid)
        {
            chating->setAnchorPoint(Vec2(0, 0.5));
            chating->setFlippedX(true);
            chating->setPosition(Vec2(baseLen - 30 - chating->getContentSize().width,vs.height / 2));
            
            text->setAnchorPoint(Vec2(1, 0.5));
            text->setPosition(Vec2(-3,vs.height / 2));
        }
        else
        {
            chating->setAnchorPoint(Vec2(0, 0.5));
            chating->setPosition(Vec2(30,vs.height / 2 -15));
            
            text->setAnchorPoint(Vec2(0, 0.5));
            text->setPosition(Vec2(baseLen + 3,vs.height / 2 -15));
            
            itemBg->setPosition(Vec2(0, -15));
        }
        
        _richText->setTouchEnabled(true);
        _richText->setSwallowTouches(false);
        
        _richText->addClickEventListener([=](Ref *ref)
                                         {
                                             PlatformHelper::playRecord(chat->content);
                                             
                                         });
    }
    else
    {
        content = chat->content;
        
        ui::RichText* _richText = ui::RichText::create();
        _richText->ignoreContentAdaptWithSize(false);
        _richText->setContentSize(Size(getChatSize().width - 300 * ss,0));
        ChatUtils::componentsSeparatedByString_roomchat(content,_richText,21,Color3B(0x4d,0x2d,0x1e));
        _richText->formatText();
        
        Size vs = _richText->getContentSize();
        
        content_layer->setContentSize(vs + Size(50,50) * ss);
        
        _richText->setAnchorPoint(Vec2(0.0, 0));
        _richText->setPosition(Vec2(15, 9));
        content_layer->addChild(_richText,1000);
        
//        LayerColor* bg = LayerColor::create(Color4B(36, 36, 36,255));
//        bg->ignoreAnchorPointForPosition(false);
//        bg->setContentSize(content_layer->getContentSize());
//        bg->setAnchorPoint(Vec2(0.0f, 0.0f));
//        bg->setPosition(Vec2::ZERO);
//        content_layer->addChild(bg, -100);
        
        auto itemBg = ui::Scale9Sprite::create("RoomPublicH/chat/chat_item_bg.png");
        itemBg->ignoreAnchorPointForPosition(false);
        itemBg->setContentSize(content_layer->getContentSize()-Size(0, 17));
        itemBg->setAnchorPoint(Vec2(0.0f, 0.0f));
        itemBg->setPosition(Vec2::ZERO);
        content_layer->addChild(itemBg, -100);
        
    }
    
    if (fromid != ZJHModel::getInstance()->uid)
    {
        Text* text = Text::create(fi->nickName, "AmericanTypewriter", 21);
        
        int allH = content_layer->getContentSize().height + text->getContentSize().height;
        
        Size itemSize = Size(size.width, allH < 100 ? 100 : allH);
        setContentSize(itemSize);
        
        text->setColor(Color3B(0xaf, 0x5a, 0x22));
        text->setAnchorPoint(Vec2(0, 1.0));
        text->setPosition(Vec2(86, itemSize.height -9));
        addChild(text,1000);
        
        //头像
        avatar->setAnchorPoint(Vec2(0, 1));
        avatar->setPosition(11, itemSize.height);
        
        content_layer->setAnchorPoint(Vec2(0, 1.0));
        content_layer->setPosition(Vec2(86, itemSize.height - text->getContentSize().height));
    }
    else
    {
        Size itemSize = Size(size.width, content_layer->getContentSize().height < 70 ? 70 : content_layer->getContentSize().height);
        setContentSize(itemSize);
        
        //头像
        if (itemSize.height > 70)
        {
            avatar->setAnchorPoint(Vec2(1, 1));
            avatar->setPosition(getChatSize().width - 11, itemSize.height);
        }
        else
        {
            avatar->setAnchorPoint(Vec2(1, 0.5));
            avatar->setPosition(getChatSize().width - 11, itemSize.height / 2);
        }
        
        content_layer->setAnchorPoint(Vec2(1, 0.5));
        content_layer->setPosition(Vec2(getChatSize().width - 86, itemSize.height / 2));
    }
}

void RoomChat2_Item::reflash(float dt)
{
    if (chatP)
    {
        if (chatP->status == MSG_STATE::MSG_STATE_SENDING)
        {
            return;
        }
        unschedule(schedule_selector(RoomChat2_Item::reflash));
        
        Text* state = (Text*)getChildByName("state");
        if (state)
        {
            if (chatP->status == MSG_STATE::MSG_STATE_SUCC) state->setString("");
            else if (chatP->status == MSG_STATE::MSG_STATE_FAIL) state->setString("[F]");
        }
    }
}

void RoomChat2_Item::initData2(long long chat,Size nChatSize)
{
    setChatSize(nChatSize);
    Size size = nChatSize;

    long long totime = chat;
    
    time_t t;
    struct tm *p;
    char buf[128];
    
    t = Utils::getCurrentTime() * 0.001;
    p = localtime(&t);
    strftime(buf, sizeof(buf), "%Y-%m-%d", p);
    std::string now_Day = buf;
    
    t = totime * 0.001;
    p = localtime(&t);
    strftime(buf, sizeof(buf), "%Y-%m-%d", p);
    std::string to_Day = buf;
    
    if (to_Day == now_Day)
    {
        strftime(buf, sizeof(buf), "%H:%M", p);
    }
    else
    {
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", p);
    }
    
    Text* text = Text::create(buf, "AmericanTypewriter", 25);
    text->setColor(Color3B(0xaf, 0x5a, 0x22));
    text->setAnchorPoint(Vec2(0.5, 0.5));
    addChild(text,1000);
    
    ui::Scale9Sprite *normalSprite2 = ui::Scale9Sprite::create("Chat/time_window.png");
    Size time_size = text->getContentSize() + Size(20,15);
    time_size.width = MAX(time_size.width,180);
    time_size.height = MAX(time_size.height,30);
    normalSprite2->setPreferredSize(time_size);
    this->addChild(normalSprite2);
    
    text->setPosition(Vec2(getChatSize().width / 2, normalSprite2->getContentSize().height / 2));
    
    normalSprite2->setPosition(getChatSize().width / 2, normalSprite2->getContentSize().height / 2);
    
//    setContentSize(Size(getChatSize().width, normalSprite2->getContentSize().height) + Size(0,20));
    setContentSize(Size(getChatSize().width, normalSprite2->getContentSize().height + 20));
}

void RoomChat2_Item::btnCallback(Ref* ref)
{
}

void RoomChat2_Item::startPlayNotify(Ref* ref)
{
    __String* str = (__String*)ref;
    if (str && chating)
    {
        std::string filename = str->getCString();
        if(filename.find(soundfile) != -1)
        {
            log("startPlayNotify");
            chating->stopAllActions();
            Animate* disappear = NULL;
            int disappearf[9] = {1,2,3,1,2,3,1,2,3};
            disappear = Utils::getCCAnimate("Chat/soundeffect/dd_left_voice_%d.png", 9, disappearf,0.2);
            chating->runAction(Repeat::create(disappear,600));
        }
    }
}

void RoomChat2_Item::stopPlayNotify(Ref* ref)
{
    __String* str = (__String*)ref;
    if (str && chating)
    {
        std::string filename = str->getCString();
        if(filename.find(soundfile) != -1)
        {
            log("stopPlayNotify");
            chating->stopAllActions();
            chating->setTexture("Chat/soundeffect/dd_left_voice_3.png");
        }
    }
}


void RoomChat2_Item::onExit()
{
    Layer::onExit();
    __NotificationCenter::getInstance()->removeAllObservers(this);
}
