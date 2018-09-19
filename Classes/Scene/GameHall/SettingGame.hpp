//
//  SettingGame.hpp
//  ZJH
//
//  Created by mac on 2017/1/2.
//
//

#ifndef SettingGame_hpp
#define SettingGame_hpp

#include "cocos2d.h"
#include "ZJH.h"

class SettingGame : public cocos2d::Layer
{
public:
    CREATE_FUNC(SettingGame);
    virtual bool init();
    void initSetting();
    bool onTouchBegan(Touch* touch, Event* event);
};

#endif /* SettingGame_hpp */
