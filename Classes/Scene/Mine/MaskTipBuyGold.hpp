//
//  MaskTipBuyGold.hpp
//  ZJH
//
//  Created by apple on 16/7/19.
//
//

#ifndef MaskTipBuyGold_hpp
#define MaskTipBuyGold_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"

using namespace cocos2d::ui;
using namespace cocos2d;

class  MaskTipBuyGold: public Layer
{
    
public:
    virtual bool init();
    CREATE_FUNC(MaskTipBuyGold);
    void showView(Json::Value json, bool isShoping=false);
    //私密公开房创建的提示
    
    void showCreateRoom(Json::Value json);
    
private:
    
    void sureOn(Ref *pSender, Widget::TouchEventType type);
    void exitOn(Ref *pSender, Widget::TouchEventType type);
    void gotoOpenOn(Ref *pSender);
    
    LayerColor *layer1;
    Json::Value jsonData;
    bool isShoping;
    
};

#endif /* MaskTipBuyGold_hpp */
