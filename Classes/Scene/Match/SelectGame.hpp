//
//  SelectGame.hpp
//  ZJH
//
//  Created by apple on 16/8/17.
//
//

#ifndef SelectGame_hpp
#define SelectGame_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ZJh.h"
#include "../Loading/Loading.h"

using namespace cocos2d::ui;
using namespace cocos2d;

class  SelectGame: public Layer, public EditBoxDelegate
{
    
public:
    virtual bool init();
    CREATE_FUNC(SelectGame);
    void requestData();
    virtual void onExit();
    void getData(float dt);
    void showLoadErrorDialog();
    void showListView(Json::Value json);
    void initTab();
    void showTab(int index);
    
    void requestSearchData();
    void showSearchListView(std::vector<int> allIndex);
    
    virtual void editBoxEditingDidBegin(EditBox* editBox);
    virtual void editBoxEditingDidEnd(EditBox* editBox);
    virtual void editBoxTextChanged(EditBox* editBox, const std::string &text);
    virtual void editBoxReturn(EditBox *editBox);
    
    Layout* createItem(std::string img,std::string gamename,std::string tabname, int tag);
    Layout* createSearchTitle();
private:
    Json::Value configList;
    Loading* loading;
    int curID;
    EditBox* _editName;
    std::vector<Layout*> allTabs;
};
#endif /* SelectGame_hpp */
