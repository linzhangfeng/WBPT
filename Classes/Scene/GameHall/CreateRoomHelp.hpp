//
//  TaskLayer.hpp
//  ZJH
//
//  Created by apple on 16/7/6.
//
//

#ifndef CreateRoomHelp_hpp
#define CreateRoomHelp_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"
#include "../Loading/Loading.h"

using namespace cocos2d::ui;
using namespace cocos2d;


class  CreateRoomHelp: public Layer
{
public:
    virtual bool init();
    void showView(Json::Value json );
    void postGameData(Json::Value jsonDate);
    void createView();
    void createData(Json::Value Data,int index);
    CREATE_FUNC(CreateRoomHelp);
    void setBaoView();
    void onBaoBtnResponse(Ref*ref);
    void onMoreResponse(Ref*ref);
    void onShouResponse(Ref*ref);
    void onExit();
private:
    Sprite * bg;
    Button *lastBaoBtn;
    int baoIndex;
    Json::Value setData;
    cocos2d::experimental::ui::WebView *webView;
    Loading *loading1;
};


#endif /* TaskLayer_hpp */
