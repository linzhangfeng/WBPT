//
//  RoomChargNotify.hpp
//  ZJH
//
//  Created by mac on 2016/12/14.
//
//

#ifndef RoomChargNotify_hpp
#define RoomChargNotify_hpp

#include <stdio.h>
#include "ZJH.h"
#include "../BaseLayer.hpp"
#include "../../Model/MsgModel.h"
class RoomChargNotify : public BaseLayer
{
public:
    CREATE_FUNC(RoomChargNotify);
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

class RoomChargNotifyItem :public Layer{
    
public:
    CREATE_FUNC(RoomChargNotifyItem);
    bool init();
    void initView(MsgModel::Chat* chat);
    Text* initTimeView(long long timeLong);
};


#endif /* RoomChargNotify_hpp */
