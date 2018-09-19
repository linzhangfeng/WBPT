//
//  RoomResultLayer.h
//  ZJH
//
//  Created by hyhy on 16/7/26.
//
//

#ifndef __ZJH__RoomResultLayer__
#define __ZJH__RoomResultLayer__

#include "ZJH.h"

class RoomResultLayer : public cocos2d::Layer
{
public:
    CREATE_FUNC(RoomResultLayer);
    virtual bool init();
    void myInit(int rid);
    void initListView(Json::Value &data);
    void show();
    void hide();
    void getData(float dt);
    bool onTouchBegan(Touch* touch, Event* event);
};

#endif /* defined(__ZJH__RoomResultLayer__) */
