//
//  MIneRoomLayer.hpp
//  ZJH
//
//  Created by apple on 16/7/11.
//
//

#ifndef MineRoomLayer_hpp
#define MineRoomLayer_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"

using namespace cocos2d::ui;
using namespace cocos2d;

class  MineRoomLayer: public BaseLayer
{
    
public:
    MineRoomLayer();
    virtual bool init();
    CREATE_FUNC(MineRoomLayer);
    
private:
    void tabClickFun(Ref* ref);
    
    void showLove(Json::Value json);
    void showCreate(Json::Value json);
    void showJoin(Json::Value json);
    
    void postLove();
    void postCreate();
    void postJoin();
    
    
    PageView *pageView;
    int currentPage;
    
    int pageIndexLove;
    int pageIndexJoin;
    int pageIndexCreate;
    
    bool isFirstLove;
    bool isFirstJoin;
    bool isFirstCreate;
    
    ListView *listView1;
    ListView *listView2;
    ListView *listView3;
};


#endif /* MIneRoomLayer_hpp */
