//
//
//

#ifndef RoomHExitDialog_h
#define RoomHExitDialog_h

#include "cocos2d.h"
#include "ZJH.h"

class RoomHExitDialog : public cocos2d::Layer
{
public:
    CREATE_FUNC(RoomHExitDialog);
    virtual bool init();
    RoomHExitDialog();
    
    void show();
    void buttonClicked(Ref *ref);
    bool onTouchBegan(Touch* touch, Event* event);
    CC_SYNTHESIZE(MyCallBack_Str, m_pCallback, Callback);
};

#endif /* defined(RoomHExitDialog_h) */
