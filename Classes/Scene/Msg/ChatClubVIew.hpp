//
//  ChatClubVIew.hpp
//  ZJH
//
//  Created by mac on 2017/4/24.
// 俱乐部聊天
//

#ifndef ChatClubVIew_hpp
#define ChatClubVIew_hpp

#include <stdio.h>
#include "ZJH.h"
#include "../BaseLayer.hpp"
#include "../../Model/MsgModel.h"
USING_NS_CC;

class ChatClubVIew : public BaseLayer
{
public:
    CREATE_FUNC(ChatClubVIew);
    bool init();
    void myInit(Json::Value &json);
    void initChatView();
    void initRoomListView();
    void switTab(string tab);
    void btnCallback(Ref* ref);
    
    void notificationChat(Ref* msg);
    void nameReflash(Ref* msg);
    void initList();
    void getRecordData(bool init = true);
    virtual void onExit();
    void initHttpRequest();
    void requestRoomList();//请求未结束的房间列表
    void refreshRoomList(Json::Value jsonValue);
    ~ChatClubVIew();
private:
    long long lastTime;
    long long newTime;
    
    int curPage;
    int pageSize;
    int lastId;
    bool hasMore;
    int _cuid;
    int clubId;//俱乐部id
    Button* chatButton;
    Button* roomListButton;
    Layout* chatTab;
    Layout* roomListTab;
    Layout* top;
    Text* roomNumber;
    ListView* roomList;
};

class ChatRoom_Item : public Layout
{
  public:
    CREATE_FUNC(ChatRoom_Item);
    bool init();
    void initView(Json::Value data);
};

class ChatClub_Item : public Layer
{
public:
    CREATE_FUNC(ChatClub_Item);
    bool init();
    void initData(MsgModel::Chat *chat);
    void initData2(long long chat);
    void reflash(float dt);
    void btnCallback(Ref* ref);
    void startPlayNotify(Ref* ref);
    void stopPlayNotify(Ref* ref);
    void onExit();
private:
    MsgModel::Chat* chatP;
    
    Node* imgLoading;
    ImageView* sending;
    
    Sprite* chating;
    std::string soundfile;
};

#endif /* ChatClubVIew_hpp */
