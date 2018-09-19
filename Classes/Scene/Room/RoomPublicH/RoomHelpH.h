//
//

#ifndef RoomHelpH_h
#define RoomHelpH_h

#include "cocos2d.h"
#include "ZJH.h"

class RoomHelpH : public cocos2d::Layer
{
public:
    CREATE_FUNC(RoomHelpH);
    virtual bool init();
    void initHelp(Json::Value &json);
    
    void initHelpWeb();
    
    bool onTouchBegan(Touch* touch, Event* event);
};

#endif /* defined(RoomHelpH_h) */
