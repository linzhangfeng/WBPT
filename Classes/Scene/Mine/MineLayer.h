//
//  FriendLayer.hpp
//  wanMain
//
//  Created by apple on 16/6/23.
//
//

#ifndef MineLayer_h
#define MineLayer_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"
#include "../BaseLayer.hpp"

using namespace cocos2d::ui;
using namespace cocos2d;

class MineLayer: public BaseLayer
{
public:
    
    MineLayer();
    virtual bool init();
    CREATE_FUNC(MineLayer);
    
    void updateView(Ref* json);
    
    void showDaiLiWeiXinView();
    
private:
    //new
    void showInfoView();
    void showLvView(Ref* json);
    void clickList_s(Ref *pSender, Widget::TouchEventType type);
    void butClickOn(Ref* ref, Widget::TouchEventType type);
    void createButMap();
    void updatePortrait(Ref*ref);
    void updateDailyReceive(Ref*ref);
    void updateNick(Ref*ref);
    void tabClickFun(Ref* ref);
    
    void fastBuyVip(Ref* ref);
    void fastLoad(Ref*ref);
    
    
    //用按钮名字生成列表
    ListView *generateListView(std::vector<std::string> btnNames);
    
    //游客
    void youKeView();
    //注册账户
    void registView();
    //VIP
    void VIPView();
    //贵族
    void richView();
    //族长
    void leaderView();
    
    Layout *lastClick;
    ListView *listView_s;
    ListView *listView_do;
    std::map<string, Button*>butMap;
    Sprite *userPortrit;
    Layout *portritBg;
    ImageView *infoBg;
    
    Text *name;
    Text *IDText;
    
    Json::Value infoList;
    Layout* listLayer1;
    std::string nameStr;
    Button *lastBtn;

};
#endif /* FriendLayer_hpp */
