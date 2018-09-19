#ifndef __ZJH__IOSHelper__
#define __ZJH__IOSHelper__

#include "cocos2d.h"



//一些IOS相关的函数
class IOSHelper {
public:
    //打开外部网站
    static bool openUrl(const char *url);
    
    //手机震动
    static void IosVibrate();
    
    //将设备标示符添加到钥匙串
    static void setUUIDToKeyChain();
    //从钥匙串取出设备标示符
    static const char *getUUIDFromKeyChain();
    //重置标示符
    static void resetUUID();
    
    //获取IDFA
    static const char *getIDFA();
    
    //获取渠道号，渠道名
    static int getChannelId();
    static const char* getChannelName();
    
    //获取版本号
    static const char* getAppVersion();
    
    //获取系统版本号
    static float getIosVersion();
    
    //复制到黏贴板
    static void copyToPasteboard(std::string str);
    
    static std::string getStringByPasteBoard();
    
    //相机相关
    static void openCamera();//打开相机
    static void openPhotoLibrary();//打开图片库
    static void openSavedPhotosAlbum();//打开相册
    
    //TalkingData
    static void trackEventWithLabel(const char *eventId, const char *eventLabel);
    
    //分享相关
    static void WXShareText(const char *shareTitle, const char *shareContent);
    
    static void WeChatShare(const char *shareTitle, const char *shareContent, const char *shareUrl, const char *thumbImage);
    static void WeFriendsCircleShare(const char *shareTitle, const char *shareContent, const char *shareUrl, const char *thumbImage);
    static void WeChatShareImage(const char *shareTitle, const char *filePath);//filePath必须是文件在手机上的完整存储路径
    static void WeFriendsCircleShareImage(const char *shareTitle, const char *filePath);//filePath必须是文件在手机上的完整存储路径
    //支付相关
    static void IOSPay(std::string invoice, float tradeValue, int payType);
    
    static void IOSPayClearId(const char * tradeID);
    
    static void weiXinLoad(bool isloading = true);//微信登录
    static void weiXin_sysInfo();
    
    //RSA加密
    static const char *encryptWithString(const char * );
    
    //获得系统字体
    std::string systemFontName();
    
    //百度地图获取经纬度
    static std::map<std::string,double> getUserCoordinate();
    static std::map<std::string, std::string> getUserAddress();
    static float getDistanceByCoordinate(cocos2d::Point start,cocos2d::Point end);
    
    //系统地图获取经纬度
    static std::map<std::string, double> getLocation();
    
    static std::map<std::string, std::string> cityMap();

    
    static void startRecord();
    static void stopRecord();
    static bool isPlaying();
    static void playRecord(std::string filename);

    static void cConViewH();
    static void cConViewV();
    
    static std::string loadCode();
    
    static std::string loadCodeNew();
    
    static std::string loadInviteCode();
    
    //ios更新提示
    static void iosUpdateTip(int force, std::string desc, std::string versionName, std::string URL );
    
    static void removeIosUpdateTip();
    
    //定位弹窗提示
    static void iosOpenLocationTip();
    static void removeIosOpenLocationTip();
    
    static std::string iphoneType();
    
    static std::string iphoneTypeName();
    
    static std::string iphoneVersion();
    
    //获取ios手机网络状态和运营商名称
    static std::map<std::string, std::string> phoneNetInfo();
    
    static bool checkIsInstallWeiXin();
    
    static int getDianLiangValue();
    
    static std::map<std::string, std::string> getSignalValue();
    
    //注册微信登录的ID
    static void registWeiXinLoginId();
    
    //存储appstore购买的数据
    static void saveStoreChainData(std::string IosTransactionId, std::string appStoreInvoice, std::string IosReceipt, std::string iosTotalFee, std::string userID);
    
    static std::map<std::string, std::string> getStoreChainData();
    
    //清除appstore购买的数据
    static void clearStoreChainData();
    
    static void callPhone(std::string phone);
    
    //获取ios系统（处理8.0.2）
    static float dealLow_8Version();
    
};

#endif /* defined(__ZJH__IOSToast__) */
