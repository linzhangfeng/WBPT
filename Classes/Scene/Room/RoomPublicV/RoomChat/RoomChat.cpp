//
//  RoomChat.cpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//
#include "RoomChat.hpp"
#include "RoomChatBottom.hpp"
#include "proto.h"
#include "../../../../Model/MsgModel.h"
#include "../../../../sqlite3/DataBase.h"
#include "../../../Loading/Loading.h"
#include "../Utils/ChatUtils.hpp"

#define ss 0.67

void RoomChat::show()
{
    Size size = Director::getInstance()->getWinSize();
    setPosition(Vec2(0,-size.height));
    runAction(MoveTo::create(0.17, Vec2(0, 0)));
    setVisible(true);
}

void RoomChat::hide()
{
    Size size = Director::getInstance()->getWinSize();
    runAction(MoveTo::create(0.17, Vec2(0,-size.height)));

}

void RoomChat::showFastChatWindow(){
    if(chat_bottom->isInputEmpty()){
        chat_bottom->showFastChatPopWindow();
    }
}

bool RoomChat::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    setName("RoomChat");
    
    Size size = Director::getInstance()->getWinSize();
    
    auto bg = Layout::create();
    bg->setContentSize(size);
    bg->setName("listener");
    bg->setTouchEnabled(true);
    bg->setSwallowTouches(true);
    bg->addClickEventListener([=](Ref* rf){
        this->hide();
    });
    addChild(bg,-10000);
    
    LayerColor* content_bg = LayerColor::create(Color4B(20, 20, 22, 255));
    content_bg->setContentSize(Size(size.width,896 * ss));
    content_bg->setAnchorPoint(Vec2(0.0f, 0.0f));
    content_bg->setPosition(0,151 * ss);
    addChild(content_bg, -100);
    
    LayerColor* title_bg = LayerColor::create(Color4B(14, 14, 15, 255));
    title_bg->setContentSize(Size(size.width,111 * ss));
    title_bg->setAnchorPoint(Vec2(0.0f, 0.0f));
    title_bg->setPosition(0, content_bg->getPositionY() + content_bg->getContentSize().height);
    addChild(title_bg, 100);
    
    Button *closeBtn = Button::create("Public/icon_cancel.png");
    closeBtn->setZoomScale(0);
    closeBtn->setPosition(Vec2(size.width - 45, title_bg->getContentSize().height / 2));
    closeBtn->addClickEventListener([=](Ref *ref)
                                    {
                                        this->hide();
                                    });
    title_bg->addChild(closeBtn);
    
    Text* text = Text::create("发言", "AmericanTypewriter", 50 * ss);
    text->setColor(Color3B(194, 174, 150));
    text->setPosition(title_bg->getContentSize() / 2);
    title_bg->addChild(text,200);
    
    chat_bottom = RoomChatBottom::create();
    addChild(chat_bottom,1000);
    
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(RoomChat::notificationChat), "RoomChat_reflash", NULL);
    
    setPosition(Vec2(0,-size.height));
    return true;
}

void RoomChat::myInit(Json::Value &json)
{
    Size size = Director::getInstance()->getWinSize();
    
//    int to_id = json["uid"].asInt();
//    
//    MsgModel::getInstance()->curChatUID = to_id;
    

    
    curPage = 1;
    lastTime = 0;
    newTime = 0;
    hasMore = true;
    pageSize = 10;
    lastId = 0;
    initList();
}

void RoomChat::initList()
{
    removeChildByName("list");
    Size size = Director::getInstance()->getWinSize();
    
    ListView* listView = ListView::create();
    listView->setDirection(ui::ScrollView::Direction::VERTICAL);
    listView->setBounceEnabled(false);
    listView->setContentSize(Size(size.width, 896 * ss));
    listView->setAnchorPoint(Vec2(0, 0));
    listView->setPosition(Vec2(0, 151 * ss));
    listView->setScrollBarEnabled(false);
    listView->setName("list");
    addChild(listView);
    
    listView->setItemsMargin(20);

//    getRecordData();
//    
//    CallFunc* func = CallFunc::create([=]
//                                      {
//                                          listView->jumpToBottom();
//                                      });
//    
//    listView->refreshView();
//    
//    runAction(Sequence::create(DelayTime::create(0.0),func, NULL));
//    
//    listView->addEventListener([=](Ref *sendre, ScrollView::EventType eventType)
//                               {
//                                   if (eventType == ScrollView::EventType::SCROLL_TO_TOP) {
//                                       if (hasMore)
//                                       {
//                                           curPage ++;
//                                           getRecordData(false);
//                                       }
//                                   }
//                               });
    
}

