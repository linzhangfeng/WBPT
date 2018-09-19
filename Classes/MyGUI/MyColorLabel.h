//
//  MyColorLabel.h
//  ZJH
//
//  Created by hyhy on 16/3/31.
//
//

#ifndef __ZJH__MyColorLabel__
#define __ZJH__MyColorLabel__

#include "cocos2d.h"

//用于拼接几个字符串
class MyColorLabel : public cocos2d::Label
{
public:
    static MyColorLabel *createWithVector(cocos2d::Vector<cocos2d::Label *> labels);
    virtual bool init(cocos2d::Vector<cocos2d::Label *> labels);
    
};

#endif /* defined(__ZJH__MyColorLabel__) */
