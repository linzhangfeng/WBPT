//
//  HotUpdateWithNormalView.hpp
//  ZJH
//
//  Created by mac on 2017/5/2.
//
//

#ifndef HotUpdateWithNormalView_hpp
#define HotUpdateWithNormalView_hpp

#include "extensions/cocos-ext.h"
#include "ZJH.h"

class HotUpdateWithNormalView : public Layout
{
public:
    
    static Layout *createScene(const Json::Value& dat);
    
    CREATE_FUNC(HotUpdateWithNormalView);
    
    void startDownloadCallback();
    void onLoadEnd(bool is);
    static void hotUpdateModule(int gameId,Node* parent,bool useCache);
    static void runTestModule();//测试代码
    ~HotUpdateWithNormalView();
    
    std::string m_run;
    Json::Value m_data;
    int currentGameId;
    
    virtual bool init() override;
    virtual void onExit() override;
    void postNotify(int event, float process,int currenIndex,int total);
private:
    int m_index;
    bool frist;
    Layout* _loadLayer;
    string tips;
    
    cocos2d::extension::AssetsManagerEx* _am;
    cocos2d::Label* _tip;
    ImageView* _loadingBg;
    LoadingBar* _loading;
    cocos2d::Label* _progress;
    cocos2d::extension::EventListenerAssetsManagerEx* _amListener;
    
    
};
#endif /* HotUpdateWithNormalView_hpp */
