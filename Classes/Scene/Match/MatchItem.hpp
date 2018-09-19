//
//  MatchItem.hpp
//  ZJH
//
//  Created by apple on 16/7/14.
//
//

#ifndef MatchItem_hpp
#define MatchItem_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"

using namespace cocos2d::ui;
using namespace cocos2d;

class MatchItem : public cocos2d::ui::Widget
{
public:
    static MatchItem *createWithData(Json::Value &info);
    bool initWithData(Json::Value &info);
    
};

class recordeItem : public cocos2d::ui::Widget
{
public:
    static recordeItem *createWithData(Json::Value &info);
    bool initWithData(Json::Value &info);
    void dealTimeFormate(long long sec, Text *text1, Text *text2);
};

#pragma mark [ WinRateItem ]
class WinRateItem : public cocos2d::ui::Widget
{
public:
    static WinRateItem *createWithData(Json::Value &info);
    bool initWithData(Json::Value &info);
};

#pragma mark [ MatchPlayerItem ]
class MatchPlayerItem : public cocos2d::ui::Widget
{
public:
    static MatchPlayerItem *createWithData(Json::Value &info);
    bool initWithData(Json::Value &info);
    
    Layout *getDeleteLayout();
    
private:
    Layout *deleteLayout;
    void addFriend(Ref *pSender, Widget::TouchEventType type);
    
};

#pragma mark [ RecordDetailItem ]
class RecordDetailItem : public cocos2d::ui::Widget
{
    
public:
    static RecordDetailItem *createWithData(Json::Value &info, int rate);
    bool initWithData(Json::Value &info, int rate);

};

#pragma mark [ LuXiangItem ]
class LuXiangItem : public Layout
{
    
public:
    static LuXiangItem *createWithData(Json::Value &info);
    bool initWithData(Json::Value &info);

private:
    
    void postReviewData(Json::Value jsonData);
    
};

#endif /* MatchItem_hpp */
