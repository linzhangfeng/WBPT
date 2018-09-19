//
//  RoomHelp.h
//  ZJH
//
//  Created by hyhy on 16/7/25.
//
//

#ifndef __ZJH__RoomHelp__
#define __ZJH__RoomHelp__

#include "cocos2d.h"
#include "ZJH.h"

class RoomHelp : public cocos2d::Layer
{
public:
    CREATE_FUNC(RoomHelp);
    virtual bool init();
    void initHelp(Json::Value &json);
    
    void initHelpWeb();
    
    bool onTouchBegan(Touch* touch, Event* event);
    
private:
    ListView* createConfigView(Size size);
    Layout * selectBtnDraw(string titleStr, bool selected, int cellType);
};

#endif /* defined(__ZJH__RoomHelp__) */
