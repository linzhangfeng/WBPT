//
//  GoldShopLayer.hpp
//  ZJH
//
//  Created by apple on 16/8/29.
//
//

#ifndef GrowTaskLayer_hpp
#define GrowTaskLayer_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"
#include "../Loading/Loading.h"

using namespace cocos2d::ui;
using namespace cocos2d;


class  GrowTaskLayer: public cocos2d::ui::Widget
{

public:
    virtual bool init();
    CREATE_FUNC(GrowTaskLayer);
    void postTask2(float dt);
    
    void showView1(Json::Value json);
    Layout *createItem(Json::Value json, int index);
    void receiveOnFun(Ref *pSender);
    void onExit();
private:
    
    Loading *loading;
    Json::Value taskList;
    
};

#endif /* GoldShopLayer_hpp */
