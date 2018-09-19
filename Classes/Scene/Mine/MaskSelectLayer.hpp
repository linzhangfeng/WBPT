//
//  MaskSelectLayer.hpp
//  wanMain
//
//  Created by apple on 16/6/29.
//
//

#ifndef MaskSelectLayer_hpp
#define MaskSelectLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"

using namespace cocos2d::ui;
using namespace cocos2d;


class  MaskSelectLayer: public Layer
{
    
public:
    static MaskSelectLayer* create(int index);
    virtual bool init(int index);
    
    void closeFun(Ref *pSender, Widget::TouchEventType type);
    void sexChangeFun(Ref *pSender, Widget::TouchEventType type);
    void selectPayFun(Ref *pSender, Widget::TouchEventType type);
    void changeMonthTextFun(Ref *pSender, Widget::TouchEventType type);
    void showInviteShareMask(LayerColor * layer);
    void portraitChange(Ref *pSender);
    
    void getVipJson(Json::Value json);
    //vip购买的遮罩层显示（代码太长，分离出来）
    void showVipBuyView();
    
private:
    Json::Value vipJson;
    LayerColor *_layer1;
    Layout *selectPay;
    Text *payMoney;
    int monthCount;
    Text *monthText;
    
    Button *reduceBut;
    Button *addBut;
    
};


#endif /* MaskSelectLayer_hpp */
