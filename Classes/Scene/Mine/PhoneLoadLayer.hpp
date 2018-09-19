//
//  LoadLayer.hpp
//  ZJH
//
//  Created by apple on 16/7/8.
//
//

#ifndef PhoneLoadLayer_hpp
#define PhoneLoadLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"


using namespace cocos2d::ui;
using namespace cocos2d;

class  PhoneLoadLayer: public BaseLayer
{
    
public:

    
    PhoneLoadLayer();
    virtual bool init();
    CREATE_FUNC(PhoneLoadLayer);
    //登录响应
    void loadOn(Ref *pSender);
    //忘记密码
    void lostPassWordOn(Ref *pSender, Widget::TouchEventType type);
    //快速登录
    void speedLoadOn(Ref *pSender, Widget::TouchEventType type);
    //登录请求
    void postLogin(Json::Value json);
    
    void rePostLoad(Ref*ref);
    
private:
    
    Json::Value loadJson;
    
    EditBox *textField1;
    EditBox *textField2;
    Button *nextBtn;
    
};

#endif /* PhoneLoadLayer_hpp */
