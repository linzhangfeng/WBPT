//
//  ChatFriends.cpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//
#include "ChatFriends.hpp"
#include "ChatFriendsBottom.hpp"
#include "proto.h"
#include "../../Model/MsgModel.h"
#include "../../sqlite3/DataBase.h"
#include "../Loading/Loading.h"

int getFaceIndex(std::string faceText)
{
    int result = -1;
    if (faceText == "[呵呵]") result = 0;
    else if (faceText == "[嘻嘻]") result = 1;
    else if (faceText == "[哈哈]") result = 2;
    else if (faceText == "[晕]") result = 3;
    else if (faceText == "[泪]") result = 4;
    else if (faceText == "[馋嘴]") result = 5;
    else if (faceText == "[抓狂]") result = 6;
    else if (faceText == "[哼]") result = 7;
    else if (faceText == "[可爱]") result = 8;
    else if (faceText == "[怒]") result = 9;
    else if (faceText == "[困]") result = 10;
    else if (faceText == "[汗]") result = 11;
    else if (faceText == "[睡觉]") result = 12;
    else if (faceText == "[偷笑]") result = 13;
    else if (faceText == "[吃惊]") result = 15;
    else if (faceText == "[闭嘴]") result = 16;
    else if (faceText == "[花心]") result = 17;
    else if (faceText == "[失望]") result = 18;
    else if (faceText == "[生病]") result = 19;
    else if (faceText == "[亲亲]") result = 20;
    else if (faceText == "[右哼哼]") result = 21;
    else if (faceText == "[嘘]") result = 22;
    else if (faceText == "[挤眼]") result = 23;
    else if (faceText == "[酷]") result = 24;
    else if (faceText == "[感冒]") result = 25;
    else if (faceText == "[做鬼脸]") result = 26;
    else if (faceText == "[阴险]") result = 27;
    else if (faceText == "[热吻]") result = 28;
    else if (faceText == "[心]") result = 29;
    else if (faceText == "[ok]") result = 30;
    else if (faceText == "[不要]") result = 31;
    else if (faceText == "[弱]") result = 32;
    else if (faceText == "[good]") result = 33;
    else if (faceText == "[拳头]") result = 34;
    else if (faceText == "[耶]") result = 35;
    else if (faceText == "[0]") result = 36;
    else if (faceText == "[1]") result = 37;
    else if (faceText == "[2]") result = 38;
    else if (faceText == "[3]") result = 39;
    else if (faceText == "[4]") result = 40;
    else if (faceText == "[5]") result = 41;
    else if (faceText == "[6]") result = 42;
    else if (faceText == "[7]") result = 43;
    else if (faceText == "[8]") result = 44;
    else if (faceText == "[9]") result = 45;
    return result;
}

__Array* componentsSeparatedByString(std::string str,ui::RichText* _richText)
{
    __Array* result = __Array::create();
    const char* str_tmp = str.c_str();
    int l = strlen(str_tmp);
    int beginIndex = -1;
    int lastIndex = 0;
    for(int i = 0; i < l;i++)
    {
        if(str_tmp[i] == '[')
        {
            beginIndex = i;
        }
        else if (str_tmp[i] == ']')
        {
            if (beginIndex != -1)
            {
                if (beginIndex - lastIndex > 0)result->addObject(__String::create(str.substr(lastIndex, beginIndex - lastIndex)));
                
                if (i + 1 - beginIndex > 0)result->addObject(__String::create(str.substr(beginIndex, i + 1 - beginIndex)));
                lastIndex = i + 1;
                beginIndex = -1;
            }
        }
    }
    
    if (lastIndex < l)
    {
        result->addObject(__String::create(str.substr(lastIndex, l - lastIndex)));
    }
    
    int label_count=(int)result->count();
    for (int i=0; i< label_count; i++)
    {
        __String * _string = (__String*)result->getObjectAtIndex(i);
//        log("%s",_string->getCString());
        std::string s = _string->getCString();
        int faceIndex = getFaceIndex(s);
        if (faceIndex != -1)
        {
            RichElementImage * reimg = RichElementImage::create(i, Color3B::WHITE, 255,__String::createWithFormat("Chat/face/tt_e%d.png",faceIndex)->getCString());
            _richText->pushBackElement(reimg);
        }
        else
        {
            auto _element_text = RichElementText::create(i, Color3B(154,154,154), 255, s.c_str(), "AmericanTypewriter", 50);
            _richText->pushBackElement(_element_text);
        }

    }
    return result;
}

