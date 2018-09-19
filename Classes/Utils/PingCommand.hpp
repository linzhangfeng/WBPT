//
//  PingCommand.hpp
//  ZJH
//
//  Created by mac on 2016/12/20.
//
//

#ifndef PingCommand_hpp
#define PingCommand_hpp

#include <stdio.h>
#include "cocos2d.h"

class PingCommand : cocos2d::Ref
{
public:
    long long lastTime;
    int interval;
    float baiduRespTime;
    static PingCommand* getInstance();
    void start();
    void getData(float dt);
};

#endif /* PingCommand_hpp */
