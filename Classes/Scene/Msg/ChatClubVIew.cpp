//
//  ChatClubVIew.cpp
//  ZJH
//
//  Created by mac on 2017/4/24.
//  俱乐部聊天
//

#include "ChatClubVIew.hpp"
#include "ChatFriendsBottom.hpp"
#include "proto.h"
#include "../../Model/MsgModel.h"
#include "../../sqlite3/DataBase.h"
#include "../Loading/Loading.h"
#include "../Room/RoomPublicV/Utils/ChatUtils.hpp"
#include "dialog/ClubCreateRoomDialog.hpp"

ChatClubVIew::~ChatClubVIew()
{
    MsgModel::FriendInfo* friendInfo = MsgModel::getInstance()->friendGroupList[_cuid];
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

bool ChatClubVIew::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
    setName("ChatClubVIew");
    this->chatTab = NULL;
    this->roomListTab = NULL;
    return true;
}

//初始化聊天选项卡
void ChatClubVIew::initChatView(){
    auto chat_bottom = ChatFriendsBottom::create();
    chat_bottom->setCallback([=](std::string str)
                            {
                                if (str == "create_room")
                                {
                                    PlatformHelper::showToast("创建房间");
                                    ClubCreateRoomDialog* dialog = ClubCreateRoomDialog::create();
                                    this->clubId = 1;
                                    dialog->initView(this->clubId);
                                    this->addChild(dialog);
                                }
                            });

    chat_bottom->setBottomType(1);
    this->chatTab->addChild(chat_bottom,1000);
    
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(ChatClubVIew::notificationChat), "ChatFriends_reflash", NULL);
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(ChatClubVIew::nameReflash), "ChatFriends_namereflash", NULL);
    initList();

}

//初始化房间选项卡
void ChatClubVIew::initRoomListView(){
    Size contentSize = this->roomListTab->getContentSize();
    roomNumber = Text::create("当前有0个未结束的房间", "", 42);
    roomNumber->setAnchorPoint(Vec2(0,1));
    roomNumber->setPosition(Vec2(128,contentSize.height - 150));
    roomNumber->setTextColor(Color4B(0xfb,0xfb,0xfc,255));
    this->roomListTab->addChild(roomNumber);
    
    Size size = Director::getInstance()->getWinSize();
    
    roomList = ListView::create();
    roomList->setDirection(ui::ScrollView::Direction::VERTICAL);
    roomList->setBounceEnabled(false);
    roomList->setContentSize(Size(size.width, size.height - top->getContentSize().height - getHeadH() - 100));
    roomList->setAnchorPoint(Vec2(0, 0));
    roomList->setPosition(Vec2(0, 0));
    roomList->setScrollBarEnabled(true);
    roomList->setItemsMargin(20);
    this->roomListTab->addChild(roomList);

}

