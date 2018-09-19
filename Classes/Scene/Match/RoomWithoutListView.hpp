//
//  RoomWithoutListView.hpp
//  ZJH
//
//  Created by niklaus on 2017/6/24.
//
//

#ifndef RoomWithoutListView_hpp
#define RoomWithoutListView_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJH.h"

using namespace cocos2d::ui;
using namespace cocos2d;

class RoomWithoutListView : public Layout{
public:
    CREATE_FUNC(RoomWithoutListView);
    virtual bool init();
    
    virtual void onExit();
private:
    int pageIndex;
    int pageSize = 20;
    bool hasMore = true;
    ListView* mListView;
    
    void initUI();
    Layout* createItem(Json::Value json);
    
    void loadData();
    void postData(float dt);
    
    void updateList(Json::Value json);
    void addMoreList(Json::Value json);
    
    void doRefresh(Ref* arg);
};

#endif /* RoomWithoutListView_hpp */
