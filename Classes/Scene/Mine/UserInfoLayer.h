//
//  UserInfoLayer.hpp
//  wanMain
//
//  Created by apple on 16/6/29.
//
//

#ifndef UserInfoLayer_hpp
#define UserInfoLayer_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"
#include "../BaseLayer.hpp"
#include "../Loading/Loading.h"

using namespace cocos2d::ui;
using namespace cocos2d;


class  UserInfoLayer: public BaseLayer
{
    
public:

    UserInfoLayer();
    
    virtual bool init();
    CREATE_FUNC(UserInfoLayer);
    
private:
    
    void showView(Json::Value data);
    void portraitFun(Ref *pSender, Widget::TouchEventType type);
    void qrcodeFun(Ref *pSender, Widget::TouchEventType type);
    void nameFun(Ref *pSender, Widget::TouchEventType type);
    void sexFun(Ref *pSender, Widget::TouchEventType type);
    void likeFun(Ref *pSender, Widget::TouchEventType type);
    void homeFun(Ref *pSender, Widget::TouchEventType type);
    void selfTextFun(Ref *pSender, Widget::TouchEventType type);
    void updateAdress(Ref*ref);
    void updatePortrait(Ref*ref);
    void updateNick(Ref*ref);
    void updateSigna(Ref*ref);
    void updateHobby(Ref*ref);
    
    void checkBangWeiXinState();
    
    void postData(float dt);
    
    //用户信息数据
    Json::Value userData;
    
    Text *nickText;
    Text *signatureText;
    Text *hobbyText;
    Text *adressText;
    
    Layout *portraiCell;
    Layout *nickCell;
    ImageView *portraiArrow;
    ImageView *nickArrow;
    Text *portraiText;
    
    bool isBangWX;
    
    Loading *loading2;
    
    Button* wx_tongbu;
    Button* wx_tongbuing;
    Button* wx_tongbuok;
    void sysWeiXinInfo(Ref*ref);
    void thirdWXPost(Ref*ref);
    
    virtual void onExit();
};
#endif /* UserInfoLayer_hpp */
