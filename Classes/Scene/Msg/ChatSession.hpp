//
//  Msg.hpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//

#ifndef ChatSession_hpp
#define ChatSession_hpp

#include "ZJH.h"
#include "../BaseLayer.hpp"
#include "../../Model/MsgModel.h"

USING_NS_CC;

class ChatSession : public BaseLayer
{
public:
    CREATE_FUNC(ChatSession);
    bool init();
    void notificationReflash(Ref* msg);
    void initList();
    void updateReflash(float dt);
    
public:
    Layout* curLayout;
    long long lastReflashTime;
    bool hasUpdate;
};

class ChatSessionItem : public Layer
{
public:
    CREATE_FUNC(ChatSessionItem);
    bool init();
    void initData(MsgModel::Session* session,const MyCallBack_Int &callback,int index);
    void btnCallback(Ref* ref);
    void updateView();
    void rendUnReadTips(long uid,Size itemSize,int sessionType = SESSION_POINT_MESSAGE);
    void renderItem(string icon,string title,string content,long sendId,long long time,int sessionType = SESSION_POINT_MESSAGE);
    void requestClubMessage(int clubId); //在没有俱乐部图标的时候要请求一次服务器获取俱乐部数据
private:
    MsgModel::Session* session;
    Sprite* unReadNums_bg;
    Text* unReadNums;
};

#endif /* Msg_hpp */
