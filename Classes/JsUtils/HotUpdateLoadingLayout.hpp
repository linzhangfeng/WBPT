//
//  HotUpdateLoadingLayout.hpp
//  ZJH
//
//  Created by mac on 2017/5/20.
//
//

#ifndef HotUpdateLoadingLayout_hpp
#define HotUpdateLoadingLayout_hpp

#include "extensions/cocos-ext.h"
#include "ZJH.h"

class HotUpdateLoadingLayout : public Layout
{
public:
    
    CREATE_FUNC(HotUpdateLoadingLayout);
    virtual bool init() override;
    virtual void onExit() override;
    void notificationCallback(cocos2d::Ref *msg);
    cocos2d::Layer* _loadLayer;
    
    int watchGameId;
    string tips;
    Text* _tip;
    ImageView* _loadingBg;
    LoadingBar* _loading;
    cocos2d::Label* _progress;
    
};
#endif /* HotUpdateLoadingLayout_hpp */
