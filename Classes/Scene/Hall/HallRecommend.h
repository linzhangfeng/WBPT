//
//  HallRecommend.h
//  ZJH
//
//  Created by hyhy on 16/7/4.
//
//

#ifndef __ZJH__HallRecommend__
#define __ZJH__HallRecommend__

#include "ZJH.h"

class HallRecommend : public Layer
{
public:
    CREATE_FUNC(HallRecommend);
    virtual bool init();
    
    void initList();
    Widget *getActivityList(Json::Value &data);
    Widget *getHotList(Json::Value &data);
    Widget *getAdList(Json::Value &data);
    Widget *getLayViewPoint(int num);
    
    void pageViewEvent(Ref *pSender, PageView::EventType type);
    void changePointView(int index);
    
    ListView* getHorizeListView(Json::Value &data);
    void RichListView(int sel);
    void setCallFun(const MyCallBack_Int &callback);
    
    void RefreshView();
    void RefreshDaTing(Ref* ref);
private:
    Json::Value m_jsonData;
    Widget* _widgetPoint;
    int adpageviewCount;
    int adimage_height;
    
    MyCallBack_Int  callFun;
    ListView *listView;
    Json::Value _pay;
    bool refreshing;
};

class ActivityListItem : public cocos2d::ui::Widget
{
public:
    static ActivityListItem *createWithData(Json::Value &info);
    bool initWithData(Json::Value &info);
};

class HotListItem : public cocos2d::ui::Widget
{
public:
    static HotListItem *createWithData(Json::Value &info);
    bool initWithData(Json::Value &info);
    ImageView *getImageView();
};

#endif /* defined(__ZJH__HallRecommend__) */
