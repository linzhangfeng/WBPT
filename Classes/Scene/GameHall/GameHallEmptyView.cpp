//
//  GameHallEmptyView.cpp
//  ZJH
//
//  Created by mac on 2017/6/23.
//
//

#include "GameHallEmptyView.hpp"
bool GameHallEmptyView::init()
{
    if (!Layout::init())
    {
        return false;
    }
    Size winSize = Director::getInstance()->getWinSize();
    setAnchorPoint(Vec2(0,0));
    setPosition(Vec2(0,0));
    setContentSize(winSize);
    log("===GameHallEmptyView====");
    return true;
}
