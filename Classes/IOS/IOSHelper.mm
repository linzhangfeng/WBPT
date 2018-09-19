//
//  IOSToast.cpp
//  ZJH
//
//  Created by hyhy on 7/15/14.
//
//
#import <AudioToolbox/AudioToolbox.h>

#import <AdSupport/AdSupport.h>
#import <AddressBook/AddressBook.h>
#include "IOSHelper.h"
#include "KeychainItemWrapper.h"
#include "TalkingData.h"

#include "SecKeyWrapper.h"

#include "ImageHelper.h"
#include "LocationGetIOS.h"
#include "ZJH.h"

#include "TeamTalk/VoiceManager.h"
#include "AppController.h"

#include "BaiduMapManager.h"

#include "UpdateTipView.h"
#include <sys/utsname.h> //获得ios手机相关
#include "OpenLocationTipView.h" //是否开启定位的提示弹窗

//获取手机的网络状态和运营商名称
#import <CoreTelephony/CTTelephonyNetworkInfo.h>
#import <CoreTelephony/CTCarrier.h>


#if (ENABLE_IOS_PAY == 1)

#include "pay/appStore/BBIapHelper.h"
#include "pay/WeChat/WXHelper.h"
#include "UICKeyChainStore.h"

#endif



#if (ENABLE_IOS_PAY == 1)
static int isLoading = 0;
@interface WXLoginManager : NSObject<WXApiManagerDelegate>
@property(retain, nonatomic) NSString *code;
@end

@implementation WXLoginManager
- (instancetype) init{
    if (self = [super init]) {
    }
    WXApiManager *apiManager = [WXApiManager sharedManager];
    apiManager.delegate = self;
    return self;
}

- (void)managerDidRecvAuthResponse:(SendAuthResp *)response {
    if (response.code != nil) {
        cocos2d::__String *sender = cocos2d::__String::create([response.code UTF8String]);
        __Dictionary *dic = __Dictionary::create();
        dic->setObject(sender, "wx_login_code");
        if (isLoading == 0) {
            __NotificationCenter::getInstance()->postNotification("wx_login_code",sender);
        } else if (isLoading == 1){
            __NotificationCenter::getInstance()->postNotification("wx_authorize",sender);
        }
        else if (isLoading == 2){
            __NotificationCenter::getInstance()->postNotification("wx_sysInfo",sender);
        }
    }
}
@end
static WXLoginManager *loginManager=nullptr;
#endif

NSString *getUUID()
{
    CFUUIDRef puuid = CFUUIDCreate(nil);
    CFStringRef uuidString = CFUUIDCreateString(nil, puuid);
    NSString *result = (NSString *)CFStringCreateCopy(NULL, uuidString);
    CFRelease(puuid);
    CFRelease(uuidString);
    NSLog(@"hyhy_test: getUUID:%@", result);
    return [result autorelease];
}

const char *IOSHelper::getIDFA()
{
    NSString *idfa = [[[ASIdentifierManager sharedManager] advertisingIdentifier] UUIDString];
    return [idfa UTF8String];
}

bool IOSHelper::openUrl(const char *url)
{
    NSString *urlStr = [NSString stringWithUTF8String:url];
    return [[UIApplication sharedApplication] openURL:[NSURL URLWithString:[urlStr stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]]];
}

void IOSHelper::IosVibrate()
{
    AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
}

void IOSHelper::setUUIDToKeyChain()
{
    KeychainItemWrapper *wrapper = [[KeychainItemWrapper alloc] initWithIdentifier:@"wanbei_identifier" accessGroup:@"CGYKFMUK3S.com.wanbei.wanbei"];
    NSString *uuid = [wrapper objectForKey:(id)kSecAttrAccount];
    if (nil == uuid || uuid.length == 0) {
        uuid = getUUID();
        [wrapper setObject:uuid forKey:(id)kSecAttrAccount];
        NSLog(@"hyhy_test: setUUIDToKeyChain:%@", uuid);
    }
}

