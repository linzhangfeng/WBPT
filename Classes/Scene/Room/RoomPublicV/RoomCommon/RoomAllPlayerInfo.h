//
//  RoomAllPlayerInfo.hpp
//  ZJH
//
//  Created by wuyu on 16/3/28.
//
//  所有外围玩家信息

#ifndef RoomAllPlayerInfo_h
#define RoomAllPlayerInfo_h

#include <stdio.h>
#include "ZJH.h"

class RoomAllPlayerInfo : public cocos2d::Layer
{
public:
    static RoomAllPlayerInfo *create(JPacket &jpacket);
    virtual bool init(JPacket &jpacket);
    bool isTouchInside(Touch* touch);
    virtual bool onTouchBegan(Touch* touch, Event* event);
    virtual void onTouchMoved(Touch* touch, Event* event);
    virtual void onTouchEnded(Touch* touch, Event* event);
    void onKeyReleased(EventKeyboard::KeyCode keycode,Event* event);
    void closeCallBack(Ref * psender);
};
#endif /* RoomAllPlayerInfo_h */
