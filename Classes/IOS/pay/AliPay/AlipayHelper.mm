#include "AlipayHelper.h"
#include "ZJH.h"

#include <AlipaySDK/AlipaySDK.h>

#pragma mark Order
@interface Order : NSObject

@property(nonatomic, copy) NSString * partner;
@property(nonatomic, copy) NSString * seller;
@property(nonatomic, copy) NSString * tradeNO;
@property(nonatomic, copy) NSString * productName;
@property(nonatomic, copy) NSString * productDescription;
@property(nonatomic, copy) NSString * amount;
@property(nonatomic, copy) NSString * notifyURL;

@property(nonatomic, copy) NSString * service;
@property(nonatomic, copy) NSString * paymentType;
@property(nonatomic, copy) NSString * inputCharset;
@property(nonatomic, copy) NSString * itBPay;
@property(nonatomic, copy) NSString * showUrl;


@property(nonatomic, copy) NSString * rsaDate;//可选
@property(nonatomic, copy) NSString * appID;//可选

@property(nonatomic, readonly) NSMutableDictionary * extraParams;

@end


@implementation Order

- (NSString *)description {
	NSMutableString * discription = [NSMutableString string];
    if (self.partner) {
        [discription appendFormat:@"partner=\"%@\"", self.partner];
    }
	
    if (self.seller) {
        [discription appendFormat:@"&seller_id=\"%@\"", self.seller];
    }
	if (self.tradeNO) {
        [discription appendFormat:@"&out_trade_no=\"%@\"", self.tradeNO];
    }
	if (self.productName) {
        [discription appendFormat:@"&subject=\"%@\"", self.productName];
    }
	
	if (self.productDescription) {
        [discription appendFormat:@"&body=\"%@\"", self.productDescription];
    }
	if (self.amount) {
        [discription appendFormat:@"&total_fee=\"%@\"", self.amount];
    }
    if (self.notifyURL) {
        [discription appendFormat:@"&notify_url=\"%@\"", self.notifyURL];
    }
	
    if (self.service) {
        [discription appendFormat:@"&service=\"%@\"",self.service];//mobile.securitypay.pay
    }
    if (self.paymentType) {
        [discription appendFormat:@"&payment_type=\"%@\"",self.paymentType];//1
    }
    
    if (self.inputCharset) {
        [discription appendFormat:@"&_input_charset=\"%@\"",self.inputCharset];//utf-8
    }
    if (self.itBPay) {
        [discription appendFormat:@"&it_b_pay=\"%@\"",self.itBPay];//30m
    }
    if (self.showUrl) {
        [discription appendFormat:@"&show_url=\"%@\"",self.showUrl];//m.alipay.com
    }
    if (self.rsaDate) {
        [discription appendFormat:@"&sign_date=\"%@\"",self.rsaDate];
    }
    if (self.appID) {
        [discription appendFormat:@"&app_id=\"%@\"",self.appID];
    }
	for (NSString * key in [self.extraParams allKeys]) {
		[discription appendFormat:@"&%@=\"%@\"", key, [self.extraParams objectForKey:key]];
	}
	return discription;
}

@end



#pragma mark AlipayInstance
@interface AlipayInstance : NSObject
{
    //nothing
}
@end

@implementation AlipayInstance

- (void)purchaseWithUID:(NSInteger) uid
              tradeType:(NSInteger) tradeType
                goodsId:(NSInteger) goodsId
             tradeValue:(NSInteger) tradeValue
              tradeAddr:(NSInteger) tradeAddr
              goodsName:(NSString *) goodsName
{
    
}

@end



#pragma mark - AlipayHelper

AlipayHelper *AlipayHelper::sm_AlipayHelper = NULL;

AlipayHelper::AlipayHelper()
    :m_payInstance(NULL)
{
    
}

AlipayHelper::~AlipayHelper()
{
    [m_payInstance release];
}

bool AlipayHelper::init()
{
    m_payInstance = [[AlipayInstance alloc] init];
    if (m_payInstance == NULL) {
        return false;
    }
    return true;
}


AlipayHelper *AlipayHelper::sharedAlipayHelper()
{
    if (AlipayHelper::sm_AlipayHelper == NULL) {
        AlipayHelper::sm_AlipayHelper = new AlipayHelper();
        sm_AlipayHelper->init();
    }
    return AlipayHelper::sm_AlipayHelper;
}

void AlipayHelper::purgeSharedAlipayHelper()
{
    CC_SAFE_DELETE(AlipayHelper::sm_AlipayHelper);
}

void AlipayHelper::purchase(const int tradeType, const int goodsId, const int tradeValue, std::string goodsName)
{
    //
//    NSInteger uid = ZJHModel::getInstance()->uid;
//    NSInteger tradeAddr = ZJHModel::getInstance()->tradeAddr;
//    [m_payInstance purchaseWithUID:uid
//                         tradeType:tradeType
//                           goodsId:goodsId
//                        tradeValue:tradeValue
//                         tradeAddr:tradeAddr
//                         goodsName:[NSString stringWithUTF8String:goodsName.c_str()]];
}

void AlipayHelper::requestPay(std::string orderInfo)
{
    Json::Value json = Utils::ParseJsonStr(orderInfo);
    std::string url = json["url"].asString();
    NSString *orderString = [NSString stringWithUTF8String:url.c_str()];
    NSString *scheme = @"com.ihuizhi.club.apple.alipay";
    
    [[AlipaySDK defaultService] payOrder:orderString fromScheme:scheme callback:nil];
}


