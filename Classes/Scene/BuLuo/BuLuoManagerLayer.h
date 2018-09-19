//
//  FriendLayer.hpp
//  wanMain
//
//  Created by apple on 16/6/23.
//
//

#ifndef BuLuoManagerLayer_h
#define BuLuoManagerLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"
#include "../BaseLayer.hpp"

using namespace cocos2d::ui;
using namespace cocos2d;

class BuLuoManagerLayer: public BaseLayer
{
public:
    
    virtual bool init();
    CREATE_FUNC(BuLuoManagerLayer);
    
    void showInfoView(Json::Value json);
    void postData(float dt);
    
    void showBuLuoGongNeng();
    void updateName(Ref*ref);
    void updatePortrit(Ref*ref);
    void clickBtnGoTo(Ref*ref);
    
private:
    Sprite *userPortrit;
    ImageView*infoBg;
    
    Text *nameText;
    Json::Value jsonData;
    
};

#endif /* BuLuoManagerLayer_hpp */
