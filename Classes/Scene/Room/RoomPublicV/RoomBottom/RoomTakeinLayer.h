//
//  RoomTakeinLayer.h
//  ZJH
//
//  Created by hyhy on 16/7/26.
//
//

#ifndef __ZJH__RoomTakeinLayer__
#define __ZJH__RoomTakeinLayer__

#include "ZJH.h"

class RoomTakeinLayer : public cocos2d::Layer
{
public:
    CREATE_FUNC(RoomTakeinLayer);
    virtual bool init();
    void myInit(int rid);
    void initListView(Json::Value &data);
    void getData(float dt);
    void show();
    void hide();
    
    bool onTouchBegan(Touch* touch, Event* event);
};

#endif /* defined(__ZJH__RoomTakeinLayer__) */
