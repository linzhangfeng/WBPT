#include "BBIapHelper.h"

#import <StoreKit/StoreKit.h>

#include "ZJH.h"

@interface IapHelper: NSObject <SKProductsRequestDelegate, SKPaymentTransactionObserver> {
    IapDelegate *delegate;
}

- (void)purchase:(NSString *)productId;
- (void)consume:(NSString *)transactionId;
- (bool)checkUnfinishedTransaction;

@end

@implementation IapHelper

- (id)init
{
    if (self = [super init]) {
        [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
        
        ///@todo test
        for (SKPaymentTransaction *transaction in [[SKPaymentQueue defaultQueue] transactions]) {
            CCLOG("hyhy_test_transaction:%@", transaction.transactionIdentifier);
        }
        
    }
    return self;
}

- (void)dealloc
{
    [[SKPaymentQueue defaultQueue] removeTransactionObserver:self];
    [super dealloc];
}

- (void)purchaseError:(const char *)error
{
    PlatformHelper::showToast(error);
    ZJHModel::getInstance()->iosPurchaseResult(0, "", "");
}

- (void)purchase:(NSString *)productId
{
    SKProductsRequest *request = [[[SKProductsRequest alloc] initWithProductIdentifiers:[NSSet setWithArray:@[productId]]] autorelease];
    request.delegate = self;
    [request start];
}

- (void)consume:(NSString *)transactionId
{
    for (SKPaymentTransaction *transaction in [[SKPaymentQueue defaultQueue] transactions]) {
        if ([transaction.transactionIdentifier isEqualToString:transactionId]) {
            [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
            break;
        }
    }
}

- (bool)checkUnfinishedTransaction
{
    for (SKPaymentTransaction *transaction in [[SKPaymentQueue defaultQueue] transactions]) {
        if (transaction.transactionState == SKPaymentTransactionStatePurchased) {
            [self completeTransaction:transaction];
            return true;
        }
    }
    return false;
}

#pragma mark - SKProductsRequestDelegate protocol

- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response
{
    if ([[response invalidProductIdentifiers] count]) {
        [self purchaseError:"无法获取商品列表"];
    } else {
        [[SKPaymentQueue defaultQueue] addPayment:[SKPayment paymentWithProduct:response.products[0]]];
    }
}

- (void)request:(SKRequest *)request didFailWithError:(NSError *)error
{
    [self purchaseError:[[error localizedDescription] UTF8String]];
}

#pragma mark - SKPaymentTransactionObserver protocol

- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
    for (SKPaymentTransaction *transaction in transactions) {
        switch (transaction.transactionState) {
            case SKPaymentTransactionStateFailed:
                NSLog(@"hyhy_test:state failed %@", transaction.error);
                [self failedTransaction:transaction];
                break;
            case SKPaymentTransactionStatePurchased:
                [self completeTransaction:transaction];
                break;
            default:
                break;
        }
    }
}

- (void)failedTransaction:(SKPaymentTransaction *)transaction
{
    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
    if (transaction.error.code != SKErrorPaymentCancelled) {
        [self purchaseError:"购买失败，请重试"];
    } else {
        [self purchaseError:"取消购买"];
    }
}

- (void)completeTransaction:(SKPaymentTransaction *)transaction
{
    const char *transactionId = [transaction.transactionIdentifier UTF8String];
    //const char *receipt = [[[[NSString alloc] initWithData:transaction.transactionReceipt encoding:NSUTF8StringEncoding] autorelease] UTF8String];
    const char *encodedReceipt = [[transaction.transactionReceipt base64Encoding] UTF8String];
    
    //ZJHModel::getInstance()->iosPurchaseResult(1, transactionId, receipt);
    ZJHModel::getInstance()->iosPurchaseResult(1, transactionId, encodedReceipt);
    
    printf("the transactionId 完成的时候 id %s\n",transactionId);
}

@end

#pragma mark - BBIapHelper

//购买钻石
const char *aaaa[] = {
    "com.ihuizhi.zjh.yuan.6",
    "com.ihuizhi.zjh.yuan.12",
    "com.ihuizhi.zjh.yuan.30",
    "com.ihuizhi.zjh.yuan.68",
    "com.ihuizhi.zjh.yuan.108",
    "com.ihuizhi.zjh.yuan.188",
    "com.ihuizhi.zjh.yuan.328"
};

//购买金币
const char *bbbb[] = {
    "com.ihuizhi.zjh.gold.6",
    "com.ihuizhi.zjh.gold.12",
    "com.ihuizhi.zjh.gold.30",
    "com.ihuizhi.zjh.gold.68",
    "com.ihuizhi.zjh.gold.108",
    "com.ihuizhi.zjh.gold.188",
    "com.ihuizhi.zjh.gold.328"
};

//购买道具
const char *cccc[] = {
    "com.ihuizhi.zjh.daoju.jinbi.6",
    "com.ihuizhi.zjh.daoju.huanpai.6",
    "com.ihuizhi.zjh.daoju.fanbei.6"
};

int getIndexByValue(int value) {
    int index = -1;
    switch (value) {
        case 5:
            index = 0;
            break;
        case 10:
            index = 1;
            break;
        case 30:
            index = 2;
            break;
        case 50:
            index = 3;
            break;
        case 100:
            index = 4;
            break;
        case 500:
            index = 5;
            break;
            
        default:
            index = -1;
            break;
    }
    return index;
}

std::string getIapProductId(const int tradeType, std::string goodsId, float tradeValue)
{
//    std::string productId = "";
//    CCLog("the price is %d" ,tradeValue);
//    if (goodsId == -1) {
//        bool isBuyGold = (tradeType == Pay_Money);
//        
//        switch (tradeValue) {
//            case 6:
//                productId = isBuyGold ? ZJHModel::getInstance()->ios_charging["gold_6"].asCString() : ZJHModel::getInstance()->ios_charging["yuan_6"].asCString();
//                productId = ZJHModel::getInstance()->ios_charging["product_6"].asCString();
//                break;
//            case 12:
////                productId = isBuyGold ? "com.ihuizhi.zjh.gold.12" : "com.ihuizhi.zjh.yuan.12";
//                productId = isBuyGold ? ZJHModel::getInstance()->ios_charging["gold_12"].asCString():ZJHModel::getInstance()->ios_charging["yuan_12"].asCString();
//                break;
//            case 30:
//                productId = isBuyGold ? ZJHModel::getInstance()->ios_charging["gold_30"].asCString():ZJHModel::getInstance()->ios_charging["yuan_30"].asCString();
//
//                break;
//            case 68:
////                productId = "com.ihuizhi.zjh.yuan.68";
//                productId = ZJHModel::getInstance()->ios_charging["yuan_68"].asCString();
//                
//                break;
//            case 108:
////                productId = "com.ihuizhi.zjh.yuan.108";
//                productId =ZJHModel::getInstance()->ios_charging["yuan_108"].asCString();
//                break;
//            case 188:
////                productId = "com.ihuizhi.zjh.yuan.188";
//                productId = ZJHModel::getInstance()->ios_charging["yuan_188"].asCString();
//                break;
//            case 328:
////                productId = "com.ihuizhi.zjh.yuan.328";
//                productId = ZJHModel::getInstance()->ios_charging["yuan_328"].asCString();
//                break;
//                
//            default:
//                break;
//        }
//    } else {

    std::string productId = "";
    //com.wanbei.wanbei.wb.1
    int price = tradeValue;
    switch (price)
    {
        case 1:
            productId = "com.wbyl.dp.WB10";
            break;
        case 6:
            productId = "com.wbyl.dp.WB60";
            break;
        case 25:
            productId = "com.wbyl.dp.WB25";
            break;
        case 50:
            productId = "com.wbyl.dp.WB50";
            break;
        case 98:
            productId = "com.wbyl.dp.WB98";
            break;
        case 198:
            productId = "com.wbyl.dp.WB198";
            break;
        case 488:
            productId = "com.wbyl.dp.WB488";
            break;
    }

    return productId;
}

BBIapHelper *BBIapHelper::sm_iapHelper = NULL;

BBIapHelper::BBIapHelper()
    : m_iapHelper(NULL)
    , m_pDelegate(NULL)
{
}

BBIapHelper::~BBIapHelper()
{
    [m_iapHelper release];
}

BBIapHelper *BBIapHelper::sharedIapHelper()
{
    if (!BBIapHelper::sm_iapHelper) {
        BBIapHelper::sm_iapHelper = new BBIapHelper();
        sm_iapHelper->init();
    }
    return BBIapHelper::sm_iapHelper;
}

void BBIapHelper::purgeSharedIapHelper()
{
    CC_SAFE_DELETE(BBIapHelper::sm_iapHelper);
}

bool BBIapHelper::init()
{
    m_iapHelper = [[IapHelper alloc] init];
    return m_iapHelper != NULL;
}

void BBIapHelper::purchase(const int tradeType, std::string goodsId, float tradeValue)
{
    if (checkUnfinishedTransaction()) {
        PlatformHelper::showToast("正在处理未完成的付款请求");
    } else {
        std::string productId = getIapProductId(tradeType, goodsId, tradeValue);
        if (productId.empty()) {
            PlatformHelper::showToast("该商品不存在");
        } else {
            [m_iapHelper purchase:[NSString stringWithUTF8String:productId.c_str()]];
        }
    }
}

void BBIapHelper::consume(const char *transactionId)
{
    [m_iapHelper consume:[NSString stringWithUTF8String:transactionId]];
}

bool BBIapHelper::checkUnfinishedTransaction()
{
    return [m_iapHelper checkUnfinishedTransaction];
}
