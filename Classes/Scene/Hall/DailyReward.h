//
//  DailyReward.hpp
//  ZJH
//
//  Created by zhangzhenhua on 16/7/14.
//
//

#ifndef DailyReward_hpp
#define DailyReward_hpp

#include <stdio.h>
#include "ZJH.h"
#include "../BaseLayer.hpp"

class DailyReward : public Layout
{
public:
    CREATE_FUNC(DailyReward);
    bool init();
    void initView();
    Layout* getLineLayout(int height);
    void addJsonData(Json::Value m_value);
    void cgettitleColor(int newindex);
    void sendGetSignreward();
    void sendSignreward();
private:
    Layout* midlay;
    Size midLaySize;
    ListView* dayListView;
    Button* button;
    Text* vipMoney;
    Text* nobleMoney;
    Text* patriarchimageMoney;
    int inde;
    Layout* laytop;
};

class rewardListItem : public Layout
{
public:
    static rewardListItem *createWithData(int index,Json::Value m_info,int width,const MyCallBack_Int &callback);
    bool initWithData(int index,Json::Value m_info,int width,const MyCallBack_Int &callback);
    Layout* getLineLayout(int width,int height);
};

#endif /* DailyReward_hpp */
