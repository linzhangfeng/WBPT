//
//  Play.hpp
//  ZJH
//
//  Created by zhangzhenhua on 16/7/23.
//
//

#ifndef Play_hpp
#define Play_hpp

#include <stdio.h>
#include "ZJH.h"
#include "../BaseLayer.hpp"

class Play : public BaseLayer
{
public:
    CREATE_FUNC(Play);
    bool init();
    void initTopLay();
    void initBottomLay();
    void initListView();
    
    void initListData(Json::Value _json);
    void initListSecData(Json::Value _json);
    void countScorll(float time);
    void CalculationPosition();
    void RequestData();
    
    void itemMCallBack(Json::Value _json);
    void postMoney();
private:
    ListView *listView;
    int firstx;
    int endx;
    bool flag;
    int decount;
    
    bool levelFlag;
    bool running;
    int runningCount;
    Button *imgBack;
    Json::Value _jsonData;
    
    Button* _play;
    Button* _froom;
    Button* _fon;
    
    Text* userMoneyNumber;
    Text* userWanbiNumber;
};

class playItem: public Layout{
public:
    static Layout* getItemLayout(int index,const MyCallBack_Int &callback,Json::Value _json);
};

class playSecItem: public Layout{
public:
    static Layout* getItemLayout(int index,Json::Value _json,const MyCallBack_Int &callback);
};
#endif /* Play_hpp */