const char *IOSHelper::getUUIDFromKeyChain()
{
    KeychainItemWrapper *wrapper = [[KeychainItemWrapper alloc] initWithIdentifier:@"wanbei_identifier" accessGroup:@"CGYKFMUK3S.com.wanbei.wanbei"];
    NSString *uuid = [wrapper objectForKey:(id)kSecAttrAccount];
    if (uuid == nil || uuid.length == 0) {
        setUUIDToKeyChain();
        ///需要再重新取一次wrapper，不然[wrapper objectForKey:(id)kSecAttrAccount]的值为空3WUKS5227P  CGYKFMUK3S
        wrapper = [[KeychainItemWrapper alloc] initWithIdentifier:@"wanbei_identifier" accessGroup:@"CGYKFMUK3S.com.wanbei.wanbei"];
        uuid = [wrapper objectForKey:(id)kSecAttrAccount];
    }
    NSLog(@"hyhy_test: getUUIDFromKeyChain:%@", uuid);
    return [uuid UTF8String];
}

void IOSHelper::resetUUID()
{
    KeychainItemWrapper *wrapper = [[KeychainItemWrapper alloc] initWithIdentifier:@"wanbei_identifier" accessGroup:@"CGYKFMUK3S.com.wanbei.wanbei"];
    if (wrapper) {
        NSLog(@"hyhy_test: reset uuid");
        [wrapper resetKeychainItem];
    }
}

int IOSHelper::getChannelId()
{
    return 10000;
}

const char *IOSHelper::getChannelName()
{
    return "ios";
}

const char *IOSHelper::getAppVersion()
{
    NSDictionary *infoDic = [[NSBundle mainBundle] infoDictionary];
    NSString *appVersion = [infoDic objectForKey:@"CFBundleShortVersionString"];
    return [appVersion UTF8String];
}

float IOSHelper::getIosVersion()
{
    NSArray *versionArr = [[[UIDevice currentDevice] systemVersion] componentsSeparatedByString:@"."];
    float rtn = [[versionArr objectAtIndex:0] intValue];
    rtn += [[versionArr objectAtIndex:1] intValue] * 0.1;
    rtn += 0.00001;    //7.1的会变成7.0999999这样的，所以加上这个值
    return rtn;
}

float IOSHelper::dealLow_8Version()
{

   return [[[UIDevice currentDevice] systemVersion] floatValue];
    
}

void IOSHelper::copyToPasteboard(std::string str)
{
    UIPasteboard *pasteboard = [UIPasteboard generalPasteboard];
    pasteboard.string = [NSString stringWithUTF8String:str.c_str()];
}

std::string IOSHelper::getStringByPasteBoard()
{
    UIPasteboard *pasteboard = [UIPasteboard generalPasteboard];
    NSString *strT1 = pasteboard.string;
    
    return [strT1 UTF8String];
    
}

void IOSHelper::trackEventWithLabel(const char *eventId, const char *eventLabel)
{
    [TalkingData trackEvent:[NSString stringWithUTF8String:eventId] label:[NSString stringWithUTF8String:eventLabel]];

}

void IOSHelper::weiXinLoad(bool isloading)
{
    #if (ENABLE_IOS_PAY == 1)
    isLoading = isloading ? 0 : 1;
    WXHelper::sharedWXHelper()->WXLoading();
    if (!loginManager) {
        loginManager = [[WXLoginManager alloc]init];
    }
    #endif
}

void IOSHelper::weiXin_sysInfo()
{
#if (ENABLE_IOS_PAY == 1)
    isLoading = 2;
    WXHelper::sharedWXHelper()->WXLoading();
    if (!loginManager) {
        loginManager = [[WXLoginManager alloc]init];
    }
#endif
}

