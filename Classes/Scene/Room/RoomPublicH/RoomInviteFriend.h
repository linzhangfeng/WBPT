//
//  RoomInviteFriend.h
//  ZJH
//
//  Created by hyhy on 16/7/27.
//
//

#ifndef __ZJH__RoomInviteFriend__
#define __ZJH__RoomInviteFriend__

#include "ZJH.h"

class RoomInviteFriend : public cocos2d::Layer
{
public:
    CREATE_FUNC(RoomInviteFriend);
    virtual bool init();
    void myInit(int roomId, Json::Value &data);
    
    void show();
    void hide();
    void buttonClicked(Ref *ref);
    void listItemClicked(Ref *ref);
    bool onTouchBegan(Touch* touch, Event* event);
private:
    bool m_hasMore;
    int m_roomId;
    std::set<int> m_selection;
};

#endif /* defined(__ZJH__RoomInviteFriend__) */
