//
//  SettingPassWordLayer.hpp
//  ZJH
//
//  Created by apple on 16/7/8.
//
//

#ifndef registFinish_hpp
#define registFinish_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"

using namespace cocos2d::ui;
using namespace cocos2d;

class  RigistFinishLayer: public BaseLayer
{
    
public:
    
    virtual bool init();
    CREATE_FUNC(RigistFinishLayer);
    
    //确认设置密码提交注册
    void registFinish(Ref *pSender, Widget::TouchEventType type);
    void getInfoJson(Json::Value json);
    void setPassWordFinish(Ref *pSender);
    
    void postLogin();
    
private:
    EditBox *textField1;
    Json::Value acountJson;
    
};


#endif /* SettingPassWordLayer_hpp */
