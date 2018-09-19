//
//  wanBeiShop.hpp
//  ZJH
//
//  Created by apple on 16/6/30.
//
//

#ifndef InviteFriendLayer_hpp
#define InviteFriendLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"
#include "../Loading/Loading.h"

using namespace cocos2d::ui;
using namespace cocos2d;

class  InviteFriendLayer: public cocos2d::ui::Widget
{
    
public:
    
    virtual bool init();
    CREATE_FUNC(InviteFriendLayer);
    
    void showPage1(Json::Value json);
    void gotoFriendList(Ref *pSender, Widget::TouchEventType type);

    void postData(float dt);
    void shareMaskLayerFun(Ref *pSender, Widget::TouchEventType type);
    
    void btnClick(Ref*ref);
    
    void mianDuiMianErWeiMaView();
    
private:

    Json::Value dataJson;
    Loading *loading;

};


#endif /* WanBeiShop_hpp */
