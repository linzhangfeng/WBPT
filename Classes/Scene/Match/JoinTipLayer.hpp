//
//  JoinTipLayer.hpp
//  ZJH
//
//  Created by apple on 16/7/13.
//
//

#ifndef JoinTipLayer_hpp
#define JoinTipLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"

using namespace cocos2d::ui;
using namespace cocos2d;

class  JoinTipLayer: public Layer
{
    
public:
    virtual bool init();
    CREATE_FUNC(JoinTipLayer);
    void showMoneyBuyTip(int states, string title, string text);
    
    void buyPropView(Json::Value json);
    
private:
    Layer *layer1;
    void sureOn(Ref *pSender, Widget::TouchEventType type);
    void exitOn(Ref *pSender, Widget::TouchEventType type);
    
    /*************************道具购买 start *********************************/
    void numChange(Ref *pSender, Widget::TouchEventType type);
    void buyPropOn(Ref *pSender, Widget::TouchEventType type);
    void priceByTypeString();
    
    
    Text *numsText;
    Text *priceText;
    int nums;
    Json::Value propJson;
    
    /************************ 道具购买 end ************************************/
};
#endif /* JoinTipLayer_hpp */