void ChatClubVIew::myInit(Json::Value &json)
{
    //进入默认选择聊天选项卡
    Size size = Director::getInstance()->getWinSize();
    
    setBackground(Color4B(20, 20, 22, 255));
    
    setReturn_img("Head/title-icon1.png", "Head/title-icon1_press.png", NULL);
    
    _cuid = json["uid"].asInt();
    MsgModel::FriendInfo* fi = MsgModel::getInstance()->friendGroupList[_cuid];
    
    if (fi != NULL)
    {
        std::string name = fi->nickName;
        char buf[128];
        Utils::parseName(15, buf, name.c_str());
        setTitle(buf);
    }
    
    setMore_img("Msg/icon_nav_03.png", "Msg/icon_nav_03.png", [=](){
        Json::Value json;
        json["cludId"] = clubId;
    });
    
    MsgModel::getInstance()->curChatUID = _cuid;
    
    //初始化选项卡
    top = Layout::create();
    top->setBackGroundColorType(LAYOUT_COLOR_SOLID);
    top->setAnchorPoint(Vec2(0, 1));
    top->setContentSize(Size(this->getContentSize().width,this->getContentSize().height));
    top->setPosition(Vec2(0,this->getContentSize().height - getHeadH()));
    top->setContentSize(Size(this->getContentSize().width,110));
    top->setBackGroundColor(Color3B(0x14,0x14,0x15));
    this->addChild(top);

    chatButton = Button::create("Msg/club_chat_btn.png","Msg/club_chat_btn_press.png","Msg/club_chat_btn_press.png");
    chatButton->setAnchorPoint(Vec2(1,0));
    chatButton->setPosition(Vec2(top->getContentSize().width / 2,0));
    chatButton->setName("chatButton");
    top->addChild(chatButton);
    chatButton->setEnabled(false);
    chatButton->addClickEventListener([=](Ref* ref){
        this->switTab("chatButton");
    });
    
    roomListButton = Button::create("Msg/club_room.png","Msg/club_room_press.png","Msg/club_room_press.png");
    roomListButton->setAnchorPoint(Vec2(0,0));
    roomListButton->setPosition(Vec2(top->getContentSize().width / 2,0));
    roomListButton->setName("roomListButton");
    top->addChild(roomListButton);
    roomListButton->addClickEventListener([=](Ref* ref){
        this->switTab("roomListButton");
    });

    Size parentSize = this->getContentSize();
    this->chatTab = Layout::create();
    this->chatTab->setAnchorPoint(Vec2(0,0));
    this->chatTab->setContentSize(Size(parentSize.width,parentSize.height - getHeadH()));
    this->chatTab->setPosition(Vec2(0,0));
    this->addChild(this->chatTab);
    
    this->roomListTab = Layout::create();
    this->roomListTab->setAnchorPoint(Vec2(0,0));
    this->roomListTab->setContentSize(Size(parentSize.width,parentSize.height - getHeadH()));
    this->roomListTab->setPosition(Vec2(0,0));
    this->addChild(this->roomListTab);
    
    initChatView();
    initRoomListView();
    this->switTab("chatButton");
    initHttpRequest();
    
}

void ChatClubVIew::switTab(string tab){
    if(tab == "chatButton"){
        this->chatButton->setEnabled(false);
        this->roomListButton->setEnabled(true);
        this->roomListTab->setVisible(false);
        this->chatTab->setVisible(true);
    }else{
        this->chatButton->setEnabled(true);
        this->roomListButton->setEnabled(false);
        this->roomListTab->setVisible(true);
        this->chatTab->setVisible(false);
        //每次切换时重新获取一次数据
        requestRoomList();
    }
}

void ChatClubVIew::nameReflash(Ref* msg)
{
    MsgModel::FriendInfo* fi = MsgModel::getInstance()->friendList[_cuid];
    std::string name = fi->rmkName != "" ? fi->rmkName : fi->nickName;
    char buf[128];
    Utils::parseName(15, buf, name.c_str());
    setTitle(buf);
}

