//
//  RoomChat.hpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//

#ifndef RoomChat_hpp
#define RoomChat_hpp

#include "ZJH.h"
#include "../../../BaseLayer.hpp"
#include "../../../../Model/MsgModel.h"
#include "RoomChatBottom.hpp"
USING_NS_CC;

class RoomChat : public Layer
{
public:
    CREATE_FUNC(RoomChat);
    bool init();
    void myInit(Json::Value &json);
    void btnCallback(Ref* ref);
    
    void notificationChat(Ref* msg);
    void initList();
    void getRecordData(bool init = true);
    virtual void onExit();
    
    void show();
    void hide();
    void showFastChatWindow();
private:
    long long lastTime;
    long long newTime;
    
    int curPage;
    int pageSize;
    int lastId;
    bool hasMore;
    RoomChatBottom* chat_bottom;
};

class RoomChat_Item : public Layer
{
public:
    CREATE_FUNC(RoomChat_Item);
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
    Sprite* chating;
    std::string soundfile;
};
#endif /* RoomChat_hpp */