ChatFriends::~ChatFriends()
{
    MsgModel::FriendInfo* friendInfo = MsgModel::getInstance()->friendList[_cuid];
    if (friendInfo)
    {
        for (long i = friendInfo->chatList.size() - 1; i >= 0 ; i --)
        {
            MsgModel::Chat* chat = friendInfo->chatList[i];
            if (!chat)continue;
            delete chat;
        }
        friendInfo->chatList.clear();
    }
    
    MsgModel::getInstance()->curChatUID = 0;
    __NotificationCenter::getInstance()->postNotification("Msg_reflash");
}

bool ChatFriends::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    setName("ChatFriends");
    
    return true;
}

void ChatFriends::myInit(Json::Value &json)
{
    Size size = Director::getInstance()->getWinSize();
    
    setBackground(Color4B(20, 20, 22, 255));
    
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", NULL);
    
    _cuid = json["uid"].asInt();
    MsgModel::FriendInfo* fi = MsgModel::getInstance()->friendList[_cuid];
    
    if (fi == NULL)
    {
        log("error fi is NULL id[%d]",_cuid);
        return;
    }
    
    std::string name = fi->rmkName != "" ? fi->rmkName : fi->nickName;
    char buf[128];
    Utils::parseName(15, buf, name.c_str());
    setTitle(buf);
    
    setMore_img("Msg/icon_nav_03.png", "Msg/icon_nav_03.png", [=](){
        Json::Value json;
        json["uid"] = MsgModel::getInstance()->curChatUID;
        ZJHModel::getInstance()->gotoView(VIEW_FRIENDS_INFO,json);
    });
    
    fi->unReadNums = 0;
    
    MsgModel::getInstance()->updateFriendInfo(fi);
    
    MsgModel::getInstance()->curChatUID = _cuid;
    
    auto chat_bottom = ChatFriendsBottom::create();
    chat_bottom->setBottomType(0);
    addChild(chat_bottom,1000);
    
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(ChatFriends::notificationChat), "ChatFriends_reflash", NULL);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(ChatFriends::nameReflash), "ChatFriends_namereflash", NULL);
    initList();
}

void ChatFriends::nameReflash(Ref* msg)
{
    MsgModel::FriendInfo* fi = MsgModel::getInstance()->friendList[_cuid];
    std::string name = fi->rmkName != "" ? fi->rmkName : fi->nickName;
    char buf[128];
    Utils::parseName(15, buf, name.c_str());
    setTitle(buf);
}

void ChatFriends::initList()
{
    curPage = 1;
    lastTime = 0;
    newTime = 0;
    hasMore = true;
    pageSize = 10;
    lastId = 0;
    
    removeChildByName("list");
    Size size = Director::getInstance()->getWinSize();
    
    ListView* listView = ListView::create();
    listView->setDirection(ui::ScrollView::Direction::VERTICAL);
    listView->setBounceEnabled(false);
    listView->setContentSize(Size(size.width, size.height - 150 - 160));
    listView->setAnchorPoint(Vec2(0, 0));
    listView->setPosition(Vec2(0, 160));
    listView->setScrollBarEnabled(false);
    listView->setName("list");
    addChild(listView);
    
    listView->setItemsMargin(50);

    getRecordData();
    
    CallFunc* func = CallFunc::create([=]
                                      {
                                          listView->jumpToBottom();
                                      });
    
    listView->refreshView();
    
    runAction(Sequence::create(DelayTime::create(0.0),func, NULL));
    
    listView->addEventListener([=](Ref *sendre, ScrollView::EventType eventType)
                               {
                                   if (eventType == ScrollView::EventType::SCROLL_TO_TOP) {
                                       if (hasMore)
                                       {
                                           curPage ++;
                                           getRecordData(false);
                                       }
                                   }
                               });
    
}

