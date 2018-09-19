//
//  RoomAllPlayerLayer.h
//  ZJH
//
//  Created by hyhy on 16/7/27.
//
//

#ifndef __ZJH__RoomAllPlayerLayer__
#define __ZJH__RoomAllPlayerLayer__

#include "ZJH.h"

class RoomAllPlayerLayer : public cocos2d::Layer
{
public:
    CREATE_FUNC(RoomAllPlayerLayer);
    virtual bool init();
    void myInit(int hostUid, int roomId);
    
    void show();
    void hide();
    ui::Widget *createItem(Json::Value &itemData);
    void inviteFriend(Ref *sender);
    
    void getData(float dt);
    void initListView(Json::Value &data);
    
    bool onTouchBegan(Touch* touch, Event* event);
private:
    int m_hostUid;
    int m_roomId;
};

#endif /* defined(__ZJH__RoomAllPlayerLayer__) */
