//
//  RoomChat2.hpp
//  ZJH
//
//  Created by pg on 16/9/6.
//
//

#ifndef RoomChat2_hpp
#define RoomChat2_hpp

#include "ZJH.h"
#include "../../BaseLayer.hpp"
#include "../../../Model/MsgModel.h"
USING_NS_CC;

class RoomChat2 : public Layer
{

public:
    CREATE_FUNC(RoomChat2);
//    static RoomChat2* create(Node* pRoot);
    bool init();
    void btnCallback(Ref* ref);
    
    void notificationChat(Ref* msg);
    void initList();
    void getRecordData(bool init = true);
    virtual void onExit();
    
    void show();
    void hide();
    
    bool onTouchBegan(Touch* touch, Event* event);
    
private:
    long long lastTime;
    long long newTime;
    
    int curPage;
    int pageSize;
    int lastId;
    bool hasMore;
    Vec2 m_nPosition;
    EventListenerTouchOneByOne* m_pTouchListener;
};

class RoomChat2_Item : public Layer
{
public:
    CREATE_FUNC(RoomChat2_Item);
    bool init();
    void initData(MsgModel::Chat *chat,Size nChatSize);
    void initData2(long long chat,Size nChatSize);
    void reflash(float dt);
    void btnCallback(Ref* ref);
    void startPlayNotify(Ref* ref);
    void stopPlayNotify(Ref* ref);
    void onExit();
private:
    //聊天框背景
    CC_SYNTHESIZE(Size, m_nChatSize, ChatSize);
    
    MsgModel::Chat* chatP;
    Sprite* chating;
    std::string soundfile;
};

#endif /* RoomChat22_hpp */