//RSA加密
const char * IOSHelper::encryptWithString(const char * content)
{
    NSString *contentOC = [[NSString alloc]initWithCString:content encoding:NSUTF8StringEncoding];
    NSString *s = @"MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCd4QSrcBlpv++k9Ho9O+o3HWT0iQcUaZ215OgsSDNGvGO7npjKZJJ0oU3XvY21WBU4TxRh/XoahaR874eN92bUtHXnoVe+toOgCSDOz8kJ1NsvkiZtBcGyujC4GYzHgCQ1M2ukxXhCrotqFw2tnMoQ6YYu7WSw+EOsuhVNwhulswIDAQAB";
    NSData *publicKey = [[NSData alloc]initWithBase64EncodedString:s options:NSDataBase64DecodingIgnoreUnknownCharacters];
    NSData *usernamm = [contentOC dataUsingEncoding: NSUTF8StringEncoding];
    NSData *newKey= [SecKeyWrapper encrypt:usernamm publicKey:publicKey];
    NSString *result = [newKey base64EncodedStringWithOptions:NSDataBase64Encoding64CharacterLineLength ];
    const char *resultC = [result UTF8String];
    
    return resultC;
}

std::map<std::string,double> IOSHelper::getUserCoordinate() {
    std::map<std::string, double> coordinateMap;
    
    BaiduMapManager *mapManager = [BaiduMapManager shareManager];
    coordinateMap["latitude"] = mapManager.latitude;
    coordinateMap["longitude"] = mapManager.longitude;
    
    [mapManager stopUpdateLocation];
    
    return coordinateMap;
}

std::map<std::string, std::string> IOSHelper::getUserAddress() {
    std::map<std::string, std::string> addressMap;
    BaiduMapManager *mapManager = [BaiduMapManager shareManager];
    
    addressMap["provice"] = mapManager.province;
    addressMap["city"] = mapManager.city;
    addressMap["district"] = mapManager.district;
    addressMap["streetName"] = mapManager.streetName;
    addressMap["streetNumber"] = mapManager.streetNumber;
    
    [mapManager stopUpdateLocation];
    
    return addressMap;
}

float IOSHelper::getDistanceByCoordinate(cocos2d::Point start,cocos2d::Point end) {
    CLLocationCoordinate2D ord1 = CLLocationCoordinate2DMake(start.x, start.y);
    CLLocationCoordinate2D ord2 = CLLocationCoordinate2DMake(end.x, end.y);
    return [[BaiduMapManager shareManager] getDistanceByStart:ord1 end:ord2];
}

std::map<std::string, double> IOSHelper::getLocation()
{
    std::map<std::string, double> locationMap;
    
    LocationGetIOS *location = [LocationGetIOS shareLocationGetIOS];
    locationMap["longitude"] = location.longitude;
    locationMap["latitude"] = location.latitude;
    
    printf("经度1 == %.2f\n纬度2 == %.2f\n", locationMap["longitude"], locationMap["latitude"]);
    
    [location.locationManager stopUpdatingLocation];
    
    return locationMap;
}

std::map<std::string, std::string> IOSHelper::cityMap()
{
    std::map<std::string, std::string> mapT;
    
    LocationGetIOS *location = [LocationGetIOS shareLocationGetIOS];
    
    mapT["provice"] = location.provice;
    mapT["city"] = location.city;
    mapT["qu"] = location.qu;

    [location.locationManager stopUpdatingLocation];
    
    printf("city＝＝ %s\n %s\n %s\n", mapT["provice"].c_str(), mapT["city"].c_str(), mapT["qu"].c_str() );
    return mapT;
    
}


std::string IOSHelper::systemFontName()
{
    UIFont *fo = [UIFont systemFontOfSize:36];
    ;
    const char *resultC = [fo.fontName UTF8String];
    std::string fontName = resultC;
    return fontName;
}

void IOSHelper::openCamera()
{
    ImageHelper::openCamera();
}

void IOSHelper::openPhotoLibrary()
{
    ImageHelper::openPhotoLibrary();
}

void IOSHelper::openSavedPhotosAlbum()
{
    ImageHelper::openSavedPhotosAlbum();
}

void IOSHelper::WXShareText(const char *shareTitle, const char *shareContent)
{
#if (ENABLE_IOS_PAY == 1)
    WXHelper::sharedWXHelper()->WXShareText(shareTitle, shareContent);
#endif
}