void ChatFriends::getRecordData(bool init)
{
    ListView* listView = (ListView*)getChildByName("list");
    MsgModel::FriendInfo* friendInfo = MsgModel::getInstance()->friendList[_cuid];
    
    if (friendInfo == NULL)return;
    
    std::vector<MsgModel::Chat*> chatList = DataBase::getInstance(ZJHModel::getInstance()->uid)->getChat(_cuid, curPage, pageSize, lastId);

    Vec2 p = listView->getInnerContainerPosition();
    
    if (chatList.size() < pageSize)hasMore = false;
    for (int i = 0; i < chatList.size() ; i ++)
    {
        MsgModel::Chat* chat = chatList[i];
        if (!chat)continue;
        
        ChatFriends_Item* item = ChatFriends_Item::create();
        item->initData(chat);
        
        auto layout = Layout::create();
        layout->setContentSize(item->getContentSize());
        layout->addChild(item,10000);
        listView->insertCustomItem(layout, 0);
        
        if (newTime == 0) newTime = chat->createTime;
        
        long long dtime = lastTime - chat->createTime;
        if ((lastTime == 0 || dtime >= 5 * 60 * 1000))
        {
            ChatFriends_Item* item = ChatFriends_Item::create();
            item->initData2(chat->createTime);
            
            auto layout = Layout::create();
            layout->setContentSize(item->getContentSize());
            layout->addChild(item,1000);
            listView->insertCustomItem(layout, 0);
        }
        lastTime = chat->createTime;
        
        friendInfo->chatList.push_back(chat);//for delete this object in the view onExit
        
        if (i == chatList.size() - 1)
        {
            lastId = chat->id;
        }
    }
    
    if (!init)
    {
        listView->refreshView();
        listView->setInnerContainerPosition(p);
    }
}

void ChatFriends::notificationChat(Ref* msg)
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
    MsgModel::FriendInfo* friendInfo = MsgModel::getInstance()->friendList[_cuid];
    if (!friendInfo)return;
    
    MsgModel::Chat* chat = friendInfo->chatList[friendInfo->chatList.size() - 1];
    
    long long dtime = chat->createTime - newTime;
    if (newTime == 0 || dtime >= 5 * 60 * 1000)
    {
        ChatFriends_Item* item = ChatFriends_Item::create();
        item->initData2(chat->createTime);
        
        auto layout = Layout::create();
        layout->setContentSize(item->getContentSize());
        layout->addChild(item);
        listView->addChild(layout);
    }
    newTime = chat->createTime;
    
    
    ChatFriends_Item* item = ChatFriends_Item::create();
    item->initData(chat);
    
    auto layout = Layout::create();
    layout->setContentSize(item->getContentSize());
    layout->addChild(item);
    listView->addChild(layout);
    
    listView->refreshView();
    listView->jumpToBottom();
    
    lastTime = chat->createTime;
}

void ChatFriends::btnCallback(Ref* ref)
{
    
}

void ChatFriends::onExit()
{
    BaseLayer::onExit();
}

//---------------ChatFriends_Item-------------------
bool ChatFriends_Item::init()
{
    if (!Layer::init())
    {
        return false;
    }
    return true;
}

