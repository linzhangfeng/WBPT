//
//
//
//
//

#ifndef RoomSettingH_h
#define RoomSettingH_h

#include "cocos2d.h"
#include "ZJH.h"

class RoomSettingH : public cocos2d::Layer
{
public:
    CREATE_FUNC(RoomSettingH)
    virtual bool init();
    void initSetting();
    void buttonClicked(Ref *sender);
    
    bool onTouchBegan(Touch* touch, Event* event);
};

#endif /* defined(RoomSettingH_h) */
