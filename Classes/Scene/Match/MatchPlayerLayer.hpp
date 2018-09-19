//
//  MatchPlayerLayer.hpp
//  ZJH
//
//  Created by apple on 16/7/15.
//
//

#ifndef MatchPlayerLayer_hpp
#define MatchPlayerLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJH.h"

using namespace cocos2d::ui;
using namespace cocos2d;

class  MatchPlayerLayer: public BaseLayer
{
    
public:
    MatchPlayerLayer();
    
    virtual bool init();
    CREATE_FUNC(MatchPlayerLayer);
    
private:
    
    void  postData();
    void deleteOn(Ref *pSender, Widget::TouchEventType type);
    void clickItemMove(Ref *pSender, Widget::TouchEventType type);
    void  showView(Json::Value json);
    
    void canDelete(Ref*ref);
    
    ListView *viewList;
    bool isDelete;
    Layout *_deleteItem;
    int index;
    Json::Value listData;
    
    Layout *createBg;
    
    string frindString;
    
    Layout *curLayout;
    
    
};

#endif /* MatchPlayerLayer_hpp */