void ChatClubVIew::initList()
{
    curPage = 1;
    lastTime = 0;
    newTime = 0;
    hasMore = true;
    pageSize = 10;
    lastId = 0;
    
    this->chatTab->removeChildByName("list");
    Size size = Director::getInstance()->getWinSize();
    
    ListView* listView = ListView::create();
    listView->setDirection(ui::ScrollView::Direction::VERTICAL);
    listView->setBounceEnabled(false);
    listView->setContentSize(Size(size.width, size.height - top->getContentSize().height - getHeadH() - 180));
    listView->setAnchorPoint(Vec2(0, 0));
    listView->setPosition(Vec2(0, 160));
    listView->setScrollBarEnabled(false);
    listView->setName("list");
    this->chatTab->addChild(listView);
    
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

void ChatClubVIew::getRecordData(bool init)
{
    ListView* listView = (ListView*)this->chatTab->getChildByName("list");
    MsgModel::FriendInfo* friendInfo = MsgModel::getInstance()->friendGroupList[_cuid];
    
    if (friendInfo == NULL)return;
    
    std::vector<MsgModel::Chat*> chatList = DataBase::getInstance(ZJHModel::getInstance()->uid)->getChat(_cuid, curPage, pageSize, lastId,CONTENT_CLUB);
    
//    string jsonList = DataBase::getInstance(ZJHModel::getInstance()->uid)->getChatByJson(_cuid, curPage, pageSize, lastId,CONTENT_CLUB);
//
//    log("=========jsonList=========%s",jsonList.c_str());
    
    Vec2 p = listView->getInnerContainerPosition();
    
    if (chatList.size() < pageSize)hasMore = false;
    for (int i = 0; i < chatList.size() ; i ++)
    {
        MsgModel::Chat* chat = chatList[i];
        if (!chat)continue;
        
        ChatClub_Item* item = ChatClub_Item::create();
        item->initData(chat);
        
        auto layout = Layout::create();
        layout->setContentSize(item->getContentSize());
        layout->addChild(item,10000);
        listView->insertCustomItem(layout, 0);
        
        if (newTime == 0) newTime = chat->createTime;
        
        long long dtime = lastTime - chat->createTime;
        if ((lastTime == 0 || dtime >= 5 * 60 * 1000))
        {
            ChatClub_Item* item = ChatClub_Item::create();
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

void ChatClubVIew::notificationChat(Ref* msg)
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
    
    ListView* listView = (ListView*)this->chatTab->getChildByName("list");
    MsgModel::FriendInfo* friendInfo = MsgModel::getInstance()->friendGroupList[_cuid];
    if (!friendInfo)return;
    
    MsgModel::Chat* chat = friendInfo->chatList[friendInfo->chatList.size() - 1];
    
    long long dtime = chat->createTime - newTime;
    if (newTime == 0 || dtime >= 5 * 60 * 1000)
    {
        ChatClub_Item* item = ChatClub_Item::create();
        item->initData2(chat->createTime);
        
        auto layout = Layout::create();
        layout->setContentSize(item->getContentSize());
        layout->addChild(item);
        listView->addChild(layout);
    }
    newTime = chat->createTime;
    
    
    ChatClub_Item* item = ChatClub_Item::create();
    item->initData(chat);
    
    auto layout = Layout::create();
    layout->setContentSize(item->getContentSize());
    layout->addChild(item);
    listView->addChild(layout);
    
    listView->refreshView();
    listView->jumpToBottom();
    
    lastTime = chat->createTime;
}

void ChatClubVIew::btnCallback(Ref* ref)
{
    
}

void ChatClubVIew::initHttpRequest(){
    Json::Value jsonDate;
    jsonDate["groupId"] = _cuid;
//    jsonDate["groupId"] = 1;

    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
        CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
        
        if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)return;
        
        if (loginPacket->status != 3)
        {
            PlatformHelper::showToast("网络链接失败，请稍后再试");
            return;
        }
        
        if (loginPacket->resultIsOK())
        {
            Json::Value data = loginPacket->recvVal["resultMap"]["data"];
            std::string name = data["clubName"].asString();
            char buf[128];
            Utils::parseName(15, buf, name.c_str());
            setTitle(buf);

            this->clubId = data["groupId"].asInt();
            //创建一个俱乐部friendInfo
            MsgModel::FriendInfo* fi = new MsgModel::FriendInfo;
            fi->mid = this->clubId;
            fi->nickName = name;
            fi->photo = data["logo"].asString();
            fi->friendType = FRIEND_TYPE_GROUP;//设置为俱乐部好友类型
            fi = MsgModel::getInstance()->addFriend(fi);
        }
    },"club/clubInfo/searchClubByGroupId",jsonDate.toStyledString(),"searchClubByGroupId");
    

}

