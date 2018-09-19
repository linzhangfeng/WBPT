//
//  Msg.cpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//
#include "ChatSession.hpp"
#include "ZJH.h"
#include "../../sqlite3/DataBase.h"
#include "PlayWebLayerMsg.hpp"

bool ChatSession::init()
{
    if (!BaseLayer::init())
    {
        return false;
    }
    
//    setName("Msg");
//    
//    Size size = Director::getInstance()->getWinSize();
//    
//    setBackground(Color4B(14, 14, 15, 255));
//
//    setTitle("社区");
//    
//    setMore_txt("通信录", [=](){
//        
//        ZJHModel::getInstance()->gotoView(VIEW_MY_FRIENDS);
//    });
    hasUpdate = false;
    __NotificationCenter::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(ChatSession::notificationReflash), "Msg_reflash", NULL);
    lastReflashTime = Utils::getCurrentTime();
    initList();
    this->schedule(schedule_selector(ChatSession::updateReflash), 1.0f);
    return true;
}

void ChatSession::updateReflash(float dt)
{
    if (!hasUpdate)return;
    initList();
    hasUpdate = false;
}

void ChatSession::notificationReflash(Ref* msg)
{
    hasUpdate = true;
//    if (Utils::getCurrentTime() - lastReflashTime < 5*1000) return;
//    initList();
}

bool sortByTime(const MsgModel::Session* chat1,const MsgModel::Session* chat2)
{
    if (chat1->createTime == -1)return true;
    if (chat2->createTime == -1)return false;
    return chat1->createTime > chat2->createTime;
}

