//
//  TaskLayer.hpp
//  ZJH
//
//  Created by apple on 16/7/6.
//
//

#ifndef TaskLayer_hpp
#define TaskLayer_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ZJh.h"


using namespace cocos2d::ui;
using namespace cocos2d;


class  TaskLayer: public Layer
{
public:
    virtual bool init();
    CREATE_FUNC(TaskLayer);
    
private:
    
    void tabClickFun(Ref *pSender);
    void showOne();
    void showGrow();
    
    Layout *lastBut;
    Layout *left;
    Layout *right;
    Layout *listBg;
};


#endif /* TaskLayer_hpp */