void ChatClubVIew::requestRoomList(){
    //请求未结束房间数据
    Json::Value jsonDate;
//    jsonDate["clubId"] = this->clubId;
    jsonDate["clubId"] = 1;
    jsonDate["pageSize"] = 50;
    jsonDate["pageIndex"] = 1;
    CCHttpAgent::getInstance()->sendHttpPost([=](std::string tag){
        CCHttpPacket* loginPacket = CCHttpAgent::getInstance()->packets[tag];
        
        if (this->getReferenceCount() <= 0 || this->getReferenceCount() > 10)return;
        
        if (loginPacket->status != 3)
        {
            PlatformHelper::showToast("网络链接失败，请稍后再试");
            return;
        }
        
        if (loginPacket->resultIsOK())
        {
            Json::Value data = loginPacket->recvVal["resultMap"]["list"];
            refreshRoomList(data);
        }
    },"club/clubRoom/roomList",jsonDate.toStyledString(),"roomList");
}

//刷新房间列表
void ChatClubVIew::refreshRoomList(Json::Value jsonValue){
    
    this->roomList->removeAllItems();
    roomNumber->setString(__String::createWithFormat("当前有%d个未结束的房间",jsonValue.size())->getCString());
    for(int i = 0; i < jsonValue.size(); i++){
        ChatRoom_Item* item = ChatRoom_Item::create();
        item->initView(jsonValue[i]);
        roomList->addChild(item);
    }
}

void ChatClubVIew::onExit()
{
    BaseLayer::onExit();
    CCHttpAgent::getInstance()->callbacks.erase("roomList");
}

//---------------ChatRoom_Item-------------------
bool ChatRoom_Item::init()
{
    if (!Layout::init())
    {
        return false;
    }
    return true;
}