void ChatSession::initList()
{
    curLayout = NULL;
    removeChildByName("list");
    removeChildByName("tip");
    Size size = Director::getInstance()->getWinSize();
    
    long long ss = MsgModel::getInstance()->sessionList.size();
    if (ss == 0)
    {
        Text* text = Text::create("你还没有跟其他人聊天", "AmericanTypewriter", 35);
        text->setColor(Color3B(150, 150, 150));
        text->setPosition(Vec2(size.width / 2,size.height / 2));
        text->setName("tip");
        addChild(text);
        return;
    }
    
    ListView* listView = ListView::create();
    listView->setDirection(ui::ScrollView::Direction::VERTICAL);
    listView->setBounceEnabled(false);
    listView->setContentSize(Size(size.width, size.height - 270 - 160));
    listView->setAnchorPoint(Vec2(0, 0));
    listView->setPosition(Vec2(0, 160));
    listView->setScrollBarEnabled(false);
    listView->setName("list");
    addChild(listView);
    
    listView->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type)
                                    {
                                        if (type == Widget::TouchEventType::ENDED || type == Widget::TouchEventType::CANCELED)
                                        {
                                            if (curLayout)
                                            {
                                                curLayout->setPositionX(0);
                                                curLayout = NULL;
                                            }
                                        }
                                    });
    
    std::vector<MsgModel::Session*> session_v;
    
    std::map<int,MsgModel::Session*>::iterator it;
    for(it = MsgModel::getInstance()->sessionList.begin();it != MsgModel::getInstance()->sessionList.end(); it ++)
    {
        MsgModel::Session* session = it->second;
        if (session)session_v.push_back(session);
    }
    //此处添加数据库以外的session会话
    
    log("ChatSession session_v size0[%ld]",session_v.size());
    if (session_v.size() > 1)sort(session_v.begin(), session_v.end(),sortByTime);
    log("ChatSession session_v size1[%ld]",session_v.size());
    for(int i = 0;i < session_v.size(); i ++)
    {
        MsgModel::Session* session = session_v[i];
        
        ChatSessionItem* item = ChatSessionItem::create();
        
        item->initData(session,[=](int index){
            DataBase::getInstance(ZJHModel::getInstance()->uid)->deleteSession(session);
            MsgModel::getInstance()->sessionList.erase(session->mid);
            delete session;
            listView->removeItem(index);
            notificationReflash(NULL);
            listView->refreshView();
            curLayout = NULL;
        },i);
        
        auto layout = Layout::create();
        layout->setTouchEnabled(true);
        layout->setSwallowTouches(true);
        layout->setTag(session->mid);
        layout->setContentSize(item->getContentSize());
        layout->addChild(item);
        layout->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type)
                                      {
                                          if (type == Widget::TouchEventType::ENDED || type == Widget::TouchEventType::CANCELED)
                                          {
                                              Layout* n = (Layout*)sender;
                                              
                                              if (curLayout && n != curLayout)
                                              {
                                                  curLayout->setPositionX(0);
                                                  curLayout = NULL;
                                              }
                                              
                                              Vec2 dvec2 = n->getTouchBeganPosition() - n->getTouchEndPosition();
                                              if (dvec2.length() < 30)
                                              {
                                                  if (curLayout)
                                                  {
                                                      curLayout->setPositionX(0);
                                                      curLayout = NULL;
                                                  }
                                                  
                                                  int tag = n->getTag();
                                                  MsgModel::Session* s = MsgModel::getInstance()->sessionList[tag];
                                                  if(s == NULL)
                                                  {
                                                      return;
                                                  }
                                                  if (s->type == MESSAGE_TYPE::SESSION_FIND_NEW_FRIENDS)
                                                  {
                                                      
                                                  }
                                                  else if (s->type == MESSAGE_TYPE::SESSION_PAIJU_DONGTAI)
                                                  {
                                                      Json::Value json;
                                                      json["uid"] = s->mid;
                                                      ZJHModel::getInstance()->gotoView(VIEW_PAIJU_DONGTAI,json);
                                                      //将未读数 清零
                                                      MsgModel::FriendInfo* fi = MsgModel::getInstance()->friendList[s->mid];
                                                      fi->unReadNums = 0;
                                                      MsgModel::getInstance()->updateFriendInfo(fi);
                                                      item->updateView();
                                                  }
                                                  else if (s->type == MESSAGE_TYPE::SESSION_SYSTEM_WEB_MESSAGE)
                                                  {
                                                      PlayWebLayerMsg *web = PlayWebLayerMsg::create();
                                                      Director::getInstance()->getRunningScene()->addChild(web, 1000);
                                                      web->showView("系统消息", CCHttpAgent::getInstance()->url +
                                                                    s->content);
                                                      
                                                      //将未读数 清零
                                                      MsgModel::FriendInfo* fi = MsgModel::getInstance()->friendList[s->mid];
                                                      fi->unReadNums = 0;
                                                      MsgModel::getInstance()->updateFriendInfo(fi);
                                                      item->updateView();
                                                  }
                                                  else if (s->type == MESSAGE_TYPE::SESSION_POINT_MESSAGE)
                                                  {
                                                      Json::Value json;
                                                      json["uid"] = s->mid;
                                                      ZJHModel::getInstance()->gotoView(VIEW_CHAT_FRIENDS,json);
                                                      
                                                      item->updateView();
                                                  }
                                                  else if(s->type == SESSION_TRIBLE_GROUP_MESSAGE){
                                                      //俱乐部群消息
                                                      Json::Value json;
                                                      json["uid"] = s->mid;
//                                                      ZJHModel::getInstance()->gotoView(VIEW_CHAT_CLUB,json);
                                                      
                                                      Json::Value eventJsos;
                                                      eventJsos["event"] = 101;
                                                      eventJsos["content"] = json;
                                                      __NotificationCenter::getInstance()->postNotification("js_module_notify",__String::create(eventJsos.toStyledString()));
                                                      //将未读数 清零
                                                      MsgModel::FriendInfo* fi = MsgModel::getInstance()->friendGroupList[s->mid];
                                                      fi->unReadNums = 0;
                                                      MsgModel::getInstance()->updateFriendInfo(fi);
                                                      item->updateView();
                                                  }
                                                  else if(s->type == MESSAGE_TYPE::SESSION_AGAINST_NOTIFY){
                                                      Json::Value json;
                                                      json["uid"] = s->mid;
                                                      ZJHModel::getInstance()->gotoView(VIEW_AGAINST_NOTIFY,json);
                                                      //将未读数 清零
                                                      MsgModel::FriendInfo* fi = MsgModel::getInstance()->friendList[s->mid];
                                                      fi->unReadNums = 0;
                                                      MsgModel::getInstance()->updateFriendInfo(fi);
                                                      item->updateView();
                                                  }else if(s->type == MESSAGE_TYPE::SESSION_ROOM_CHARG_LOG){
                                                      Json::Value json;
                                                      json["uid"] = s->mid;
                                                      ZJHModel::getInstance()->gotoView(VIEW_ROOM_CHARGE,json);
                                                      //将未读数 清零
                                                      MsgModel::FriendInfo* fi = MsgModel::getInstance()->friendList[s->mid];
                                                      fi->unReadNums = 0;
                                                      MsgModel::getInstance()->updateFriendInfo(fi);
                                                      item->updateView();
                                                  }else if(s->type == MESSAGE_TYPE::SESSION_JOIN_TRIBLE_REQUEST){
                                                      Json::Value json;
                                                      json["uid"] = s->mid;
                                                      ZJHModel::getInstance()->gotoView(VIEW_TRIBLE_JOIN_REQUEST,json);
                                                      //将未读数 清零
                                                      MsgModel::FriendInfo* fi = MsgModel::getInstance()->friendList[s->mid];
                                                      fi->unReadNums = 0;
                                                      MsgModel::getInstance()->updateFriendInfo(fi);
                                                      item->updateView();
                                                  }
                                              }
                                              else
                                              {
                                                  if(dvec2.x > 0)
                                                  {
                                                      if (abs(dvec2.y) > abs(dvec2.x) || curLayout == n)return;
                                                      
                                                      n->setPositionX(-160);
                                                      
                                                      this->curLayout = n;
                                                  }
                                                  else
                                                  {
                                                      if (curLayout)
                                                      {
                                                          curLayout->setPositionX(0);
                                                          curLayout = NULL;
                                                      }
                                                  }
                                              }
                                          }
                                      });
        listView->addChild(layout);
    }
    
    listView->setItemsMargin(5);
    listView->refreshView();
}

