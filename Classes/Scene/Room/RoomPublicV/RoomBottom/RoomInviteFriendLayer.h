//
//  RoomInviteFriendLayer.h
//  ZJH
//
//  Created by hyhy on 16/7/27.
//
//

#ifndef __ZJH__RoomInviteFriendLayer__
#define __ZJH__RoomInviteFriendLayer__

#include "ZJH.h"

class RoomInviteFriendLayer : public cocos2d::Layer
{
public:
    CREATE_FUNC(RoomInviteFriendLayer);
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

#endif /* defined(__ZJH__RoomInviteFriendLayer__) */
