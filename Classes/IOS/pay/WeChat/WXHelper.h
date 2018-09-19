//
//  WXHelper.h
//  ZJH
//
//  Created by hyhy on 1/21/16.
//
//

#ifndef WXHelper_h
#define WXHelper_h


#include "WXApiManager.h"

class WXHelper
{
public:
    ~WXHelper();
    bool init();
    
    static WXHelper *sharedWXHelper();
    static void purgeSharedWXHelper();
    
    void WXShareText(const char *shareTitle, const char *shareContent);
    void WXShare(const char *shareTitle, const char *shareContent, const char *shareUrl, const char *thumbImage);
    void WXFriendsCircleShare(const char *shareTitle, const char *shareContent, const char *shareUrl, const char *thumbImage);
    void WXShareImage(const char *shareTitle, const char *filePath);
    void WXFriendsCircleShareImage(const char *shareTitle, const char *filePath);
    
    void WXLoading();
    
    bool checkIsInstallWeiXin();
    
    void weiXinShareTip();
    
    void requestPay(std::string orderInfo);
    
    void purchase(const int tradeType, const int goodsId, const int tradeValue, std::string goodsName);
    
private:
    WXHelper();
    
    static WXHelper *sm_WXHelper;
    id m_payInstance;
};


#endif /* WXHelper_h */
