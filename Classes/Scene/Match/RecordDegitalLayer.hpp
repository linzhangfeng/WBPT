//
//  RecordDegitalLayer.hpp
//  ZJH
//
//  Created by apple on 16/7/16.
//
//

#ifndef RecordDegitalLayer_hpp
#define RecordDegitalLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJH.h"

using namespace cocos2d::ui;
using namespace cocos2d;

class  RecordDegitalLayer: public BaseLayer
{
    
public:
    
    virtual bool init();
    CREATE_FUNC(RecordDegitalLayer);
    
    void show(Json::Value json);
    void getData(float dt);
    
    void showView(Json::Value json);
    void showShareWaitView();
    
    void createNewView();
    
private:
    void shareBtnOn(Ref*ref);

    void showHidenView(Json::Value json);
    void showDownView(Json::Value json);
    void shareVideo();
    
    void hideOn(Ref *pSender, Widget::TouchEventType type);
    void reviewOn(Ref *pSender, Widget::TouchEventType type);
    Json::Value jsonData;
    
    //隐藏的cell对象
    Layout *layout2Cell;
    
    bool isShow;
    
    ListView *viewList;
    
    std::vector<Widget*> temp_list;
    
    Layout *newList;
    
    void removeSaveImage(Ref*ref);
    
    Layout *hideCell;
    
    string pathImage;
    
    Button *shareBtn;
    int num;
    
    int roomID;
    int gameType;
};

#endif /* RecordDegitalLayer_hpp */