void IOSHelper::WeChatShare(const char *shareTitle, const char *shareContent, const char *shareUrl, const char *thumbImage)
{
#if (ENABLE_IOS_PAY == 1)
    WXHelper::sharedWXHelper()->WXShare(shareTitle, shareContent, shareUrl, thumbImage);
#endif
}

void IOSHelper::WeFriendsCircleShare(const char *shareTitle, const char *shareContent, const char *shareUrl, const char *thumbImage)
{
#if (ENABLE_IOS_PAY == 1)
    WXHelper::sharedWXHelper()->WXFriendsCircleShare(shareTitle, shareContent, shareUrl, thumbImage);
#endif
}

void IOSHelper::WeChatShareImage(const char *shareTitle, const char *filePath)
{
#if (ENABLE_IOS_PAY == 1)
    if (Utils::hasImageFile(filePath))
    {
        WXHelper::sharedWXHelper()->WXShareImage(shareTitle, filePath);
    }
    else
    {
        PlatformHelper::showToast("图片不存在");
    }
#endif
}

void IOSHelper::WeFriendsCircleShareImage(const char *shareTitle, const char *filePath)
{
#if (ENABLE_IOS_PAY == 1)
    if (Utils::hasImageFile(filePath))
    {
        WXHelper::sharedWXHelper()->WXFriendsCircleShareImage(shareTitle, filePath);
    }
    else
    {
        PlatformHelper::showToast("图片不存在");
    }
#endif
}

void IOSHelper::IOSPay(std::string invoice, float tradeValue, int payType)
{
    if (invoice.empty()) {
        return;
    }
#if (ENABLE_IOS_PAY == 1)
    Json::Value json;
    json["invoice"] = invoice;
    
    std::string reqAction = "";
    std::string reqTag;
    if(payType == 3)
    {
        reqAction = "pay?channelId=a";
        reqTag = "appstore_pay";
    }
    
    if (reqAction.empty()) {
        return;
    }
    
    CCHttpAgent::getInstance()->sendPayHttpPost([](std::string tag)
                                                {
                                                    CCHttpPacket *packet = CCHttpAgent::getInstance()->packets[tag];
                                                    if (packet)
                                                    {
                                                        if (packet->status == 3)
                                                        {
                                                            packet->status = 0;
                                                            
                                                            if (!packet->recvVal["data"].isNull())
                                                            {
                                                                if (tag == "appstore_pay")
                                                                {
                                                                    
                                                                    BBIapHelper::sharedIapHelper()->purchase(1, ZJHModel::getInstance()->appleId, ZJHModel::getInstance()->iosPrice);
                                                                }
                                                            }
                                                            
                                                        }else
                                                        {
                                                            __NotificationCenter::getInstance()->postNotification("appstore_deleteLoad");
                                                        }
                                                        
                                                        //删除packet
                                                        CCHttpAgent::getInstance()->packets.erase(tag);
                                                        delete packet;
                                                    }
                                                },
                                                reqAction, json.toStyledString(), reqTag);
#endif
}

void IOSHelper::startRecord()
{
    VoiceManager::getInstance()->startRecord();
}

void IOSHelper::stopRecord()
{
    VoiceManager::getInstance()->stopRecord();
}

bool IOSHelper::isPlaying()
{
    return VoiceManager::getInstance()->isPlaying();
}

void IOSHelper::playRecord(std::string filename)
{
    VoiceManager::getInstance()->playRecord(filename);
}

 void IOSHelper::cConViewH()
{
    [AppController changeRootViewControllerH];
}
 void IOSHelper::cConViewV()
{
    [AppController changeRootViewControllerV];
}

std::string IOSHelper::loadCode()
{
    return [AppController loadCode];
}

std::string IOSHelper::loadCodeNew()
{
    return [AppController loadCodeNew];
}

