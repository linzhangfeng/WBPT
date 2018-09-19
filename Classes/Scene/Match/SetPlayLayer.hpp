//
//  TipChangeEditLayer.hpp
//  ZJH
//
//  Created by apple on 16/7/23.
//
//

#ifndef SetPlayLayer_hpp
#define SetPlayLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"

using namespace cocos2d::ui;
using namespace cocos2d;


class  SetPlayLayer: public Layer , public cocos2d::ui::EditBoxDelegate
{
public:

    SetPlayLayer();
    static SetPlayLayer* create();
    virtual bool init();
    void showView(Json::Value wayJson);
    
    Button *selectBtnDraw(string titleStr, bool selected, int cellType = 1);
    
    void type4Edit(Layout *cell, Json::Value json, int tag);
    void type3Down(Layout *cell, Json::Value json, int tag);
    void showDownListView(Ref*ref);
    
    void selectBtnOn(Ref*ref);
    void setFinishBtnOn(Ref*ref);
    void refreshListView();
    void onClickDownItem(Ref*ref);
    void closeSelectView(Ref*ref);
    
    
    virtual void editBoxEditingDidBegin(EditBox* editBox);
    virtual void editBoxTextChanged(EditBox* editBox, const std::string& text);
    virtual void editBoxReturn(EditBox* editBox);
    virtual void editBoxEditingDidEnd(EditBox* editBox);
    
private:
    
    int selectTag;
    Json::Value setData;
    std::map<int, std::vector<Button*>>butMap;
    ListView *listV;
    
    std::vector<Json::Value> hideList;
    
    std::vector<EditBox*> editBoxList;
    
    std::map<int, Layout*> downCell;
    
    std::map<int, bool> isDownShow;
    
    ImageView *listBg;
    
};

#endif /* SetPlayLayer_hpp */
