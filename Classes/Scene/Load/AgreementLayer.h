//
//  AgreementLayer.h
//  ZJH
//
//  Created by hyhy on 16/8/12.
//
//

#ifndef __ZJH__AgreementLayer__
#define __ZJH__AgreementLayer__

#include "ZJH.h"
USING_NS_CC;

class AgreementLayer : public cocos2d::Layer
{
public:
    virtual bool init();
    
    static cocos2d::Scene* createScene();
    CREATE_FUNC(AgreementLayer);
private:
    bool m_bHaveRead;
};

#endif /* defined(__ZJH__AgreementLayer__) */
