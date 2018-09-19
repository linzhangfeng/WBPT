//
//  ChatFriends.hpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//

#ifndef ChatFriends_hpp
#define ChatFriends_hpp

#include "ZJH.h"
#include "../BaseLayer.hpp"
#include "../../Model/MsgModel.h"
USING_NS_CC;

class ChatFriends : public BaseLayer
{
public:
    CREATE_FUNC(ChatFriends);
    bool init();
    void myInit(Json::Value &json);
    void btnCallback(Ref* ref);
    
    void notificationChat(Ref* msg);
    void nameReflash(Ref* msg);
    void initList();
    void getRecordData(bool init = true);
    virtual void onExit();
    ~ChatFriends();
private:
    long long lastTime;
    long long newTime;
    
    int curPage;
    int pageSize;
    int lastId;
    bool hasMore;
    int _cuid;
};

class ChatFriends_Item : public Layer
{
public:
    CREATE_FUNC(ChatFriends_Item);
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
#endif /* ChatFriends_hpp */
