//
//  ClubHomeEmptyView.cpp
//  ZJH
//
//  Created by mac on 2017/5/3.
//
//

#include "ClubHomeEmptyView.hpp"
#include "../../JsUtils/HotUpdateLoadingLayout.hpp"

bool ClubHomeEmptyView::init()
{
    if (!Layout::init())
    {
        return false;
    }
    Size winSize = Director::getInstance()->getWinSize();
    setAnchorPoint(Vec2(0,0));
    setContentSize(winSize);
    log("===ClubHomeEmptyView====");
    HotUpdateLoadingLayout* loading = HotUpdateLoadingLayout::create();
//    loading->watchGameId = 2130;
    loading->watchGameId = 9905;
    loading->tips = "俱乐部更新中(%d/%d)...";
    this->addChild(loading);
    return true;
}
