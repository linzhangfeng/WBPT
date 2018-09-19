//
//  wanBeiShop.hpp
//  ZJH
//
//  Created by apple on 16/6/30.
//
//

#ifndef TuiJianRenLayer_hpp
#define TuiJianRenLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"
#include "../Loading/Loading.h"

using namespace cocos2d::ui;
using namespace cocos2d;


class  TuiJianRenLayer: public cocos2d::ui::Widget
{
public:
    
    virtual bool init();
    CREATE_FUNC(TuiJianRenLayer);
    
    void loadView();
    void showPageView(Json::Value json);
    void showNoPage2(Json::Value json);
    void showPage2IsDraw(Json::Value json);
    
    void postData(float dt);
    void sureOn(Ref *pSender);
    
    void inviteCodeInto(string code);
    
private:
    
    EditBox* textField2;
    bool isNO;
    Loading*loading;
    
};


#endif /* TuiJianRenLayer_hpp */
