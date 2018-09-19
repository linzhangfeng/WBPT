//
// 房间信息改版
//
//

#ifndef __ZJH__RoomInformation__
#define __ZJH__RoomInformation__

#include "ZJH.h"

class RoomInformation : public cocos2d::Layer
{
public:
    CREATE_FUNC(RoomInformation);
    virtual bool init();
    void myInit(Json::Value &data);
    
    void show();
    void hide();
    
    RichText * createRichText(const std::string &label, const std::string &content, int type);
    
    bool onTouchBegan(Touch* touch, Event* event);
};

#endif /* defined(__ZJH__RoomInformation__) */
