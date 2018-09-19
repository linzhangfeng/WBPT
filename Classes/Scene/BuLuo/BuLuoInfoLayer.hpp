//
//  MineMoreLayer.hpp
//  ZJH
//
//  Created by apple on 16/7/27.
//
//

#ifndef BuLuoInfoLayer_hpp
#define BuLuoInfoLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"
#include "../BaseLayer.hpp"

using namespace cocos2d::ui;
using namespace cocos2d;


class  BuLuoInfoLayer: public BaseLayer
{
public:
    
    virtual bool init();

    CREATE_FUNC(BuLuoInfoLayer);
    
    void showView(Json::Value jsonData);
    
    string createTimeBecomeString(long long);
    
private:
       
};

#endif /* BuLuoInfoLayer_hpp */
