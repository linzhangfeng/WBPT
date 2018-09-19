//
//  FriendLayer.hpp
//  wanMain
//
//  Created by apple on 16/6/23.
//
//

#ifndef BuLuoTuiGuangLayer_h
#define BuLuoTuiGuangLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"
#include "../BaseLayer.hpp"

using namespace cocos2d::ui;
using namespace cocos2d;

class BuLuoTuiGuangLayer: public BaseLayer
{
public:
    
    BuLuoTuiGuangLayer();
    virtual bool init();
    CREATE_FUNC(BuLuoTuiGuangLayer);
    
    
    void showInfoView(Json::Value json);
    
private:
    Text *codeText;
    
};

#endif /* BuLuoManagerLayer_hpp */