void RoomChat::getRecordData(bool init)
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
//            RoomChat_Item* item = RoomChat_Item::create();
//            item->initData2(chat->createTime);
//            
//            auto layout = Layout::create();
//            layout->setContentSize(item->getContentSize());
//            layout->addChild(item);
//            listView->insertCustomItem(layout, 0);
//        }
//        lastTime = chat->createTime;
//        
//        RoomChat_Item* item = RoomChat_Item::create();
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

void RoomChat::notificationChat(Ref* msg)
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
        RoomChat_Item* item = RoomChat_Item::create();
        item->initData2(chat->createTime);
        
        auto layout = Layout::create();
        layout->setContentSize(item->getContentSize());
        layout->addChild(item);
        listView->addChild(layout);
    }
    
    if(listView->getChildrenCount() > 100)listView->removeItem(0);
    
    newTime = chat->createTime;
    
    RoomChat_Item* item = RoomChat_Item::create();
    item->initData(chat);
    
    auto layout = Layout::create();
    layout->setContentSize(item->getContentSize());
    layout->addChild(item);
    listView->addChild(layout);
    
    listView->refreshView();
    if(p.y >= -50)listView->jumpToBottom();
    
    lastTime = chat->createTime;
}

void RoomChat::btnCallback(Ref* ref)
{
    
}

void RoomChat::onExit()
{
    Layer::onExit();
    __NotificationCenter::getInstance()->removeAllObservers(this);
}

//---------------RoomChat_Item-------------------
bool RoomChat_Item::init()
{
    if (!Layer::init())
    {
        return false;
    }
    return true;
}

void RoomChat_Item::initData(MsgModel::Chat* chat)
{
    chatP = chat;
    
    Size size = Director::getInstance()->getWinSize();
    
    int fromid = chat->fromId;
    
    MsgModel::FriendInfo* fi = MsgModel::getInstance()->friendList[fromid];
    
    Sprite* avatar = Utils::createCircleAvatar(fi->photo, "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(144,144) * ss);
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
        
        __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(RoomChat_Item::startPlayNotify), "chat_sound_play_start_notify", NULL);
        
        __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(RoomChat_Item::stopPlayNotify), "chat_sound_play_over_notify", NULL);
        
        int time = chat->roomID;
        int baseLen = 200 * ss + time * 20;
        baseLen = MIN(baseLen,600 * ss);
        
        Layout* _richText = Layout::create();
        _richText->setContentSize(Size(baseLen,100 * ss));
        
        Size vs = _richText->getContentSize();
        
        content_layer->setContentSize(vs + Size(0,0));
        
        _richText->setAnchorPoint(Vec2(0.0, 0.0));
        _richText->setPosition(Vec2(0, 0));
        content_layer->addChild(_richText,1000);
        
        LayerColor* bg = LayerColor::create(Color4B(36, 36, 36,255));
        bg->ignoreAnchorPointForPosition(false);
        bg->setContentSize(content_layer->getContentSize());
        bg->setAnchorPoint(Vec2(0.0f, 0.0f));
        bg->setPosition(Vec2::ZERO);
        content_layer->addChild(bg, -100);
        
        Director::getInstance()->getTextureCache()->addImage("Chat/soundeffect/dd_left_voice_1.png");
        Director::getInstance()->getTextureCache()->addImage("Chat/soundeffect/dd_left_voice_2.png");
        Director::getInstance()->getTextureCache()->addImage("Chat/soundeffect/dd_left_voice_3.png");
        
        chating = Sprite::create("Chat/soundeffect/dd_left_voice_3.png");
        chating->setScale(1.7 * ss);
        content_layer->addChild(chating,1000);
        
        auto text = Text::create(__String::createWithFormat("%d\"",time)->getCString(), "AmericanTypewriter", 40 * ss);
        text->setColor(Color3B(154,154,154));
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
            chating->setPosition(Vec2(30,vs.height / 2));
            
            text->setAnchorPoint(Vec2(0, 0.5));
            text->setPosition(Vec2(baseLen + 3,vs.height / 2));
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
        _richText->setContentSize(Size(size.width - 400 * ss,0));
        ChatUtils::componentsSeparatedByString_roomchat(content,_richText);
        _richText->formatText();
        
        Size vs = _richText->getContentSize();
        
        content_layer->setContentSize(vs + Size(50,50) * ss);
        
        _richText->setAnchorPoint(Vec2(0.0, 0.0));
        _richText->setPosition(Vec2(25 * ss, 25 * ss));
        content_layer->addChild(_richText,1000);
        
        LayerColor* bg = LayerColor::create(Color4B(36, 36, 36,255));
        bg->ignoreAnchorPointForPosition(false);
        bg->setContentSize(content_layer->getContentSize());
        bg->setAnchorPoint(Vec2(0.0f, 0.0f));
        bg->setPosition(Vec2::ZERO);
        content_layer->addChild(bg, -100);
    }
    
    if (fromid != ZJHModel::getInstance()->uid)
    {
        Text* text = Text::create(fi->nickName, "AmericanTypewriter", 30);
        
        int allH = content_layer->getContentSize().height + text->getContentSize().height;
        
        Size itemSize = Size(size.width, allH < 200 * ss ? 200 * ss : allH);
        setContentSize(itemSize);
        
        text->setColor(Color3B(208, 208, 208));
        text->setAnchorPoint(Vec2(0, 1.0));
        text->setPosition(Vec2(200 * ss,itemSize.height));
        addChild(text,1000);
        
        avatar->setAnchorPoint(Vec2(0.5, 1));
        avatar->setPosition(100 * ss, itemSize.height);
        
        content_layer->setAnchorPoint(Vec2(0, 1.0));
        content_layer->setPosition(Vec2(200 * ss, itemSize.height - text->getContentSize().height));
    }
    else
    {
        Size itemSize = Size(size.width, content_layer->getContentSize().height < 200 * ss ? 200 * ss : content_layer->getContentSize().height);
        setContentSize(itemSize);
        
        if (itemSize.height > 200 * ss)
        {
            avatar->setAnchorPoint(Vec2(0.5, 1));
            avatar->setPosition(size.width - 100 * ss, itemSize.height);
        }
        else
        {
            avatar->setPosition(size.width - 100 * ss, itemSize.height / 2);
        }
        
        content_layer->setAnchorPoint(Vec2(1, 0.5));
        content_layer->setPosition(Vec2(size.width - 200 * ss, itemSize.height / 2));
    }
}

