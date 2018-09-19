//
//  RoomButton.h
//  ZJH
//
//  Created by wuyu on 16/3/15.
//
//

#ifndef RoomButton_h
#define RoomButton_h

#include <stdio.h>
#include "ZJH.h"

using namespace cocos2d;

class RoomButton : public cocos2d::Layer
{
public:
    RoomButton();
    virtual bool init();
    CREATE_FUNC(RoomButton);
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
#endif /* RoomButton_h */
