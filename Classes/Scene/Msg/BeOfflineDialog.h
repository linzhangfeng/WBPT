//
//  BeOfflineDialog.h
//  ZJH
//
//  Created by hyhy on 16/7/21.
//
//

#ifndef __ZJH__BeOfflineDialog__
#define __ZJH__BeOfflineDialog__

#include "cocos2d.h"
#include "ZJH.h"

class BeOfflineDialog : public cocos2d::Layer
{
public:
    CREATE_FUNC(BeOfflineDialog);
    virtual bool init();
    void buttonClicked(cocos2d::Ref *sender);
};

#endif /* defined(__ZJH__BeOfflineDialog__) */
