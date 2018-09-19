//
//  HotList.h
//  ZJH
//
//  Created by hyhy on 16/7/7.
//
//

#ifndef __ZJH__HotList__
#define __ZJH__HotList__

#include "ZJH.h"
#include "../BaseLayer.hpp"

class HotList : public BaseLayer
{
public:
    CREATE_FUNC(HotList);
    bool init();
    void getData(float dt);
    void selectGameType(int type);
    void initList(int type);
    void initTable();
    void clickImage(Ref *sender);
private:
    Node *m_pLoading;
    Text *m_pNoData;
    Json::Value m_jsonData;
    Sprite *m_pSelectedFlag;
    
    std::map<int, ListView*> listViewMap;
    std::map<int, Json::Value> listViewData;
    std::map<int, int> listViewPage;
    int m_curType;
};

#endif /* defined(__ZJH__HotList__) */
