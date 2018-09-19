//
//  FriendLayer.hpp
//  wanMain
//
//  Created by apple on 16/6/23.
//
//

#ifndef MyDaiLiLayer_h
#define MyDaiLiLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"
#include "../BaseLayer.hpp"
#include "../Loading/Loading.h"

using namespace cocos2d::ui;
using namespace cocos2d;

class MyDaiLiLayer: public BaseLayer
{
public:
    
    MyDaiLiLayer();
    virtual bool init();
    CREATE_FUNC(MyDaiLiLayer);
    
    void postData(float dt);
   
    void noJoinDaiLiView();
    void haveJoinView(Json::Value json);
    
    void sureJoinOn(Ref*ref);
    void sureJoinTipOn(Ref*ref);
    
    void daiLiOn(Ref*ref);
    void sureJoinTipView(Json::Value json);
    
private:
    
   EditBox* textField2;

    ImageView* BG;
    
    Json::Value infoJson;
    
    Loading *loading;
    
};
#endif /* MyDaiLiLayer_hpp */
