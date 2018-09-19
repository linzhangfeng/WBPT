//
//  CreateRoom.hpp
//  ZJH
//
//  Created by apple on 16/7/13.
//
//

#ifndef CreateRoom_hpp
#define CreateRoom_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"
#include "../Loading/Loading.h"

using namespace cocos2d::ui;
using namespace cocos2d;

class  CreateRoom: public Layer, public cocos2d::ui::EditBoxDelegate
{
    
public:
    
    virtual bool init();
    CREATE_FUNC(CreateRoom);
    
    void showView(Json::Value json );
    
    void clickTime(Ref *pSender, Widget::TouchEventType type);
    void clickBottom(Ref *pSender, Widget::TouchEventType type);
    void sureOpen(Ref *pSender);
    void showUpdateView();
    
    void postData(Json::Value jsonDate);
    
    void postGameData(Json::Value jsonDate);
    
    void showSetViewData(bool isQie = false);
    
    void showListView();
    
    //设置游戏配置的缓存
    void setConfigCacheByGameId();
    
    //通过游戏id获取游戏配置缓存
    Json::Value getConfigCacheByGameId();
    
    //清空本地缓存
    void clearConfigCache();
    
    //set
    Layout *selectBtnDraw(string titleStr, bool selected, int cellType);
    Layout *juShuBtnDraw(bool selected, int index);
    
    void setBaoView();
    
    void type4Edit(Layout *cell, Json::Value json, int tag);
    void type3Down(Layout *cell, Json::Value json, int tag);
    void showDownListView(Ref*ref);
    
    void selectBtnOn(Ref*ref);
    void setFinishBtnOn(Ref*ref);
    void onClickDownItem(Ref*ref);
    void closeSelectView(Ref*ref);
    
    void onBaoBtnResponse(Ref*ref);
    void onMoreResponse(Ref*ref);
    void onShouResponse(Ref*ref);
    
    float getSetViewH();
    void getSureSetString();
    
    virtual void editBoxEditingDidBegin(EditBox* editBox);
    virtual void editBoxTextChanged(EditBox* editBox, const std::string& text);
    virtual void editBoxReturn(EditBox* editBox);
    virtual void editBoxEditingDidEnd(EditBox* editBox);
    
    void countNewView();
    
    void showCannotCreateTip(string tipString);
    void onExit();
private:
    int type;
    Sprite* mbg;
    Json::Value duijuList;
    
    Text *lastTime;
    
    //    EditBox *roomName;
    ImageView *markLast;
    Json::Value gameJson;
    
    int time;
    
    Json:: Value bottomJson;
    
    //时间选中的图片
    ImageView *timeSelect;
    //底注选中的图片
    ImageView *bottomSelect;
    Text *price;
    Text *feiyong;
    Text *feiyong_tip;
    Layout* layInfo;
    
    Json:: Value bottomArray;
    Json:: Value timeArray;
    Json:: Value gameConfig;
    
    Layout* layout3;
    Layout* layout4;
    
    Text *base;
    Text *bottom;
    ImageView *playSetArrow;
    
    string playSetStr;
    Layout *createBg;
    ListView *listBg;
    Layout *setBg;
    
    Json::Value setData;
    //服务器原始数据 用于md5对比版本用
    Json::Value serverData;
    string serverMd5;
    
    Json::Value config;
    
    //set
    int selectTag;
    
    std::map<int, std::vector<Layout*>>butMap;
    
    std::vector<Json::Value> hideList;
    
    std::vector<EditBox*> editBoxList;
    
    std::map<int, Layout*> downCell;
    
    std::map<int, bool> isDownShow;
    
    float lastCellH;
    
    Button *lastBaoBtn;
    int baoIndex;
    
    float lastOffY;
    
    Loading *loading;
    Loading *loading1;
    
    void LvMove(Ref *pSender, Widget::TouchEventType type);
    
    int countLastIndex;
    
    Layout* feibg;
    
    //纪录上一次选中刷新的设置的值
    
    //上一次点击的选项
    int baoL;
    int classL;
    int smallL;
    
    bool is4Name;
    Text *countText;
    
    
};


#endif /* CreateRoom_hpp */
