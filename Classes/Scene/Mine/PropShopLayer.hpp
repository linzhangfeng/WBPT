//
//  PropShopLayer.hpp
//  ZJH
//
//  Created by apple on 16/8/29.
//
//

#ifndef PropShopLayer_hpp
#define PropShopLayer_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"


using namespace cocos2d::ui;
using namespace cocos2d;


class  PropShopLayer: public cocos2d::ui::Widget
{
    
public:
    virtual bool init();
    CREATE_FUNC(PropShopLayer);
    
    ImageView* createItem(Json::Value json);
    
    void postProp(float dt);
    void propView(Json::Value jsonData);
    
    void buyPropOn(Ref *pSender, Widget::TouchEventType type);
    void gotoMyPropOn(Ref *pSender, Widget::TouchEventType type);
    
    void getMoney(Json::Value json);
    
private:
    
    Json::Value propJson;
    Json::Value moneyJson;
    
};
#endif /* PropShopLayer_hpp */
