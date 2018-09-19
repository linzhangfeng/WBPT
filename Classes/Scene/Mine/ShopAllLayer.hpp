//
//  ShopAllLayer.hpp
//  ZJH
//
//  Created by apple on 16/8/29.
//
//

#ifndef ShopAllLayer_hpp
#define ShopAllLayer_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"
#include "../BaseLayer.hpp"
#include "../Loading/Loading.h"

using namespace cocos2d::ui;
using namespace cocos2d;


class  ShopAllLayer: public BaseLayer
{
    
public:
    
    virtual bool init();
    CREATE_FUNC(ShopAllLayer);
    
    void wanView();
    void goldView();
    void propView();
    
    void postMoney(Ref* ref);
    void updateShowWan(Ref* ref);
    void tabClickFun(Ref *pSender, Widget::TouchEventType type);
    void addShopObserver();
    virtual void onEnter();
    void deleteLoading(Ref*ref);
    void createLoading(Ref*ref);
    
private:
    //新添加的
    int currentPage;
    
    Json::Value tipJson;
    
    Layout *bgLayout;
    Text *wanText;
    Text*cardText;
    Text *goldText;
    Loading *loading;
};


#endif /* ShopAllLayer_hpp */
