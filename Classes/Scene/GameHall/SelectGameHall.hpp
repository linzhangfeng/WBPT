//
//  SelectGameHall.hpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//

#ifndef SelectGameHall_hpp
#define SelectGameHall_hpp

#include "ZJH.h"
#include "../Loading/Loading.h"

class SelectGameHall : public Layer
{
public:
    static cocos2d::Scene* createScene();
    CREATE_FUNC(SelectGameHall);
    bool init();
    void getData(float dt);
private:
    void initData(Json::Value data);
    void showLoadErrorDialog();
    Json::Value configList;
    Loading *loading;
};

#endif /* SelectGameHall_hpp */
