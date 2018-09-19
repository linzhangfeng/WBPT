//
//  MyColorLabel.cpp
//  ZJH
//
//  Created by hyhy on 16/3/31.
//
//

#include "MyColorLabel.h"

using namespace cocos2d;

MyColorLabel *MyColorLabel::createWithVector(cocos2d::Vector<cocos2d::Label *> labels)
{
    MyColorLabel *pRet = new MyColorLabel();
    if (pRet && pRet->init(labels)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool MyColorLabel::init(cocos2d::Vector<cocos2d::Label *> labels)
{
    if (!Label::init()) {
        return false;
    }
    
    float totalX = 0;
    float totalY = 0;
    for (int i = 0; i < labels.size(); i++) {
        Label *item = labels.at(i);
        Size size = item->getContentSize();
        item->setPosition(totalX + size.width * 0.5, size.height * 0.5);
        this->addChild(item);
        totalX += size.width;
        if (size.height > totalY) {
            totalY = size.height;
        }
    }
    this->setContentSize(Size(totalX, totalY));
    return true;
}