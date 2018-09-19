//
//  LoadLayer.hpp
//  ZJH
//
//  Created by apple on 16/7/8.
//
//

#ifndef LoadLayer_hpp
#define LoadLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"


using namespace cocos2d::ui;
using namespace cocos2d;

class  LoadLayer: public BaseLayer
{
    
public:
    
    virtual bool init();
    CREATE_FUNC(LoadLayer);
    //微信授权
    void loadByWeiXin();
    //微信登录
    void thirdWXLoginPost(Ref* pSender);
    
    void userAgreementView();
    
    void exitTipView();
    
    void webAgreePost(Ref*ref);
    
    void showAgreementWebView(string url);
    
    void dealAppUpdateLoadView();
};

#endif /* LoadLayer_hpp */
