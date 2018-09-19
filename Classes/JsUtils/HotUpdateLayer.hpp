//
//  HotUpdateLayer.hpp
//  ZJH
//
//  Created by mac on 16/10/12.
//
//

#ifndef HotUpdateLayer_hpp
#define HotUpdateLayer_hpp

#include "extensions/cocos-ext.h"
#include "ZJH.h"

class HotUpdateLayer : public Layer
{
public:
    
    static cocos2d::Scene *createScene(const Json::Value& dat);

    CREATE_FUNC(HotUpdateLayer);
    
    void startDownloadCallback();
    void onLoadEnd(bool is);
    
    std::string m_run;
    Json::Value m_data;

    
    virtual bool init() override;
    virtual void onExit() override;
private:
        int m_index;
    bool frist;
    cocos2d::Layer* _loadLayer;
    
    cocos2d::extension::AssetsManagerEx* _am;
    cocos2d::Label* _tip;
    ImageView* _loadingBg;
    LoadingBar* _loading;
    cocos2d::Label* _progress;
    cocos2d::extension::EventListenerAssetsManagerEx* _amListener;
    
        
};

#endif /* HotUpdateLayer_hpp */
