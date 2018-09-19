#ifndef __BBIAPHELPER_H__
#define __BBIAPHELPER_H__

#include <objc/objc.h>

class IapDelegate
{
    virtual void purchaseResult(int result) {};
};

class BBIapHelper
{
public:
	~BBIapHelper();
	bool init();

	static BBIapHelper *sharedIapHelper();
	static void purgeSharedIapHelper();

    void purchase(const int tradeType, std::string goodsId, float tradeValue);
	void consume(const char *transactionId);
    
    bool checkUnfinishedTransaction();
    
    void setDelegate(IapDelegate *delegate)
    {
        m_pDelegate = delegate;
    }

private:
	BBIapHelper();

	static BBIapHelper *sm_iapHelper;
	id m_iapHelper;
    IapDelegate *m_pDelegate;
};

#endif /* defined(__BBIAPHELPER_H__) */
