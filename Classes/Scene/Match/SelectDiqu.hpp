//
//  SelectDiqu.hpp
//  ZJH
//
//  Created by apple on 16/8/17.
//
//

#ifndef SelectDiqu_hpp
#define SelectDiqu_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ZJh.h"
#include "../Loading/Loading.h"

using namespace cocos2d::ui;
using namespace cocos2d;

class  SelectDiqu: public Layer, public EditBoxDelegate
{
    
public:
    virtual bool init();
    CREATE_FUNC(SelectDiqu);
    void requestData();
    virtual void onExit();
    void gotoInitView(float dt);
    void getData(float dt);
    void showLoadErrorDialog();
    void showListView(Json::Value json);
    void showmore();
    void initOverSelect();
    
    void requestSearchData();
    void showSearchListView(std::vector<int> allIndex);
    
    virtual void editBoxEditingDidBegin(EditBox* editBox);
    virtual void editBoxEditingDidEnd(EditBox* editBox);
    virtual void editBoxTextChanged(EditBox* editBox, const std::string &text);
    virtual void editBoxReturn(EditBox *editBox);
    
    Layout* createKey(std::string gamename);
    Layout* createItem(std::string gamename);
    Layout* createSearchTitle();
private:
    Json::Value configList;
    Loading* loading;
    EditBox* _editName;
    int curIndex;
    std::string cur_key;
};
#endif /* SelectDiqu_hpp */
