//
//  SettingLayer.hpp
//  ZJH
//
//  Created by apple on 16/7/7.
//
//

#ifndef SettingLayer_hpp
#define SettingLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"

using namespace cocos2d::ui;
using namespace cocos2d;

class  SettingLayer: public BaseLayer
{
    
public:
    
    //登录的
    void loadView();
    //未登录的
    void unLoadView();
    // 退出登录
    void exitLoadOn(Ref *pSender, Widget::TouchEventType type);
    
    //未登录的时候手机号cell去登录
    void cellPhoneGotoLoad(Ref *pSender, Widget::TouchEventType type);
    //登录时，去修改手机号
    void cellUpdatePhone(Ref *pSender, Widget::TouchEventType type);
    
    //登录时，去修改密码
    void cellUpdatePassWord(Ref *pSender, Widget::TouchEventType type);
    
    // 绑定微信
    void checkBindState();
    void bindWeiXinOn(Ref *pSender);
    void phoneWXBindPost(Ref* pSender);
    
    virtual bool init();
    CREATE_FUNC(SettingLayer);
    void thirdLoginPost();
    void updateView(Ref* json);
    
    void updateStateView(Ref* json);
    
    void postUserInfo();
    
private:
    
    void swithClickFun(Ref *pSender, Widget::TouchEventType type);
    ListView* listView;
    
    Layout *barBg;
    Layout *lockBg;
    Layout *noticeBg;
    
    bool isWXBind;
    
    Layout *cellItem(int index = 1, string leftStr = "", string leftIcon = "", string rightStr = "");
    Button *exitBtn;
    

};
#endif /* SettingLayer_hpp */
