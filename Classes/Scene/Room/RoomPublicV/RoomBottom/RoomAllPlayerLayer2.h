//
//  RoomAllPlayerLayer.h
//  ZJH
//
//  Created by hyhy on 16/7/27.
//
//

#ifndef __ZJH__RoomAllPlayerLayer2__
#define __ZJH__RoomAllPlayerLayer2__

#include "ZJH.h"

class RoomAllPlayerLayer2 : public cocos2d::Layer
{
public:
    CREATE_FUNC(RoomAllPlayerLayer2);
    virtual bool init();
    void myInit(int hostUid, int roomId);
    
    void show();
    void hide();
    ui::Widget *createItem(Json::Value &itemData, Size &size, int i);
    void inviteFriend(Ref *sender);
    
    void getData(float dt);
    void initListView(Json::Value &data);
    
    bool onTouchBegan(Touch* touch, Event* event);
private:
    int m_hostUid;
    int m_roomId;
};

#endif /* defined(__ZJH__RoomAllPlayerLayer__) */
