//
//  LoadTable.h
//  ZJH
//
//  Created by hyhy on 16/7/12.
//
//

#ifndef __ZJH__LoadTable__
#define __ZJH__LoadTable__

#include "ZJH.h"

class LoadTable : public Layer
{
public:
    static cocos2d::Scene *createScene();
    static LoadTable *createWithData();
    bool initWithData();
    
    void updateGame(float dt);
    void cancelCallback();
};

#endif /* defined(__ZJH__LoadTable__) */
