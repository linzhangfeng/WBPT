//
//  FriendLayer.hpp
//  wanMain
//
//  Created by apple on 16/6/23.
//
//

#ifndef MyBuLuoLayer_h
#define MyBuLuoLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"
#include "../BaseLayer.hpp"

using namespace cocos2d::ui;
using namespace cocos2d;

class MyBuLuoLayer: public BaseLayer
{
public:
    
    MyBuLuoLayer();
    virtual bool init();
    CREATE_FUNC(MyBuLuoLayer);
    
    
    void postData(float dt);
    void showInfoView(Json::Value json);
    void showBuLuoGongNeng(ListView *listView);
    void noBuoLuoInfoView();
    
    void joinBuLuo(Ref*ref);
    void showBuyZuZhang(Json::Value json);
    
    void sureJoinOn(Ref*ref);
    
    void exitBuLuoOn();
    
    void exitTipView(Ref*ref);
private:
    
   EditBox* textField2;

    Layout* BG;
    
    Json::Value infoJson;
    
};
#endif /* MyBuLuoLayer_hpp */
