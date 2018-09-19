//
//  DynamicNotify.hpp
//  ZJH
//  动态通知界面
//  Created by Jack on 16/9/29.
//
//

#ifndef DynamicNotify_hpp
#define DynamicNotify_hpp

#include <stdio.h>
#include "ZJH.h"
#include "../BaseLayer.hpp"
#include "../../Model/MsgModel.h"
class DynamicNotify : public BaseLayer{
public:
    CREATE_FUNC(DynamicNotify);
    bool init();
    void initView();
    void initData(Json::Value &json);
    void getRecordData(bool init);
    
private:
    Size size;
    long long lastTime;
    long long newTime;
    ListView* listView;
    int curPage;
    int pageSize;
    int lastId;
    bool hasMore;
    int _cuid;
    
};

class DynamicNotifyItem : public Layer{
public:
    CREATE_FUNC(DynamicNotifyItem);
    bool init();
    void initView(MsgModel::Chat* chat);
    void getData(float dt);
    ~DynamicNotifyItem();
private:
    int action;
    string tag;
    MsgModel::Chat* chat;
    void initTimeView(long long timeLong);
};
#endif /* DynamicNotify_hpp */
