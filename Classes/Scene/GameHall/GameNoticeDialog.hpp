//
//  GameNoticeDialog.hpp
//  ZJH
//
//  Created by mac on 2017/3/17.
//
//

#ifndef GameNoticeDialog_hpp
#define GameNoticeDialog_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"
#include "../Loading/Loading.h"

using namespace cocos2d::ui;
using namespace cocos2d;

class  GameNoticeDialog: public Layer
{
public:
    virtual bool init();
    CREATE_FUNC(GameNoticeDialog);
    ClippingNode* clip;
    LabelTTF* notice;
    string noticeText;
    void show(string content);
    void touchCallback(Ref *pSender, Widget::TouchEventType type);
};



#endif /* GameNoticeDialog_hpp */