//----------------------------ChatSessionItem------------------------------------
bool ChatSessionItem::init()
{
    if (!Layer::init())
    {
        return false;
    }
    return true;
}

void ChatSessionItem::initData(MsgModel::Session* session,const MyCallBack_Int &callback,int index)
{
    this->session = session;
    
    Size size = Director::getInstance()->getWinSize();
    
    setContentSize(Size(size.width, 222));
    
    Size itemSize = getContentSize();
    
    LayerColor* bg = LayerColor::create(Color4B(50, 50, 50, 255));
    bg->setContentSize(Size(itemSize.width,2));
    bg->setAnchorPoint(Vec2(0.0f, 0.0f));
    bg->setPosition(30,0);
    addChild(bg, -100);
    
    {
        LayerColor* del_bg = LayerColor::create(Color4B(178, 0, 14, 255));
        del_bg->setContentSize(Size(162,220));
        
        auto layout = Layout::create();
        layout->setTag(session->mid);
        layout->setContentSize(del_bg->getContentSize());
        layout->setTouchEnabled(true);
        layout->setSwallowTouches(true);
        layout->setAnchorPoint(Vec2(0.0f, 0.0f));
        layout->setPosition(Vec2(size.width,1));
        layout->addClickEventListener([=](Ref* sender)
                                    {
                                        layout->setTouchEnabled(false);
                                        Layout* n = (Layout*)sender;
                                        Vec2 dvec2 = n->getTouchBeganPosition() - n->getTouchEndPosition();
                                        if (dvec2.length() < 10)
                                        {
                                            if(callback)callback(index);
                                        }
                                    });
        addChild(layout);
        
        del_bg->setAnchorPoint(Vec2(0.0f, 0.0f));
        del_bg->setPosition(0,0);
        layout->addChild(del_bg, -100);
        
        Text* text = Text::create("删除", "AmericanTypewriter", 45);
        text->setColor(Color3B(208, 208, 208));
        text->setAnchorPoint(Vec2(0.5, 0.5));
        text->setPosition(del_bg->getContentSize() / 2);
        del_bg->addChild(text,1000);
    }
    if(session->type == MESSAGE_TYPE::SESSION_ROOM_CHARG_LOG){
        std::string name = "充值记录";  //现在有解散房间的消息和人满的消息 type为1是解散房间的消息  没有则为满人的消息
        std::string content;
        content = session->content;
        Json::Value json = Utils::ParseJsonStr(content);
        std::string tipsContent;
        tipsContent = json["content"].asString();
        renderItem("Msg/room_charg_icon.png",name, tipsContent, MESSAGE_TYPE::SESSION_ROOM_CHARG_LOG, session->createTime);
    }else if(session->type == MESSAGE_TYPE::SESSION_SYSTEM_WEB_MESSAGE){
        std::string name = "系统消息";
        renderItem("Msg/im_web_icon.png",name, "有新的消息,点击查看", 10004, session->createTime);
    }else if(session->type == MESSAGE_TYPE::SESSION_PAIJU_DONGTAI){
        std::string name = "动态通知";
        std::string content;
        content = session->content;
        Json::Value json = Utils::ParseJsonStr(content);
        int type = json["type"].asInt();
        if(type == 24){
            content = "你被踢出俱乐部" + json["clubName"].asString();
        }if(type == 25){
            content = "你被拒绝加入俱乐部" + json["clubName"].asString();
        }if(type == 26){
            content = "你被批准加入俱乐部" + json["clubName"].asString();
        }else{
            content = "您有新动态";
        }
        renderItem("Msg/im_dynamic_icon.png",name, content, 10001, session->createTime);
    }
    else if(session->type == MESSAGE_TYPE::SESSION_JOIN_TRIBLE_REQUEST){
        std::string name = "俱乐部申请";
        std::string content;
        content = session->content;
        Json::Value json = Utils::ParseJsonStr(content);
        renderItem("Msg/trible_request_icon.png",name, json["content"].asString(), 10003, session->createTime);
    }
    else if(session->type == MESSAGE_TYPE::SESSION_AGAINST_NOTIFY){
        std::string name = "对局通知";  //现在有解散房间的消息和人满的消息 type为1是解散房间的消息  没有则为满人的消息
        std::string content;
        content = session->content;
        Json::Value json = Utils::ParseJsonStr(content);
        std::string tipsContent;
        if(json["type"].asInt() == 1){
            //解散
            tipsContent = json["content"].asString();
        }else{
            stringstream stream;
            stream<<json["number"].asInt();
            tipsContent += "[已"+stream.str()+"人]"+json["gameName"].asString()+":"+json["roomName"].asString();
        }
        renderItem("Msg/against_notify_icon.png",name, tipsContent, json["sendId"].asInt(), session->createTime);
    }else if (session->type == MESSAGE_TYPE::SESSION_POINT_MESSAGE)
    {
        int id = session->mid;
        
        MsgModel::FriendInfo* fi = MsgModel::getInstance()->friendList[id];
        if (fi == NULL)
        {
            log("error fi is NULL id[%d]",id);
            return;
        }
//        Sprite* avatar = Utils::createCircleAvatar(fi->photo, "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(144,144));
        auto avatar = Utils::createCircleImage(fi->photo, "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(144,144));
        avatar->setPosition(100, itemSize.height / 2);
        addChild(avatar);
        
        std::string name = fi->rmkName != "" ? fi->rmkName : fi->nickName;
        
        char buf[128];
        Utils::parseName(10, buf, name.c_str());
        Text* text = Text::create(buf, "AmericanTypewriter", 45);
        text->setColor(Color3B(208, 208, 208));
        text->setAnchorPoint(Vec2(0, 0.5));
        text->setPosition(Vec2(200,144));
        addChild(text,1000);
        
        std::string content;
        if (session->msgType == MESSAGE_TYPE::MSG_TYPE_TEXT)
        {
            content = session->content;
        }
        else if (session->msgType == MESSAGE_TYPE::MSG_TYPE_IMAGE)
        {
            content = "[图片]";
        }
        else if (session->msgType == MESSAGE_TYPE::MSG_TYPE_AUDIO)
        {
            content = "[语音]";
        }
        else
        {
            content = session->content;
        }
        
        Utils::parseName(15, buf, content.c_str());
        
        text = Text::create(buf, "AmericanTypewriter", 40);
        text->setColor(Color3B(112, 112, 112));
        text->setAnchorPoint(Vec2(0, 0.5));
        text->setPosition(Vec2(200,68));
        addChild(text,1000);
        
        Utils::getTimeOver(session->createTime, buf);
        
        text = Text::create(buf, "AmericanTypewriter", 40);
        text->setColor(Color3B(112, 112, 112));
        text->setAnchorPoint(Vec2(1, 0.5));
        text->setPosition(Vec2(size.width - 100,itemSize.height * 4 / 5));
        addChild(text,1000);
        
        rendUnReadTips(id,itemSize);
    }
    else if (session->type == MESSAGE_TYPE::SESSION_TRIBLE_GROUP_MESSAGE)
    {
        int id = session->mid;
        MsgModel::FriendInfo* fi = MsgModel::getInstance()->friendGroupList[id];
        if (fi == NULL)
        {
            log("error fi is NULL id[%d]",id);
            return;
        }
        std::string content;
        if (session->msgType == MESSAGE_TYPE::MSG_TYPE_TEXT)
        {
            content = session->content;
        }
        else if (session->msgType == MESSAGE_TYPE::MSG_TYPE_IMAGE)
        {
            content = "[图片]";
        }
        else if (session->msgType == MESSAGE_TYPE::MSG_TYPE_AUDIO)
        {
            content = "[语音]";
        }else if (session->msgType == MESSAGE_TYPE::MSG_TYPE_CLUB_ROOM_CREATE)
        {
            content = "[创建新房间]";
        }
        else
        {
            content = session->content;
        }
        string photo = fi->photo;
        if(photo == ""){
            requestClubMessage(id);
        }
        renderItem(fi->photo,fi->nickName, content, id, session->createTime, SESSION_TRIBLE_GROUP_MESSAGE);

    }
    else if (session->type == MESSAGE_TYPE::SESSION_FIND_NEW_FRIENDS)
    {
        Sprite* avatar = Sprite::create("Avatars/user2.png");
        avatar->setPosition(100, itemSize.height / 2);
        addChild(avatar);
        
        Text* text = Text::create("发现好友", "AmericanTypewriter", 45);
        text->setColor(Color3B(208, 208, 208));
        text->setAnchorPoint(Vec2(0, 0.5));
        text->setPosition(Vec2(200,itemSize.height * 4 / 5));
        addChild(text,1000);
    }
    else if (session->type == MESSAGE_TYPE::SESSION_PAIJU_DONGTAI)
    {
        Sprite* avatar = Sprite::create("Avatars/user2.png");
        avatar->setPosition(100, itemSize.height / 2);
        addChild(avatar);
        
        Text* text = Text::create("牌局动态", "AmericanTypewriter", 45);
        text->setColor(Color3B(208, 208, 208));
        text->setAnchorPoint(Vec2(0, 0.5));
        text->setPosition(Vec2(200,itemSize.height * 4 / 5));
        addChild(text,1000);
    }
}

void ChatSessionItem::renderItem(string icon,string title, string content,long sendId, long long time,int sessionType){
    auto avatar = Utils::createCircleImage(icon, "Avatars/user4_unlogin.png", "Avatars/StencilBg.png", Size(144,144));
    avatar->setPosition(100, getContentSize().height / 2);
    addChild(avatar);
    
    char buf[128];
    Utils::parseName(10, buf, title.c_str());
    Text* text = Text::create(buf, "AmericanTypewriter", 45);
    text->setColor(Color3B(208, 208, 208));
    text->setAnchorPoint(Vec2(0, 0.5));
    text->setPosition(Vec2(200,144));
    addChild(text,1000);
    
    Utils::parseName(16, buf, content.c_str());
    
    text = Text::create(buf, "AmericanTypewriter", 40);
    text->setColor(Color3B(112, 112, 112));
    text->setAnchorPoint(Vec2(0, 0.5));
    text->setPosition(Vec2(200,68));
    addChild(text,1000);
    
    Utils::getTimeOver(time, buf);
    
    Size size = Director::getInstance()->getWinSize();
    text = Text::create(buf, "AmericanTypewriter", 40);
    text->setColor(Color3B(112, 112, 112));
    text->setAnchorPoint(Vec2(1, 0.5));
    text->setPosition(Vec2(size.width - 100,getContentSize().height * 4 / 5));
    addChild(text,1000);
    rendUnReadTips(sendId,getContentSize(),sessionType);
}

void ChatSessionItem::rendUnReadTips(long uid,Size itemSize,int sessionType){
    Size size = Director::getInstance()->getWinSize();
    unReadNums_bg = Sprite::create("Msg/news.png");
    unReadNums_bg->setPosition(Vec2(size.width - 130,itemSize.height * 2 / 5));
    addChild(unReadNums_bg,1000);
    MsgModel::FriendInfo* fi = NULL;
    if(sessionType == SESSION_TRIBLE_GROUP_MESSAGE){
        fi = MsgModel::getInstance()->friendGroupList[uid];
    }else{
        fi = MsgModel::getInstance()->friendList[uid];
    }
    if (fi == NULL)
    {
        log("error fi is NULL id[%ld]",uid);
        return;
    }
    
    unReadNums = Text::create(__String::createWithFormat("%d",fi->unReadNums)->getCString(), "AmericanTypewriter", 40);
    unReadNums->setColor(Color3B(208, 208, 208));
    unReadNums->setAnchorPoint(Vec2(0.5, 0.5));
    unReadNums->setPosition(unReadNums_bg->getContentSize() / 2);
    unReadNums_bg->addChild(unReadNums,1000);
    
    if (fi->unReadNums > 0)
    {
        unReadNums_bg->setVisible(true);
    }
    else
    {
        unReadNums_bg->setVisible(false);
    }
    
}

void ChatSessionItem::updateView()
{
    int id = session->mid;
    
    MsgModel::FriendInfo* fi = MsgModel::getInstance()->friendList[id];
    
    if (fi == NULL)
    {
        log("error fi is NULL id[%d]",id);
        return;
    }
    
    if (fi->unReadNums > 0)
    {
        unReadNums->setString(__String::createWithFormat("%d",fi->unReadNums)->getCString());
        unReadNums_bg->setVisible(true);
    }
    else
    {
        unReadNums->setString("");
        unReadNums_bg->setVisible(false);
    }
}

void ChatSessionItem::requestClubMessage(int groupId){
    Json::Value jsonDate;
    jsonDate["groupId"] = groupId;
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
            //创建一个俱乐部friendInfo
            MsgModel::FriendInfo* fi = new MsgModel::FriendInfo;
            fi->mid = groupId;
            fi->nickName = name;
            fi->photo = data["logo"].asString();
            fi->friendType = FRIEND_TYPE_GROUP;//设置为俱乐部好友类型
            fi = MsgModel::getInstance()->addFriend(fi);
            __NotificationCenter::getInstance()->postNotification("Msg_reflash");        }
    },"club/clubInfo/searchClubByGroupId",jsonDate.toStyledString(),"searchClubByGroupId");
}

void ChatSessionItem::btnCallback(Ref* ref)
{
}
