//
//  Loading.h
//  ZJH
//
//  Created by luochuanting on 13-5-3.
//
//

#ifndef __ZJH__Loading__
#define __ZJH__Loading__

#include "cocos2d.h"

using namespace cocos2d;
using namespace std;

class Loading : public cocos2d::Layer
{
public:
    
    static Loading* startLoading(Node *pNode = NULL,Vec2 p = Vec2(0, 0));
    void stopLoading(Node *pNode);
    
    void maskSwllowTouch();
    
    virtual bool init();
    CREATE_FUNC(Loading);
};

#endif /* defined(__ZJH__Loading__) */
