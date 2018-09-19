//
//  MineMoreLayer.hpp
//  ZJH
//
//  Created by apple on 16/7/27.
//
//

#ifndef ZuBuLuoInfoLayer_hpp
#define ZuBuLuoInfoLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"
#include "../BaseLayer.hpp"

using namespace cocos2d::ui;
using namespace cocos2d;


class  ZuBuLuoInfoLayer: public BaseLayer
{
public:
    
    virtual bool init();

    CREATE_FUNC(ZuBuLuoInfoLayer);
    
    void showView(Json::Value jsonData);
    
    string createTimeBecomeString(long long);
    
    void portraitFun(Ref *pSender, Widget::TouchEventType type);
    
    void nameFun(Ref *pSender, Widget::TouchEventType type);
    
    void updateName(Ref*ref);
    
    void updatePortrait(Ref*ref);
    
private:
    
    Json::Value dataTemp;
    
    Text *nameText;
    
    Layout *portraiCell;
};

#endif /* BuLuoInfoLayer_hpp */
