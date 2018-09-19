//
//  ForgetPass2Layer.hpp
//  ZJH
//
//  Created by apple on 16/7/11.
//
//

#ifndef ForgetPass2Layer_hpp
#define ForgetPass2Layer_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"

using namespace cocos2d::ui;
using namespace cocos2d;

class  ForgetPass2Layer: public BaseLayer
{
    
public:
    
    virtual bool init();
    CREATE_FUNC(ForgetPass2Layer);
    
    //确认设置密码提交
    void finish(Ref *pSender, Widget::TouchEventType type);
    void getInfoJson(Json::Value json);
    
private:
    EditBox *textPass1;
    EditBox *textPass2;
    
    Json::Value acountJson;

};

#endif /* ForgetPass2Layer_hpp */