void ChatFriends_Item::initData(MsgModel::Chat* chat)
{
    chatP = chat;
    
    imgLoading = NULL;
    sending = NULL;
    chating = NULL;
    
    Size size = Director::getInstance()->getWinSize();
    
    int fromid = chat->fromId;
    
    MsgModel::FriendInfo* fi = MsgModel::getInstance()->friendList[fromid];
    
    if (fi == NULL)
    {
        log("error fi is NULL id[%d]",fromid);
        return;
    }
    
    Sprite* avatar = Utils::createCircleAvatar(fi->photo, "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(144,144));
    this->addChild(avatar);
    
    Layer* content_layer = Layer::create();
    content_layer->ignoreAnchorPointForPosition(false);
    addChild(content_layer);
    
    std::string content;
    if (chat->msgType == MESSAGE_TYPE::MSG_TYPE_IMAGE)
    {
        content = "[图片]";
        
        Size maxSize = Size(640, 400);
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
            image->loadTexture("Hall/pic_default2.png");
            image->adaptRenderers();
            
            content_layer->setContentSize(image->getContentSize());
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
                                             Json::Value json;
                                             json["img"] = chat->content;
                                             ZJHModel::getInstance()->gotoView(VIEW_SHOW_IMAGE,json);
                                         }

                                     });
        
        if (chat->status == MSG_STATE::MSG_STATE_SENDING)
        {
            imgLoading = Loading::startLoading(image,image->getContentSize() / 2);
        }
    }
    else if (chat->msgType == MESSAGE_TYPE::MSG_TYPE_SYSTEM_AGAINST_INVITE)
    {
        /*
         {"bottom":1,"disPlayMode":1,"duration":120,"invite":"邀请您加入炸金花对局房间：玩呗101379的对局房间","number":5,"roomId":1196,"sendId":101379,"sendName":"玩呗101379","sendPhoto":"/chess_img/head/default/00.jpg","takeIn":500,"timestamp":1469446056933}
         */
        content = chat->content;
        Json::Value json = Utils::ParseJsonStr(content);
        
        content_layer->setContentSize(Size(804,369));
        
        LayerColor* bg = LayerColor::create(Color4B(36, 36, 36,255));
        bg->ignoreAnchorPointForPosition(false);
        bg->setContentSize(content_layer->getContentSize());
        bg->setAnchorPoint(Vec2(0.0f, 0.0f));
        bg->setPosition(Vec2::ZERO);
        content_layer->addChild(bg, -100);
        
        char buf[128];
        Utils::parseName(17, buf, json["invite"].asCString());
        Text* text = Text::create(buf, "AmericanTypewriter", 40);
        text->setColor(Color3B(205, 185, 160));
        text->setAnchorPoint(Vec2(0, 0.5));
        text->setPosition(Vec2(29,308));
        content_layer->addChild(text,1000);
        int type = json["type"].asInt();//0时间   1局数
        string formatStr = "人数和时长：%d人       %d分钟";
        if(type == 1){
           formatStr = "人数和时长：%d人       %d局";
        }
        text = Text::create(__String::createWithFormat(formatStr.c_str(),json["number"].asInt(),json["duration"].asInt())->getCString(), "AmericanTypewriter", 40);
        text->setColor(Color3B(205, 185, 160));
        text->setAnchorPoint(Vec2(0, 0.5));
        text->setPosition(Vec2(29,227));
        content_layer->addChild(text,1000);
        
        text = Text::create(__String::createWithFormat("底注和带入：底注%d       初始带入%d",json["bottom"].asInt(),json["takeIn"].asInt())->getCString(), "AmericanTypewriter", 40);
        text->setColor(Color3B(205, 185, 160));
        text->setAnchorPoint(Vec2(0, 0.5));
        text->setPosition(Vec2(29,175));
        content_layer->addChild(text,1000);
        
        auto btn = Button::create("Msg/btn_yellow.png");
        btn->setScale9Enabled(true);
        btn->setContentSize(Size(Size(261, 81)));
        btn->setTitleText("进入房间");
        btn->setTitleFontSize(40);
        btn->setTitleColor(Color3B(110,69,25));
        btn->setPosition(Vec2(content_layer->getContentSize().width / 2,67));
        btn->setVisible(true);
        btn->setName("send_msg");
        content_layer->addChild(btn,1000);
        btn->addClickEventListener([=](Ref *ref)
                                   {
                                       ZJHModel::getInstance()->gotoRoom(json["roomId"].asInt());
                                   });
    }
    else if (chat->msgType == MESSAGE_TYPE::MSG_TYPE_AUDIO)
    {
        soundfile = Utils::getSoundSavePath(chat->content);
        
        __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(ChatFriends_Item::startPlayNotify), "chat_sound_play_start_notify", NULL);
        
        __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(ChatFriends_Item::stopPlayNotify), "chat_sound_play_over_notify", NULL);
        
        int time = chat->roomID;
        int baseLen = 200 + time * 20;
        baseLen = MIN(baseLen,600);
        
        Layout* _richText = Layout::create();
        _richText->setContentSize(Size(baseLen,100));
        
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
        chating->setScale(1.7);
        content_layer->addChild(chating,1000);
        
        auto text = Text::create(__String::createWithFormat("%d\"",time)->getCString(), "AmericanTypewriter", 40);
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
        _richText->setContentSize(Size(size.width - 400,0));
        componentsSeparatedByString(content,_richText);
        _richText->formatText();
        
        Size vs = _richText->getContentSize();
        
        content_layer->setContentSize(vs + Size(50,50));
        
        _richText->setAnchorPoint(Vec2(0.0, 0.0));
        _richText->setPosition(Vec2(25, 25));
        content_layer->addChild(_richText,1000);
        
        LayerColor* bg = LayerColor::create(Color4B(36, 36, 36,255));
        bg->ignoreAnchorPointForPosition(false);
        bg->setContentSize(content_layer->getContentSize());
        bg->setAnchorPoint(Vec2(0.0f, 0.0f));
        bg->setPosition(Vec2::ZERO);
        content_layer->addChild(bg, -100);
    }
    
    Size itemSize = Size(size.width, content_layer->getContentSize().height < 200 ? 200 : content_layer->getContentSize().height);
    setContentSize(itemSize);
    
    if (fromid != ZJHModel::getInstance()->uid)
    {
        if (itemSize.height > 200)
        {
            avatar->setAnchorPoint(Vec2(0.5, 1));
            avatar->setPosition(100, itemSize.height);
        }
        else
        {
            avatar->setPosition(100, itemSize.height / 2);
        }
        
        content_layer->setAnchorPoint(Vec2(0, 0.5));
        content_layer->setPosition(Vec2(200, itemSize.height / 2));
    }
    else
    {
        if (itemSize.height > 200)
        {
            avatar->setAnchorPoint(Vec2(0.5, 1));
            avatar->setPosition(size.width - 100, itemSize.height);
        }
        else
        {
            avatar->setPosition(size.width - 100, itemSize.height / 2);
        }
        
        content_layer->setAnchorPoint(Vec2(1, 0.5));
        content_layer->setPosition(Vec2(size.width - 200, itemSize.height / 2));
    }
    
    if (chat->status == MSG_STATE::MSG_STATE_SENDING)
    {
        this->schedule(schedule_selector(ChatFriends_Item::reflash), 0.0f);
    }
}

