//
//  VIPCentreLayer.hpp
//  ZJH
//
//  Created by apple on 16/7/1.
//
//

#ifndef VIPCentreLayer_hpp
#define VIPCentreLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"
#include "../BaseLayer.hpp"

using namespace cocos2d::ui;
using namespace cocos2d;


class  VIPCentreLayer: public BaseLayer
{
    
public:
    
    virtual bool init();
    CREATE_FUNC(VIPCentreLayer);
    
    void showView(Json::Value data);
    
    void buyVipFun(Ref *pSender, Widget::TouchEventType type);
    void teQaunFun(Ref *pSender, Widget::TouchEventType type);
    
    void postVIPInfoRequest(Json::Value);
private:
    Json::Value infoList;
    Json::Value vipInfoList;
};


#endif /* VIPCentreLayer_hpp */