void ChatRoom_Item::initView(Json::Value data)
{
    Size winSize = Director::getInstance()->getWinSize();
    
    ImageView* bg = ImageView::create("Msg/room_list_item.png");
    bg->setAnchorPoint(Vec2(0.5,0));
    bg->ignoreContentAdaptWithSize(false);
    bg->setScale9Enabled(true);
    bg->setContentSize(Size(833,460));
    addChild(bg);
    
    this->setContentSize(Size(winSize.width,bg->getContentSize().height));
    bg->setPosition(Vec2(winSize.width / 2,0));
    
    char buf[500];
    Utils::parseName(8, buf, data["clubPlayName"].asString().c_str());
    string gameName = __String::createWithFormat("[%s]",buf)->getCString();
    Text* text = Text::create(gameName, "", 42);
    text->setColor(Color3B(0xf1, 0xd9, 0x74));
    text->setAnchorPoint(Vec2(0, 1));
    text->setPosition(Vec2(28,bg->getContentSize().height - 39));
    bg->addChild(text);
    
    
    gameName = __String::createWithFormat("房间号:%s",data["clubRoomCode"].asString().c_str())->getCString();
    Text* roomCode = Text::create(gameName, "", 42);
    roomCode->setColor(Color3B(0xe5, 0xe5, 0xe5));
    roomCode->setAnchorPoint(Vec2(0, 1));
    roomCode->setPosition(Vec2(410,bg->getContentSize().height - 39));
    bg->addChild(roomCode);
    
    ImageView* timeIcon = ImageView::create("Msg/room_list_time_icon.png");
    timeIcon->setAnchorPoint(Vec2(0.5,0.5));
    timeIcon->setPosition(Vec2(47,347));
    bg->addChild(timeIcon);
    
    gameName = __String::createWithFormat("%s",data["roomJuDesc"].asString().c_str())->getCString();
    Text* jushu = Text::create(gameName, "", 42);
    jushu->setColor(Color3B(0xff, 0x7d, 0x02));
    jushu->setAnchorPoint(Vec2(0, 1));
    jushu->setPosition(Vec2(73,bg->getContentSize().height - 92));
    bg->addChild(jushu);
    
    ImageView* peopleIcon = ImageView::create("Msg/room_list_peple_icon.png");
    peopleIcon->setAnchorPoint(Vec2(0.5,0.5));
    peopleIcon->setPosition(Vec2(370,347));
    bg->addChild(peopleIcon);

    gameName = __String::createWithFormat("%s",data["roomCountDesc"].asString().c_str())->getCString();
    Text* renshu = Text::create(gameName, "", 42);
    renshu->setColor(Color3B(0xff, 0x7d, 0x02));
    renshu->setAnchorPoint(Vec2(0, 1));
    renshu->setPosition(Vec2(400,bg->getContentSize().height - 92));
    bg->addChild(renshu);
    
    Text* wanfaText = Text::create("玩法:", "", 42);
    wanfaText->setColor(Color3B(0xa8, 0xa8, 0xa8));
    wanfaText->setAnchorPoint(Vec2(0, 1));
    wanfaText->setPosition(Vec2(35,bg->getContentSize().height - 157));
    bg->addChild(wanfaText);
    
    auto wanfaDesc = LabelTTF::create(data["playDesc"].asString().c_str(), "", 40,
                                  Size(611, 150), TextHAlignment::LEFT);
    wanfaDesc->setColor(Color3B(0xa8, 0xa8, 0xa8));
    wanfaDesc->setAnchorPoint(Vec2(0,1));
    wanfaDesc->setPosition(Vec2(140,bg->getContentSize().height - 157));
    bg->addChild(wanfaDesc);
    
    ImageView* line = ImageView::create("Msg/room_list_line.png");
    line->setScale9Enabled(true);
    line->setAnchorPoint(Vec2(0,0.5));
    line->setContentSize(Size(bg->getContentSize().width,2));
    line->setPosition(Vec2(0,143));
    bg->addChild(line);
    
    //获取房间状态 (0未开始、1进行中、2已结束、3已解散)',
    int roomStatu = data["roomStatus"].asInt();
    Button* statuBtn = Button::create("Msg/room_list_join.png","Msg/room_list_join_press.png","Msg/room_list_manren.png");
    statuBtn->setAnchorPoint(Vec2(0.5,0.5));
    statuBtn->setPosition(Vec2(bg->getContentSize().width / 2,70));
    bg->addChild(statuBtn);
    statuBtn->addClickEventListener([=](Ref*){
        string code = data["clubRoomCode"].asString();
        ZJHModel::getInstance()->gotoRoomWithCode(code,false);
    });
    if(roomStatu == 2 || roomStatu == 3){
        statuBtn->setEnabled(false);
    }
}

//---------------ChatFriends_Item-------------------
bool ChatClub_Item::init()
{
    if (!Layer::init())
    {
        return false;
    }
    return true;
}

void ChatClub_Item::initData(MsgModel::Chat* chat)
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
        
        __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(ChatClub_Item::startPlayNotify), "chat_sound_play_start_notify", NULL);
        
        __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(ChatClub_Item::stopPlayNotify), "chat_sound_play_over_notify", NULL);
        
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
        ChatUtils::componentsSeparatedByString_roomchat(content,_richText);

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
        this->schedule(schedule_selector(ChatClub_Item::reflash), 0.0f);
    }
}

void ChatClub_Item::reflash(float dt)
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
        
        unschedule(schedule_selector(ChatClub_Item::reflash));
        
        Text* state = (Text*)getChildByName("state");
        if (state)
        {
            if (chatP->status == MSG_STATE::MSG_STATE_SUCC) state->setString("");
            else if (chatP->status == MSG_STATE::MSG_STATE_FAIL) state->setString("[F]");
        }
    }
}

void ChatClub_Item::initData2(long long chat)
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

void ChatClub_Item::btnCallback(Ref* ref)
{
}

void ChatClub_Item::startPlayNotify(Ref* ref)
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

void ChatClub_Item::stopPlayNotify(Ref* ref)
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

void ChatClub_Item::onExit()
{
    Layer::onExit();
    __NotificationCenter::getInstance()->removeAllObservers(this);
}