void ChatFriends_Item::reflash(float dt)
{
    if (chatP)
    {
        if (chatP->status == MSG_STATE::MSG_STATE_SENDING)
        {
            return;
        }
        if (imgLoading)
        {
            imgLoading->removeFromParentAndCleanup(true);
            imgLoading = NULL;
        }
        
        unschedule(schedule_selector(ChatFriends_Item::reflash));
        
        Text* state = (Text*)getChildByName("state");
        if (state)
        {
            if (chatP->status == MSG_STATE::MSG_STATE_SUCC) state->setString("");
            else if (chatP->status == MSG_STATE::MSG_STATE_FAIL) state->setString("[F]");
        }
    }
}

void ChatFriends_Item::initData2(long long chat)
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
    
    Text* text = Text::create(buf, "AmericanTypewriter", 40);
    text->setColor(Color3B(190, 190, 190));
    text->setAnchorPoint(Vec2(0.5, 0.5));
    addChild(text,1000);
    
    ui::Scale9Sprite *normalSprite2 = ui::Scale9Sprite::create("Chat/time_window.png");
    
    Size time_size = text->getContentSize() + Size(50,50);
    time_size.width = MAX(time_size.width,180);
    time_size.height = MAX(time_size.height,50);
    normalSprite2->setPreferredSize(time_size);
    this->addChild(normalSprite2);
    
    text->setPosition(Vec2(size.width / 2, normalSprite2->getContentSize().height / 2));
    
    normalSprite2->setPosition(size.width / 2, normalSprite2->getContentSize().height / 2);
    
    setContentSize(Size(size.width, normalSprite2->getContentSize().height));
}

void ChatFriends_Item::btnCallback(Ref* ref)
{
}

void ChatFriends_Item::startPlayNotify(Ref* ref)
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

void ChatFriends_Item::stopPlayNotify(Ref* ref)
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

void ChatFriends_Item::onExit()
{
    Layer::onExit();
    __NotificationCenter::getInstance()->removeAllObservers(this);
}
