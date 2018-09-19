//
//  RoomInformationLayer.h
//  ZJH
//
//  Created by hyhy on 16/7/26.
//
//

#ifndef __ZJH__RoomInformationLayer2__
#define __ZJH__RoomInformationLayer2__

#include "ZJH.h"

//房间信息新布局
class RoomInformationLayer2 : public cocos2d::Layer
{
public:
    CREATE_FUNC(RoomInformationLayer2);
    virtual bool init();
    void myInit(Json::Value &data);
    
    void show();
    void hide();
    
    bool onTouchBegan(Touch* touch, Event* event);
};

#endif /* defined(__ZJH__RoomInformationLayer__) */
