//
//  GameListView.hpp
//  ZJH
//
//  Created by apple on 16/8/17.
//
//

#ifndef GameListView_hpp
#define GameListView_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ZJh.h"
#include "../Loading/Loading.h"

using namespace cocos2d::ui;
using namespace cocos2d;


class  GameListView: public Layer
{
    
public:
    virtual bool init();
    CREATE_FUNC(GameListView);
    void requestData();
    virtual void onExit();
    void getData(float dt);
    void showLoadErrorDialog();
    void showListView(Json::Value json);
    void updateViewNotify(Ref* r);
    void createDiqu();
private:
    Json::Value configList;
    Loading* loading;
};

#endif /* GameListView_hpp */
