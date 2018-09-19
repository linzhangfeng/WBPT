//
//  InviteLayer.hpp
//  ZJH
//
//  Created by apple on 16/7/1.
//
//

#ifndef InviteLayer_hpp
#define InviteLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJH.h"

using namespace cocos2d::ui;
using namespace cocos2d;

class  InviteLayer: public BaseLayer
{
public:
    virtual bool init();
    CREATE_FUNC(InviteLayer);
    
    void tabClickFun(Ref *pSender);
    
    void showPage1();
    void showPage2();
    
    Button *getReceiveBtn();
    void inviteCodeInto(string code);
    
private:
    Layout *Bglayout;
    Button* lastBut;
    Button* receiveBtn2;
};


#endif /* InviteLayer_hpp */
