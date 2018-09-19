//
//  RoomExitDialog.h
//  ZJH
//
//  Created by hyhy on 16/8/4.
//
//

#ifndef __ZJH__RoomExitDialog__
#define __ZJH__RoomExitDialog__

#include "cocos2d.h"
#include "ZJH.h"

class RoomExitDialog : public cocos2d::Layer
{
public:
    CREATE_FUNC(RoomExitDialog);
    virtual bool init();
    RoomExitDialog();
    
    void show();
    void buttonClicked(Ref *ref);
    bool onTouchBegan(Touch* touch, Event* event);
    CC_SYNTHESIZE(MyCallBack_Str, m_pCallback, Callback);
};

#endif /* defined(__ZJH__RoomExitDialog__) */
