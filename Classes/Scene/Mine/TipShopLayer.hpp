//
//  TipChangeEditLayer.hpp
//  ZJH
//
//  Created by apple on 16/7/23.
//
//

#ifndef TipShopLayer_hpp
#define TipShopLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ZJh.h"


using namespace cocos2d::ui;
using namespace cocos2d;


class  TipShopLayer: public Layer
{
    
public:

    static TipShopLayer* create();
    virtual bool init();
    
    void buyWanPayType(Json::Value json, int index, int FromType = 0);
    void buyWanFun(Ref*ref);
    
    void buyCardGold(int type ,Json::Value json);
    void buyCardGoldWanRechange(int type = 1);
    
private:
    
    Json::Value wanList;
    int wanIndex;
    Json::Value goldList;
    int payFromType;
};

#endif /* TipChangeEditLayer_hpp */
