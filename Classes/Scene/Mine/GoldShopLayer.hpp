//
//  GoldShopLayer.hpp
//  ZJH
//
//  Created by apple on 16/8/29.
//
//

#ifndef GoldShopLayer_hpp
#define GoldShopLayer_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"


using namespace cocos2d::ui;
using namespace cocos2d;


class  GoldShopLayer: public cocos2d::ui::Widget
{

public:
    virtual bool init();
    CREATE_FUNC(GoldShopLayer);

    void goldView(Json::Value jsonData);
    void postGold(float dt);
    void buyGoldFun(Ref *pSender, Widget::TouchEventType type);
    void getMoney(Json::Value json);
    string covertValueStr(int value);
    
private:
    
    Json::Value goldList;
    Json::Value tipJson;
};

#endif /* GoldShopLayer_hpp */
