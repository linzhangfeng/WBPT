//
//  AlipayHelper.h
//  ZJH
//
//  Created by hyhy on 12/16/14.
//
//

#ifndef ZJH_AlipayHelper_h
#define ZJH_AlipayHelper_h

#include <objc/objc.h>
#include <string>

class AlipayHelper
{
public:
    ~AlipayHelper();
    bool init();
    
    static AlipayHelper *sharedAlipayHelper();
    static void purgeSharedAlipayHelper();
    
    void purchase(const int tradeType, const int goodsId, const int tradeValue, std::string goodsName);
    void requestPay(std::string orderInfo);
private:
    AlipayHelper();
    
    static AlipayHelper *sm_AlipayHelper;
    id m_payInstance;
    
};


#endif
