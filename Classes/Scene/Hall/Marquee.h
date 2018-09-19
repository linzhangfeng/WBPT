//
//  Marquee.h
//  ZJH
//
//  Created by hyhy on 16/7/9.
//
//

#ifndef __ZJH__Marquee__
#define __ZJH__Marquee__

#include "cocos2d.h"
#include "json.h"

class Marquee : public cocos2d::Layer
{
public:
    static Marquee *createWithSize(cocos2d::Size size);
    bool initWithSize(cocos2d::Size size);
    virtual ~Marquee();
    
    void start(float dt);
    int refreshData();//更新消息数据
    void checkNext(float dt);
    void initCurOne(int index);
    void updateMove(float dt);
private:
    cocos2d::ClippingNode *m_pClip;
    cocos2d::Size m_viewSize;
    Node *m_pCurLabel;
    float m_fLabelWidth;
    int m_nCurCount;
    int m_nTotalCount;
    
    Json::Value jsonData;
};

#endif /* defined(__ZJH__Marquee__) */
