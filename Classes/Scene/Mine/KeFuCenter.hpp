//
//  KeFuCenter.hpp
//  ZJH
//
//  Created by apple on 16/7/1.
//
//

#ifndef KeFuCenter_hpp
#define KeFuCenter_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"
#include "../BaseLayer.hpp"
#include "../Loading/Loading.h"

using namespace cocos2d::ui;
using namespace cocos2d;

class KeFuCenter: public BaseLayer
{
    
public:
    
    virtual bool init();
    CREATE_FUNC(KeFuCenter);
    void requestData();
    void getData(float dt);
    void showListView(Json::Value json);
    void showLoadErrorDialog();
    Layout* createItem(Json::Value json);
    void createTitle();
    void createCallPhone();
private:
    Json::Value configList;
    Loading* loading;
};


#endif /* KeFuCenter_hpp */
