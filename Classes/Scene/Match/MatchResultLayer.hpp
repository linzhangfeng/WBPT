//
//  MatchResultLayer.hpp
//  ZJH
//
//  Created by apple on 16/8/17.
//
//

#ifndef MatchResultLayer_hpp
#define MatchResultLayer_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"
#include "../Loading/Loading.h"

using namespace cocos2d::ui;
using namespace cocos2d;

class  MatchResultLayer: public BaseLayer
{
    
public:
    virtual bool init();
    CREATE_FUNC(MatchResultLayer);
    
private:
    void postRecord(float dt);
    void postRecordDetail(float dt);
    void showView(Json::Value json);
    void showRecordList(Json::Value json);
    void addDataInList(Json::Value json);
    
    ListView *listRecord;
    bool isFirstPost;
    int pageIndex;
    int recordIndex;
    
    //游戏游戏胜率
    void gotoWinRate(Ref *pSender);
    //对局玩家
    void gotoMatchPlayer(Ref *pSender);
    
    void updateMatchPlayerNum(Ref*ref);
    
    void updatePlayAllAddNum(Ref*ref);
    
    void gotoRecordDetail(Ref *pSender);
    
    void showVideoInput();//输入查看录像的房间id
    
    void requestVideoInfo(Json::Value value);
    
    RichText *playerRichText;
    Layout *parant;
    
    int num;
    
    std::vector<Json::Value> recordJson;
    Loading *loading;
    Loading *loading2;
    
    Text *numT;
};


#endif /* MatchResultLayer_hpp */
