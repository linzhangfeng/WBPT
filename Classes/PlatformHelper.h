//
//  PlatformHelper.h
//  ZJH
//
//  Created by Apple on 13-11-27.
//
//

#ifndef ZJH_PlatformHelper_h
#define ZJH_PlatformHelper_h

#include "cocos2d.h"
#include "json.h"
#include <string>
#include "ZJH.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#ifndef ENABLE_IOS_PAY
#define ENABLE_IOS_PAY 1
#endif
#else
#ifndef ENABLE_IOS_PAY
#define ENABLE_IOS_PAY 1
#endif
#endif

class PlatformHelper
{
public:
    static void openCamera();//打开相机
    static void openPhotoLibrary();//打开图片库
    static void openSavedPhotosAlbum();//打开相册
    static bool openUrl(const char *url);//打开网页
    static void showToast(std::string str,int type = 0);
    
    static void WeChatShareText(const char *shareTitle, const char *shareContent);
    
    static void WeChatShare(const char *shareTitle, const char *shareContent, const char *shareUrl, const char *thumbImage);
    
    static void WeFriendsCircleShare(const char *shareTitle, const char *shareContent, const char *shareUrl, const char *thumbImage);
    
    static void WeFriendsCircleShareURL(const char *shareTitle, const char *shareContent, const char *shareUrl, const char *thumbImage, string url);
    
    static const char* encryptWithString(const char * content);
    
    static void startRecord();
    
    static void stopRecord();
    
    static bool isPlaying();
    
    static void playRecord(std::string filename);
    
    static std::map<std::string, double> getLocation();
    
    static std::string getGPSInfo();
    
    static void weiXinLoad(bool isloading = true);
    
    static void weiXin_sysInfo();
    
    static void WeChatShareImage(const char *shareTitle, const char *filePath);
    
    static void WeChatFrendsCircleShareImage(const char *shareTitle, const char *filePath);
    
    static void requestPay(Json::Value &json,int payType);
    
    static void IOSPay(std::string invoice, float tradeValue, int payType);
    
    static void cConViewV();
    static void cConViewH();
    
    static void sdkDoForVoid_C(std::string str,std::string data = "");
    
    static string sdkDoForStr_C(std::string str,std::string data = "");
    
    static int sdkDoForInt_C(std::string str,std::string data = "");
    
    static void askExitGame();
    
    static void exitWithoutParam();
    
    static void showUpdateDialog(Json::Value &json);
    
    static void removeIosUpdateTip();
    
    static void showOpenLocationDialog();
    
    static void removeIosLocationTip();
    
    static string loadCode();
    
    static string loadCodeNew();
    
    static string loadInviteCode();
    
    
    //百度地图获取经纬度
    static std::map<std::string,double> getUserCoordinate();
    static std::map<std::string, std::string> getUserAddress();
    static float getDistanceByCoordinate(cocos2d::Point start,cocos2d::Point end);

    static void vibrate();
    
    static void IOSPayClearId(const char * tradeID);
    
    static std::string iphoneVersion();
    
    static float dealLow_8Version();
    

    static std::string iphoneTypeName();
    
    static std::string getPhoneStateSig();
    
    static std::string getPowStateSig();

    //获取网络类型和运营商名称
    static std::string getNetWorkType();

    static std::map<std::string, std::string> phoneNetInfo();

    static bool checkIsInstallWeiXin();
    
    static void registWeiXinLoginId();
    
    static void saveStoreChainData(std::string IosTransactionId, std::string appStoreInvoice, std::string IosReceipt, std::string iosTotalFee, std::string userID);
    
    static std::map<std::string, std::string> getStoreChainData();
    
    static void clearStoreChainData();
    
    static void callPhone(std::string phone);
    
    static std::string getStringByPasteBord();
    
    static void setPvrEncryptionKey(unsigned int key1,unsigned int key2,unsigned int key3,unsigned int key4);
};
#endif
