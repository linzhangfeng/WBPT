//
//  GoldShopLayer.hpp
//  ZJH
//
//  Created by apple on 16/8/29.
//
//

#ifndef CardShopLayer_hpp
#define CardShopLayer_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"


using namespace cocos2d::ui;
using namespace cocos2d;


class  CardShopLayer: public cocos2d::ui::Widget
{

public:
    virtual bool init();
    CREATE_FUNC(CardShopLayer);

    void cardView(Json::Value jsonData);
    void postCard(float dt);
    void buyGoldFun(Ref *pSender, Widget::TouchEventType type);
    void getMoney(Json::Value json);
    
private:
    
    Json::Value cardList;
    Json::Value tipJson;
};

#endif /* GoldShopLayer_hpp */