void RoomChat_Item::reflash(float dt)
{
    if (chatP)
    {
        if (chatP->status == MSG_STATE::MSG_STATE_SENDING)
        {
            return;
        }
        unschedule(schedule_selector(RoomChat_Item::reflash));
        
        Text* state = (Text*)getChildByName("state");
        if (state)
        {
            if (chatP->status == MSG_STATE::MSG_STATE_SUCC) state->setString("");
            else if (chatP->status == MSG_STATE::MSG_STATE_FAIL) state->setString("[F]");
        }
    }
}

void RoomChat_Item::initData2(long long chat)
{
    Size size = Director::getInstance()->getWinSize();

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
    
    Text* text = Text::create(buf, "AmericanTypewriter", 40 * ss);
    text->setColor(Color3B(190, 190, 190));
    text->setAnchorPoint(Vec2(0.5, 0.5));
    addChild(text,1000);
    
    ui::Scale9Sprite *normalSprite2 = ui::Scale9Sprite::create("Chat/time_window.png");
    
    Size time_size = text->getContentSize() + Size(50 * ss,50 * ss);
    time_size.width = MAX(time_size.width,180);
    time_size.height = MAX(time_size.height,50);
    normalSprite2->setPreferredSize(time_size);
    this->addChild(normalSprite2);
    
    text->setPosition(Vec2(size.width / 2, normalSprite2->getContentSize().height / 2));
    
    normalSprite2->setPosition(size.width / 2, normalSprite2->getContentSize().height / 2);
    
    setContentSize(Size(size.width, normalSprite2->getContentSize().height) + Size(0,20));
}

void RoomChat_Item::btnCallback(Ref* ref)
{
}

void RoomChat_Item::startPlayNotify(Ref* ref)
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

void RoomChat_Item::stopPlayNotify(Ref* ref)
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

void RoomChat_Item::onExit()
{
    Layer::onExit();
    __NotificationCenter::getInstance()->removeAllObservers(this);
}
