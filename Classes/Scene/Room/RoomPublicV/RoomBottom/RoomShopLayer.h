//
//  RoomShopLayer.h
//  ZJH
//
//  Created by hyhy on 16/8/5.
//
//

#ifndef __ZJH__RoomShopLayer__
#define __ZJH__RoomShopLayer__

#include "ZJH.h"

class RoomShopLayer : public cocos2d::Layer
{
public:
    RoomShopLayer();
    CREATE_FUNC(RoomShopLayer);
    virtual bool init();
    void myInit();
    void myInitWealth(Json::Value &data);
    void myInitShop(Json::Value &data);
    void show();
    void hide();
    
    bool onTouchBegan(Touch* touch, Event* event);
private:
    cocos2d::ui::ListView *m_listView;
};

#endif /* defined(__ZJH__RoomShopLayer__) */
