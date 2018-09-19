//
//  RoomInvite.h
//  ZJH
//
//  Created by hyhy on 16/7/26.
//
//

#ifndef __ZJH__RoomInvite__
#define __ZJH__RoomInvite__

#include "ZJH.h"

class RoomInvite : public cocos2d::Layer
{
public:
    CREATE_FUNC(RoomInvite);
    virtual bool init();
    void myInit(Json::Value &data);
    
    void show();
    void hide();
    
    bool onTouchBegan(Touch* touch, Event* event);
    void buttonClicked(Ref *sender);
private:
    std::string m_inviteCode;
    int m_roomId;
    Json::Value m_jsonData;
};

#endif /* defined(__ZJH__RoomInvite__) */
