//
//  MatchBottomView.hpp
//  ZJH
//
//  Created by niklaus on 2017/6/23.
//
//

#ifndef MatchBottomView_hpp
#define MatchBottomView_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJH.h"

using namespace cocos2d::ui;
using namespace cocos2d;

class MatchBottomView : public Layout{
public:
    CREATE_FUNC(MatchBottomView);
    virtual bool init();
    
    virtual void onExit();
    
    PageView* pageView;
    Layout* freeRoomView;
    
    CC_SYNTHESIZE(MyCallBack_Str, m_pCallback, Callback);
    //快速加入房间
    void quickAddRoom(Ref *pSender, Widget::TouchEventType type);
    //展开
    void moreView(Ref *pSender, Widget::TouchEventType type);
    
private:
    string mRoomCode = "";
    Button* addButton;
    void initUI();
    void createItem(Json::Value json);
    
    void loadData();
    void postData(float dt);
    
    void updateList(Json::Value json);
    
    void doRefresh(Ref* arg);
    void doErrorRefresh(Ref* arg);
};

#endif /* MatchBottomView_hpp */
