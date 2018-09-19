//
//  MatchCurrentLayer.hpp
//  ZJH
//
//  Created by apple on 16/8/17.
//
//

#ifndef MatchCurrentLayer_hpp
#define MatchCurrentLayer_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"
#include "../Loading/Loading.h"

using namespace cocos2d::ui;
using namespace cocos2d;

class  MatchCurrentLayer: public cocos2d::ui::Widget
{
    
public:
    virtual bool init();
    CREATE_FUNC(MatchCurrentLayer);
private:
    
    void showView(Json::Value json);
    void postCurrent(float dt);
    void currentItemJoinRoom(Ref *pSender);
    
    Json:: Value jsonlist;
    Loading *loading;
    
};
#endif /* MatchCurrentLayer_hpp */
