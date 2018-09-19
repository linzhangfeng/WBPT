//
//  RoomSetting.h
//  ZJH
//
//  Created by hyhy on 16/7/25.
//
//

#ifndef __ZJH__RoomSetting__
#define __ZJH__RoomSetting__

#include "cocos2d.h"
#include "ZJH.h"

class RoomSetting : public cocos2d::Layer
{
public:
    CREATE_FUNC(RoomSetting)
    virtual bool init();
    void initSetting();
    void buttonClicked(Ref *sender);
    
    bool onTouchBegan(Touch* touch, Event* event);
};

#endif /* defined(__ZJH__RoomSetting__) */