std::string IOSHelper::loadInviteCode()
{
    return [AppController loadInviteCode];
}
void IOSHelper::IOSPayClearId(const char * tradeID)
{
#if (ENABLE_IOS_PAY == 1)

    BBIapHelper::sharedIapHelper()->consume(tradeID);
    
#endif
    
}
void IOSHelper::iosUpdateTip(int force, std::string desc, std::string versionName, std::string URL)
{
    //每次视图即将出现,都去Appdelegate中取    
    UIWindow *window = [[UIApplication sharedApplication] keyWindow];
    UpdateTipView *tip = [UpdateTipView shareUpdateTipView];
    [window addSubview:tip.view];

    NSString *str1 = [[NSString alloc] initWithUTF8String:desc.c_str()];
    NSString *str2 = [[NSString alloc] initWithUTF8String:versionName.c_str()];
    NSString *str3 = [[NSString alloc] initWithUTF8String:URL.c_str()];
    
    [tip showView:force Desc:str1 VerName:str2 URL:str3];
}

void IOSHelper::removeIosUpdateTip()
{
    UpdateTipView *tip = [UpdateTipView shareUpdateTipView];
    [tip  removeAlertViewController];
    [tip.view removeFromSuperview];
    
}

//定位弹窗提示
void IOSHelper::iosOpenLocationTip()
{
    if([CLLocationManager locationServicesEnabled] == NO || ([CLLocationManager locationServicesEnabled] && [CLLocationManager authorizationStatus] == kCLAuthorizationStatusDenied))
    {
        //每次视图即将出现,都去Appdelegate中取
        UIWindow *window = [[UIApplication sharedApplication] keyWindow];
        OpenLocationTipView  *tip = [OpenLocationTipView shareOpenTipView];
        [window addSubview:tip.view];
        
        [tip showView];
    }
}

void IOSHelper::removeIosOpenLocationTip()
{
    OpenLocationTipView *tip = [OpenLocationTipView shareOpenTipView];
    [tip  removeAlertViewController];
    [tip.view removeFromSuperview];
}

//1.手机系统：iPhone OS
std::string IOSHelper::iphoneVersion()
{
    NSString* phoneVersion = [[UIDevice currentDevice] systemVersion];
    string tmp1 = (std::string)[phoneVersion UTF8String];
    return tmp1;
    
}
//2.手机类型：iPhone 6
std::string IOSHelper::iphoneTypeName()
{
    return iphoneType();
}

