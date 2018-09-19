//
//  TaskLayer.hpp
//  ZJH
//
//  Created by apple on 16/7/6.
//
//

#ifndef KeFu_hpp
#define KeFu_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"
#include "../Loading/Loading.h"

using namespace cocos2d::ui;
using namespace cocos2d;


class  KeFu: public Layer
{
public:
    virtual bool init();
    CREATE_FUNC(KeFu);
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
    Sprite* bg;
};


#endif /* TaskLayer_hpp */
