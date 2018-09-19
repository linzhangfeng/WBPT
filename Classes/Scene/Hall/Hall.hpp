//
//  Hall.hpp
//  ZJH
//
//  Created by admin on 16/3/6.
//
//

#ifndef Hall_hpp
#define Hall_hpp

#include "ZJH.h"
#include "../BaseLayer.hpp"
#include "Moreoptions.h"
#include "OtherGameType.hpp"
#include "OtherGameTypeView.h"
#include "PaymentView.h"
#include "HallRecommend.h"
USING_NS_CC;

class Hall : public BaseLayer
{
public:
    enum HallIndex
    {
        kHallRecommend = 0,
        kHallFriend,
    };
    CREATE_FUNC(Hall);
    bool init();
    void btnCallback(Ref* ref);
    void getData(float dt);
    void initHall();
    void addoptions();
    
    void initTable();
    void clickImage(Ref *sender);
    Layout* getTopTextLay(string tipname,int gametag);
    void callBackToIndex(int gameid);
    void scrollListViewToPosition(Layout* _lay);
    
    void showTopLoading();
    void cahcelTopLoading();
    void showPayMent();
private:
    int m_curIndex;
    Widget *m_pContainer;
    Moreoptions *mp;  //更多
    
    ListView *typeList;
    int oldTag;
    OtherGameType *hf;
    Node *m_pLoading;
    HallRecommend *hr;
};

#endif /* Hall_hpp */