string IOSHelper::iphoneType()
{
    struct utsname systemInfo;
    
    uname(&systemInfo);
    
    NSString *platform = [NSString stringWithCString:systemInfo.machine encoding:NSASCIIStringEncoding];
    
    if ([platform isEqualToString:@"iPhone1,1"]) return "iPhone 2G";
    
    if ([platform isEqualToString:@"iPhone1,2"]) return "iPhone 3G";
    
    if ([platform isEqualToString:@"iPhone2,1"]) return "iPhone 3GS";
    
    if ([platform isEqualToString:@"iPhone3,1"]) return "iPhone 4";
    
    if ([platform isEqualToString:@"iPhone3,2"]) return "iPhone 4";
    
    if ([platform isEqualToString:@"iPhone3,3"]) return "iPhone 4";
    
    if ([platform isEqualToString:@"iPhone4,1"]) return "iPhone 4S";
    
    if ([platform isEqualToString:@"iPhone5,1"]) return "iPhone 5";
    
    if ([platform isEqualToString:@"iPhone5,2"]) return "iPhone 5";
    
    if ([platform isEqualToString:@"iPhone5,3"]) return "iPhone 5c";
    
    if ([platform isEqualToString:@"iPhone5,4"]) return "iPhone 5c";
    
    if ([platform isEqualToString:@"iPhone6,1"]) return "iPhone 5s";
    
    if ([platform isEqualToString:@"iPhone6,2"]) return "iPhone 5s";
    
    if ([platform isEqualToString:@"iPhone7,1"]) return "iPhone 6 Plus";
    
    if ([platform isEqualToString:@"iPhone7,2"]) return "iPhone 6";
    
    if ([platform isEqualToString:@"iPhone8,1"]) return "iPhone 6s";
    
    if ([platform isEqualToString:@"iPhone8,2"]) return "iPhone 6s Plus";
    
    if ([platform isEqualToString:@"iPhone8,4"]) return "iPhone SE";
    
    if ([platform isEqualToString:@"iPhone9,1"]) return "iPhone 7";
    
    if ([platform isEqualToString:@"iPhone9,2"]) return "iPhone 7 Plus";
    
    if ([platform isEqualToString:@"iPod1,1"])  return  "iPod Touch 1G";
    
    if ([platform isEqualToString:@"iPod2,1"])  return  "iPod Touch 2G";
    
    if ([platform isEqualToString:@"iPod3,1"])  return  "iPod Touch 3G";
    
    if ([platform isEqualToString:@"iPod4,1"])  return  "iPod Touch 4G";
    
    if ([platform isEqualToString:@"iPod5,1"])  return  "iPod Touch 5G";
    
    if ([platform isEqualToString:@"iPad1,1"])  return  "iPad 1G";
    
    if ([platform isEqualToString:@"iPad2,1"])  return  "iPad 2";
    
    if ([platform isEqualToString:@"iPad2,2"])  return  "iPad 2";
    
    if ([platform isEqualToString:@"iPad2,3"])  return "iPad 2";
    
    if ([platform isEqualToString:@"iPad2,4"])  return "iPad 2";
    
    if ([platform isEqualToString:@"iPad2,5"])  return "iPad Mini 1G";
    
    if ([platform isEqualToString:@"iPad2,6"])  return "iPad Mini 1G";
    
    if ([platform isEqualToString:@"iPad2,7"])  return "iPad Mini 1G";
    
    if ([platform isEqualToString:@"iPad3,1"])  return "iPad 3";
    
    if ([platform isEqualToString:@"iPad3,2"])  return "iPad 3";
    
    if ([platform isEqualToString:@"iPad3,3"])  return "iPad 3";
    
    if ([platform isEqualToString:@"iPad3,4"])  return "iPad 4";
    
    if ([platform isEqualToString:@"iPad3,5"])  return "iPad 4";
    
    if ([platform isEqualToString:@"iPad3,6"])  return "iPad 4";
    
    if ([platform isEqualToString:@"iPad4,1"])  return "iPad Air";
    
    if ([platform isEqualToString:@"iPad4,2"])  return "iPad Air";
    
    if ([platform isEqualToString:@"iPad4,3"])  return "iPad Air";
    
    if ([platform isEqualToString:@"iPad4,4"])  return "iPad Mini 2G";
    
    if ([platform isEqualToString:@"iPad4,5"])  return "iPad Mini 2G";
    
    if ([platform isEqualToString:@"iPad4,6"])  return "iPad Mini 2G";
    
    if ([platform isEqualToString:@"i386"])      return "iPhone Simulator";
    
    if ([platform isEqualToString:@"x86_64"])    return "iPhone Simulator";
    
    return "暂无匹配类型";
    
}

std::map<std::string, std::string> IOSHelper::phoneNetInfo()
{
    std::map<std::string, std::string> mapT;
    
    NSArray *subviews = [[[[UIApplication sharedApplication] valueForKey:@"statusBar"] valueForKey:@"foregroundView"]subviews];
    NSNumber *dataNetworkItemView = nil;
    
    for (id subview in subviews) {
        if([subview isKindOfClass:[NSClassFromString(@"UIStatusBarDataNetworkItemView") class]]) {
            dataNetworkItemView = subview;
            break;
        }
    }
    
    string netString = "";
    
    switch ([[dataNetworkItemView valueForKey:@"dataNetworkType"]integerValue])
    {
        case 0:
            NSLog(@"No wifi or cellular");
            netString = "无服务";
            break;
            
        case 1:
            NSLog(@"2G");
            netString = "2G";
            break;
            
        case 2:
            NSLog(@"3G");
            netString = "3G";
            break;
            
        case 3:
            NSLog(@"4G");
            netString = "4G";
            break;
            
        case 4:
            NSLog(@"LTE");
            netString = "LTE";
            break;
            
        case 5:
            NSLog(@"Wifi");
            netString = "Wifi";
            break;
    }
    
    mapT["networkType"] = netString;
    
    
    CTTelephonyNetworkInfo *telephonyInfo = [[CTTelephonyNetworkInfo alloc] init];
    CTCarrier *carrier = [telephonyInfo subscriberCellularProvider];
    NSString *currentCountry=[carrier carrierName];
   
    if (currentCountry != NULL)
    {
        mapT["operatorType"] = [currentCountry UTF8String];
    }else
    {
        mapT["operatorType"] = "未监测到运营商";
    }
    
    
    printf("AAA = %s\nBBB = %s\n", mapT["networkType"].c_str(), mapT["operatorType"].c_str());
    
    return mapT;
    
}

