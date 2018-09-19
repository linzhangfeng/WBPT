//
//  RoomShareLayer.h
//  ZJH
//
//  Created by hyhy on 16/7/26.
//
//

#ifndef __ZJH__RoomShareLayer__
#define __ZJH__RoomShareLayer__

#include "ZJH.h"

class RoomShareLayer : public cocos2d::Layer
{
public:
    CREATE_FUNC(RoomShareLayer);
    virtual bool init();
    
    void show();
    void hide();
    
    bool onTouchBegan(Touch* touch, Event* event);
    void buttonClicked(Ref *sender);
};

#endif /* defined(__ZJH__RoomShareLayer__) */
