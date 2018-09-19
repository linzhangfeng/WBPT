//
//  RoomInformationLayer.h
//  ZJH
//
//  Created by hyhy on 16/7/26.
//
//

#ifndef __ZJH__RoomInformationLayer__
#define __ZJH__RoomInformationLayer__

#include "ZJH.h"

class RoomInformationLayer : public cocos2d::Layer
{
public:
    CREATE_FUNC(RoomInformationLayer);
    virtual bool init();
    void myInit(Json::Value &data);
    
    void show();
    void hide();
    
    bool onTouchBegan(Touch* touch, Event* event);
};

#endif /* defined(__ZJH__RoomInformationLayer__) */
