//
//  wanBeiShop.hpp
//  ZJH
//
//  Created by apple on 16/6/30.
//
//

#ifndef WanBeiShop_hpp
#define WanBeiShop_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"

using namespace cocos2d::ui;
using namespace cocos2d;


class  WanBeiShop: public cocos2d::ui::Widget
{
    
public:
    
    virtual bool init();
    CREATE_FUNC(WanBeiShop);
    
    void selectPayFun(Ref *pSender, Widget::TouchEventType type);
    void touchIndexFun(Ref *pSender, Widget::TouchEventType type);
    

    ImageView* createItem(Json::Value json);
    
    void wanView(Json::Value jsonData);
    void postWan(float dt);
    
    void buyWanFun(Ref*ref);
private:
    
    Json::Value tipJson;
    
    Layout *selectPay;
    Text *wanValue;
    Text *renMinValue;
    Text *payValue;
    
    Text *wanTextRight;
    Json::Value wanList;
    
    int wanNumRight;
    
    Text *payTitle;
    Layout *lastItem;
    
    
};


#endif /* WanBeiShop_hpp */
