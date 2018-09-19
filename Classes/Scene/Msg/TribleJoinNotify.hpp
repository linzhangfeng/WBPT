//
//  TribleJoinNotify.hpp
//  ZJH
//
//  Created by Jack on 16/9/27.
//
//

#ifndef TribleJoinNotify_hpp
#define TribleJoinNotify_hpp

#include <stdio.h>
#include "ZJH.h"
#include "../BaseLayer.hpp"
#include "../../Model/MsgModel.h"
//加群申请的UI
class TribleJoinNotify : public BaseLayer{
public:
    CREATE_FUNC(TribleJoinNotify);
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

class TribleJoinItem : public Layout{
public:
    CREATE_FUNC(TribleJoinItem);
    bool init();
    void initView(MsgModel::Chat* chat);
    void getData(float dt);
    void sendHttpRequest(int targetId,int action,int userId);
    ~TribleJoinItem();
private:
    int action;
    string tag;
    MsgModel::Chat* chat;
    void updateUI(int result);
};
#endif /* TribleJoinNotify_hpp */
