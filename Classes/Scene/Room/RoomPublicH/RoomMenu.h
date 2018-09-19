//
//  RoomButton.h
//  ZJH
//
//  Created by wuyu on 16/3/15.
//
//

#ifndef RoomMenu_h
#define RoomMenu_h

#include <stdio.h>
#include "ZJH.h"

using namespace cocos2d;

class RoomMenu : public cocos2d::Layer
{
public:
    RoomMenu();
    virtual bool init();
    CREATE_FUNC(RoomMenu);
    void MyInit(int type =0,bool isCMRoom = false);
    void buttonClicked(Ref *ref);
    
    CC_SYNTHESIZE(MyCallBack_Str, m_menuCallback, MenuCallback);
    
    bool isTouchInside(Touch* touch);
    void closeCallBack();
    
    virtual bool onTouchBegan(Touch* touch, Event* event);
    virtual void onTouchMoved(Touch* touch, Event* event);
    virtual void onTouchEnded(Touch* touch, Event* event);
    void onKeyReleased(EventKeyboard::KeyCode keycode,Event* event);
};
#endif /* RoomMenu_h */
