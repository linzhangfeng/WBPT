//
//  RoomChatLayer.h
//  ZJH
//
//  Created by hyhy on 16/7/25.
//
//

#ifndef __ZJH__RoomChatLayer__
#define __ZJH__RoomChatLayer__

#include "cocos2d.h"
#include "ZJH.h"

class RoomChatLayer : public cocos2d::Layer
{
public:
    CREATE_FUNC(RoomChatLayer)
    virtual bool init();
    void notificationCallback(cocos2d::Ref *msg);
    void ShowMoveChat(int uid, std::string name, std::string content);//弹幕
    Point GetChatPos(int nums);
    void onExit();
};

#endif /* defined(__ZJH__RoomChatLayer__) */