//注册微信登录的ID
void IOSHelper::registWeiXinLoginId()
{
#if (ENABLE_IOS_PAY == 1)
    //应用宝的ID
    [WXApi registerApp:@"wxf9f89e76d7b349e9"];
#endif
}

bool IOSHelper::checkIsInstallWeiXin()
{
#if (ENABLE_IOS_PAY == 1)
    return  WXHelper::sharedWXHelper()->checkIsInstallWeiXin();
#else
    return true;
#endif
}

//存储appstore购买的数据
void IOSHelper::saveStoreChainData(std::string IosTransactionId, std::string appStoreInvoice, std::string IosReceipt, std::string iosTotalFee, std::string userID)
{
    #if (ENABLE_IOS_PAY == 1)
    
    //存储密码
    UICKeyChainStore *keychainStore = [UICKeyChainStore keyChainStore];
    keychainStore[@"IosTransactionId"] = [NSString stringWithFormat:@"%s", IosTransactionId.c_str()];
    keychainStore[@"appStoreInvoice"] = [NSString stringWithFormat:@"%s", appStoreInvoice.c_str()];
    keychainStore[@"IosReceipt"] = [NSString stringWithFormat:@"%s", IosReceipt.c_str()];
    keychainStore[@"iosTotalFee"] = [NSString stringWithFormat:@"%s", iosTotalFee.c_str()];
    keychainStore[@"userID"] = [NSString stringWithFormat:@"%s", userID.c_str()];
    
    #endif
    
}

std::map<std::string, std::string> IOSHelper::getStoreChainData()
{
    std::map<std::string, std::string> tempMap;
    
#if (ENABLE_IOS_PAY == 1)
    
    UICKeyChainStore *keychainStore = [UICKeyChainStore keyChainStore];
    
    NSString *IosTransactionId = [keychainStore stringForKey:@"IosTransactionId"];
    NSString *appStoreInvoice = [keychainStore stringForKey:@"appStoreInvoice"];
    NSString *IosReceipt = [keychainStore stringForKey:@"IosReceipt"];
    NSString *iosTotalFee = [keychainStore stringForKey:@"iosTotalFee"];
    NSString *userID = [keychainStore stringForKey:@"userID"];
    
    if (IosTransactionId != nil && appStoreInvoice != nil && IosReceipt != nil && iosTotalFee != nil && userID != nil)
    {
        tempMap["IosTransactionId"] = [IosTransactionId UTF8String];
        tempMap["appStoreInvoice"] = [appStoreInvoice UTF8String];
        tempMap["IosReceipt"] = [IosReceipt UTF8String];
        tempMap["iosTotalFee"] = [iosTotalFee UTF8String];
        tempMap["userID"] = [userID UTF8String];
    }

#endif

        return tempMap;
    
}
//清除appstore购买的数据
void IOSHelper::clearStoreChainData()
{
#if (ENABLE_IOS_PAY == 1)

    UICKeyChainStore *keychainStore = [UICKeyChainStore keyChainStore];
    keychainStore[@"IosTransactionId"] = [NSString string];
    keychainStore[@"appStoreInvoice"] = [NSString string];
    keychainStore[@"IosReceipt"] = [NSString string];
    keychainStore[@"iosTotalFee"] = [NSString string];
    keychainStore[@"userID"] = [NSString string];

#endif
}

int IOSHelper::getDianLiangValue()
{
    return [AppController getDianLiangValue];
}

std::map<std::string, std::string> IOSHelper::getSignalValue()
{
    return  [AppController getSignalValue];
}

void IOSHelper::callPhone(std::string phone)
{
    [AppController callPhone: phone];
}
