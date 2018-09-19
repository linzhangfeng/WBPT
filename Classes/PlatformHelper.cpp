//
//  PlatformHelper.cpp
//  ZJH
//
//  Created by wujunru on 14-4-16.
//
//

#include "PlatformHelper.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOS/IOSHelper.h"
#include "IOS/IOSToast.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "Jni/MiscHelper.h"
#endif

void PlatformHelper::showToast(std::string str,int type)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (type == 0) {
        IOSToast::showToast(str.c_str(), IOSToast::ToastDurationNormal);
    }
    else
    {
        IOSToast::showToast(str.c_str(), IOSToast::ToastDurationLong);
    }
#else
    if (type == 0)
    {
        sdkDoForVoid_C("showToast_Short",str);
    }
    else
    {
        sdkDoForVoid_C("showToast_Long",str);
    }
#endif
}

bool PlatformHelper::openUrl(const char *url)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return IOSHelper::openUrl(url);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
#endif
}

void PlatformHelper::openCamera()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IOSHelper::openCamera();;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    std::string saveFile = Utils::getTmpFoldPath() + "temp_image.png";
    CCHttpAgent::getInstance()->_makeMultiDir(saveFile.c_str());
    sdkDoForVoid("openCamera",saveFile.c_str());
#endif
}

void PlatformHelper::openPhotoLibrary()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IOSHelper::openPhotoLibrary();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    std::string saveFile = Utils::getTmpFoldPath() + "temp_image.png";
    CCHttpAgent::getInstance()->_makeMultiDir(saveFile.c_str());
    sdkDoForVoid("openPhotoLibrary",saveFile.c_str());
#endif
    
}

void PlatformHelper::openSavedPhotosAlbum()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IOSHelper::openSavedPhotosAlbum();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    std::string saveFile = Utils::getTmpFoldPath() + "temp_image.png";
    CCHttpAgent::getInstance()->_makeMultiDir(saveFile.c_str());
    sdkDoForVoid("openSavedPhotosAlbum",saveFile.c_str());
#endif
}

void PlatformHelper::WeChatShareText(const char *shareTitle, const char *shareContent)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IOSHelper::WXShareText(shareTitle,shareContent);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    Json::Value json;
    json["title"] = shareTitle;
    json["text"] = shareContent;
    json["imageUrl"] = "";
    json["url"] = "";
    sdkDoForVoid("WeChatShare",json.toStyledString().c_str());
#endif
}

void PlatformHelper::WeChatShare(const char *shareTitle, const char *shareContent, const char *shareUrl, const char *thumbImage)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IOSHelper::WeChatShare(shareTitle,shareContent,shareUrl,thumbImage);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    Json::Value json;
    json["title"] = shareTitle;
    json["text"] = shareContent;
    json["imageUrl"] = thumbImage;
    json["url"] = shareUrl;
    sdkDoForVoid("WeChatShare",json.toStyledString().c_str());
#endif

}

void PlatformHelper::WeFriendsCircleShareURL(const char *shareTitle, const char *shareContent, const char *shareUrl, const char *thumbImage, string url)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IOSHelper::WeFriendsCircleShare(shareTitle,shareContent,shareUrl,thumbImage);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    Json::Value json;
    json["title"] = shareTitle;
    json["text"] = shareContent;
    json["imageUrl"] = url;
    json["url"] = shareUrl;
    sdkDoForVoid("WeFriendsCircleShareNormal",json.toStyledString().c_str());//服务器
#endif
}
void PlatformHelper::WeFriendsCircleShare(const char *shareTitle, const char *shareContent, const char *shareUrl, const char *thumbImage)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IOSHelper::WeFriendsCircleShare(shareTitle,shareContent,shareUrl,thumbImage);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    Json::Value json;
    json["title"] = shareTitle;
    json["text"] = shareContent;
    json["imageUrl"] = thumbImage;
    json["url"] = shareUrl;
    sdkDoForVoid("WeFriendsCircleShareNormal",json.toStyledString().c_str());//服务器
#endif
    
}

const char* PlatformHelper::encryptWithString(const char * content)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return IOSHelper::encryptWithString(content);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return sdkDoForStr("encryptWithString",content).c_str();
#endif
}

void PlatformHelper::startRecord()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IOSHelper::startRecord();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    std::string saveFile = Utils::getTmpFoldPath() + "temp_sound.spx";
    CCHttpAgent::getInstance()->_makeMultiDir(saveFile.c_str());
    sdkDoForVoid("startRecord",saveFile.c_str());
#endif
}

bool PlatformHelper::isPlaying()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return IOSHelper::isPlaying();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return sdkDoForInt("isPlaying","") == 1;
#endif
}

void PlatformHelper::stopRecord()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IOSHelper::stopRecord();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    sdkDoForVoid("stopRecord","");
#endif
}

void PlatformHelper::playRecord(std::string filename)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IOSHelper::playRecord(Utils::getSoundSavePath(filename));
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    sdkDoForVoid("playRecord",Utils::getSoundSavePath(filename).c_str());
#endif
}

std::map<std::string, double> PlatformHelper::getLocation()
{

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return IOSHelper::getUserCoordinate();
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    std::string str = sdkDoForStr("getLocation","");
    Json::Value json = Utils::ParseJsonStr(str);
    std::map<std::string, double> maps;
    maps["latitude"] = json["latitude"].asDouble();
    maps["longitude"] = json["longitude"].asDouble();
    return maps;
#endif
}

std::string PlatformHelper::getGPSInfo()
{
    double latitude = 0;
    double longitude = 0;
    std::string city = "";
    std::string province = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::map<std::string, double> locationMap = IOSHelper::getUserCoordinate();
    latitude = locationMap["latitude"];
    longitude = locationMap["longitude"];
    
    std::map<std::string, std::string> addressMap = IOSHelper::getUserAddress();
    city = addressMap["city"];
    province = addressMap["province"];
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    std::string str = sdkDoForStr("getLocation","");
    Json::Value json = Utils::ParseJsonStr(str);
    latitude = json["latitude"].asDouble();
    longitude = json["longitude"].asDouble();
    city = json["city"].asString();
    province = json["province"].asString();
#endif
    Json::Value json1;
    json1["latitude"] = latitude;
    json1["longitude"] = longitude;
    json1["city"] = city;
    json1["province"] = province;
    std::string result = json1.toStyledString();
//    result += latitude;
//    result += ",";
//    result += longitude;
//    result += ",";
//    result += city;
//    result += ",";
//    result += province;
    return result;
}

void PlatformHelper::weiXinLoad(bool isloading)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IOSHelper::weiXinLoad(isloading);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (isloading)
    {
        sdkDoForVoid("wx_login","");
    }
    else
    {
        sdkDoForVoid("wx_bind","");
    }
#endif
}

void PlatformHelper::weiXin_sysInfo()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IOSHelper::weiXin_sysInfo();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    sdkDoForVoid("wx_sysInfo","");
#endif
}

void PlatformHelper::WeChatShareImage(const char *shareTitle, const char *filePath)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IOSHelper::WeChatShareImage(shareTitle,filePath);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    Json::Value json;
    json["title"] = shareTitle;
    json["imagePath"] = filePath;
    sdkDoForVoid("WeChatShareImage",json.toStyledString().c_str());
    
#endif
}

//分享到微信朋友圈
void PlatformHelper::WeChatFrendsCircleShareImage(const char *shareTitle, const char *filePath){
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        IOSHelper::WeFriendsCircleShareImage(shareTitle,filePath);
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        Json::Value json;
        json["title"] = shareTitle;
        json["imagePath"] = filePath;
        sdkDoForVoid("WeFriendsCircleShareImage",json.toStyledString().c_str());
    #endif
}

void PlatformHelper::requestPay(Json::Value &data,int payType)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    printf("buy start 11111 = %ld", time(NULL));
    Json::Value json;
    char buff[128];
    double reqFee = data["order"]["price"].asDouble();
    snprintf(buff, sizeof(buff), "%f", reqFee);
    json["reqFee"] = buff;
    json["tradeDesc"] = data["order"]["description"].asString();
    json["tradeName"] = data["order"]["name"].asString();
    json["notifyUrl"] = data["extData"]["callback"].asString();
    json["extInfo"] = data["order"]["orderId"].asString();
    
    CCHttpAgent::getInstance()->sendPayHttpPost([=](std::string tag)
                                                {
                                                    CCHttpPacket *packet = CCHttpAgent::getInstance()->packets[tag];
                                                    if (packet)
                                                    {
                                                        if (packet->status == 3)
                                                        {
                                                            
                                                            packet->status = 0;
                                                            
                                                            if (!packet->recvVal["result"].isNull())
                                                            {
                                                                std::string result = packet->recvVal["result"].asString();
                                                                Json::Value orderInfo = Utils::ParseJsonStr(result);
                                                                std::string invoice = orderInfo["invoice"].asString();
                                                                
                                                                //保存商品价格、发票
                                                                ZJHModel::getInstance()->iosTotalFee = buff;
                                                                ZJHModel::getInstance()->appStoreInvoice = invoice;
                                                                ZJHModel::getInstance()->iosPrice = reqFee;
                                                                
                                                                PlatformHelper::IOSPay(invoice, (float)reqFee, payType);
                                                            }
                                                            else
                                                            {
                                                                
                                                            }
                                                            
                                                        }else
                                                        {
                                                            PlatformHelper::showToast("网络异常，请重试。");
                                                            __NotificationCenter::getInstance()->postNotification("appstore_deleteLoad");
                                                        }
                                                        
                                                        //删除packet
                                                        CCHttpAgent::getInstance()->packets.erase(tag);
                                                        delete packet;
                                                    }
                                                },
                                                "purchase/v2", json.toStyledString(), "requestpay");
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    double reqFee = data["order"]["price"].asDouble();
    Json::Value json;
    json["backUrl"] = data["extData"]["callback"].asString();
    json["userId"] = ZJHModel::getInstance()->uid;
    json["reqFee"] = reqFee;
    json["orderId"] = data["order"]["orderId"].asString();
    json["tradeName"] = data["order"]["name"].asString();
    json["payType"] = payType;
    sdkDoForVoid("requestPay",json.toStyledString().c_str());
#endif
}

void PlatformHelper::IOSPay(std::string invoice, float tradeValue, int payType)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IOSHelper::IOSPay(invoice,tradeValue,payType);
#else
    
#endif
}

void PlatformHelper::cConViewH()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IOSHelper::cConViewH();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    sdkDoForVoid("chang_view_portrait","");
#endif
}

void PlatformHelper::cConViewV()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IOSHelper::cConViewV();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    sdkDoForVoid("chang_view_landscape","");
#endif
}

void PlatformHelper::askExitGame()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    sdkDoForVoid("askExitGame","");
#endif
}

void PlatformHelper::exitWithoutParam()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    Director::getInstance()->end();
    exit(0);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    sdkDoForVoid("exitGameWithoutParam","");
#endif
}

void PlatformHelper::removeIosUpdateTip()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IOSHelper::removeIosUpdateTip();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
#endif
}
void PlatformHelper::showUpdateDialog(Json::Value &json)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    IOSHelper::iosUpdateTip(json["force"].asInt(),json["desc"].asString(), json["name"].asString(), json["url"].asString());
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    sdkDoForVoid("showUpdateDialog",json.toStyledString().c_str());
#endif
}

void PlatformHelper::showOpenLocationDialog()
{
    double lastTime = UserDefault::getInstance()->getDoubleForKey("openLocation_tips",0.0);
    long long currentTime = Utils::getCurrentSecend();
    double interval = (currentTime-lastTime)*0.001/60/60;

    if(interval > 24)
    {
        UserDefault::getInstance()->setDoubleForKey("openLocation_tips", currentTime);
        
        #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        
        IOSHelper::iosOpenLocationTip();
        
        #elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        
        #endif
    }

}

void PlatformHelper::removeIosLocationTip()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IOSHelper::removeIosOpenLocationTip();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
#endif
    
}
std::string PlatformHelper::iphoneVersion()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return IOSHelper::iphoneVersion();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return "";
#endif
}

float PlatformHelper::dealLow_8Version()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return IOSHelper::dealLow_8Version();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return 0.0f;
#endif
}

void PlatformHelper::registWeiXinLoginId()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    IOSHelper::registWeiXinLoginId();
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
#endif
}

std::string PlatformHelper::iphoneTypeName()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return IOSHelper::iphoneTypeName();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return "";
#endif
}


string PlatformHelper::loadCodeNew()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return IOSHelper::loadCodeNew();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return "";
#endif
}

string PlatformHelper::loadInviteCode()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return IOSHelper::loadInviteCode();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return "";
#endif
}

string PlatformHelper::loadCode()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return IOSHelper::loadCode();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return sdkDoForStr("web_code","");
#endif
}

std::map<std::string,double> PlatformHelper::getUserCoordinate() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return IOSHelper::getUserCoordinate();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    std::string str = sdkDoForStr("getLocation","");
    Json::Value json = Utils::ParseJsonStr(str);
    std::map<std::string, double> maps;
    maps["latitude"] = json["latitude"].asDouble();
    maps["longitude"] = json["longitude"].asDouble();
    return maps;
#endif
}

std::map<std::string, std::string> PlatformHelper::getUserAddress() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return IOSHelper::getUserAddress();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#endif
}

float PlatformHelper::getDistanceByCoordinate(cocos2d::Point start,cocos2d::Point end) {

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return IOSHelper::getDistanceByCoordinate(start, end);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    Json::Value json;
    json["start_latitude"] = start.x;
    json["start_longitude"] = start.y;
    json["end_latitude"] = end.x;
    json["end_longitude"] = end.y;
    return sdkDoForInt("getDistanceByCoordinate",json.toStyledString().c_str());
#endif
}

string PlatformHelper::sdkDoForStr_C(std::string str,std::string data)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return sdkDoForStr(str.c_str(),data.c_str());
#else
    log("sdkDoForStr cmd[%s] data[%s]",str.c_str(),data.c_str());
    if (str == "phonename")
    {
        return iphoneTypeName();
    }
    else if (str == "phoneos")
    {
        return iphoneVersion();
    }
    else if (str == "phonemac")
    {
        return "mac";
    }
    else if (str == "phoneimei")
    {
        //使用存在钥匙串里的UUID做imei
        return IOSHelper::getIDFA();
    }
    else if (str == "phoneimsi")
    {
        //使用存在钥匙串里的UUID做imsi
        return IOSHelper::getIDFA();
    }
    else if (str == "UrlEncode")
    {
        return data;
    }
    else if (str == "channel_id")
    {
        //IOS版本渠道好为10000
        return "86011079";
    }
    else if (str == "getAppInterMetaStr")
    {
        if (data == "login_sdk")
        {
            return "ihuizhi";
        }
        else if (data == "pay_sdk")
        {
            return "ihuizhi";
        }
        else if (data == "inner_channel_id")
        {
            //IOS版本渠道好为10000
            return "10000";
        }
        else if (data == "appid")
        {
            //IOS版本的appid为3
            return "3";
        }
    }
    else if (str == "sdkDoSomething")
    {
        if (data == "ext1")
        {
            return "3d1babbd9703b57b";
        }
        else if (data == "ext2")
        {
            return "MjFiMjY0MmZkODdmYWRiODRmODkuODcwMjMzNDMuMTQ2MTg1MTI5MTczMw%3D%3D";
        }
        else if (data == "ext3")
        {
            return "";
        }
    }
    return "";
#endif
}

int PlatformHelper::sdkDoForInt_C(std::string str,std::string data)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return sdkDoForInt(str.c_str(),data.c_str());
#else
    log("sdkDoForInt cmd[%s] data[%s]",str.c_str(),data.c_str());
    if (str == "getAppInterMetaInt")
    {
        if (data == "third_login")
        {
            return 0;
        }
    }
    else if (str == "isRoot")
    {
        return 0;
    }
    else if (str == "isEmulator")
    {
        return 0;
    }
    else if (str == "useServer")
    {
        return 1;
    }
    return 0;
#endif
}

std::string PlatformHelper::getStringByPasteBord()
{

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return "";
#else
    return IOSHelper::getStringByPasteBoard();
#endif
    
    
    
}

void PlatformHelper::sdkDoForVoid_C(std::string str,std::string data)
{
    log("-----sdkDoForVoid_C");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    sdkDoForVoid(str.c_str(),data.c_str());
#else
    //MessageBox(data.c_str(), "提示");
    log("sdkDoForVoid cmd[%s] data[%s]",str.c_str(),data.c_str());
    if (str == "InitSDK")
    {
        
    }
    else if (str == "showAvatarDialog")
    {
        
    }
    else if (str == "setVibrate")
    {
        vibrate();
    }
    else if (str == "copyToPasteboard")
    {
        IOSHelper::copyToPasteboard(data);
    }
    else if (str == "requestPay")
    {
        log("去充值咯");
    }
    else if (str == "btn_click" || str == "scene_enter")
    {
        IOSHelper::trackEventWithLabel(str.c_str(), data.c_str());
    }
#endif
}

void PlatformHelper::vibrate()
{
    if (ZJHModel::getInstance()->vibrate)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        return sdkDoForVoid("setVibrate","");
#else
        IOSHelper::IosVibrate();
#endif
    }
}

void PlatformHelper::IOSPayClearId(const char * tradeID)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IOSHelper::IOSPayClearId(tradeID);
#endif
}

std::string PlatformHelper::getNetWorkType()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::map<std::string, std::string> map = IOSHelper::phoneNetInfo();
    Json::Value value;
    value["networkType"] = map["networkType"];
    value["operatorType"] = map["operatorType"];
    return value.toStyledString();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return sdkDoForStr_C("netWorkType");
#endif
}

std::map<std::string, std::string> PlatformHelper::phoneNetInfo()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return IOSHelper::phoneNetInfo();
#endif
}

bool PlatformHelper::checkIsInstallWeiXin()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return IOSHelper::checkIsInstallWeiXin();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return true;
#endif
}

string PlatformHelper::getPhoneStateSig(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::map<std::string, std::string> map = IOSHelper::getSignalValue();
    Json::Value value;
    value["level"] = std::atoi(map["level"].c_str());
    value["netType"] = map["netType"];
    return value.toStyledString();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return PlatformHelper::sdkDoForStr_C("phone_state");;
#endif
}

string PlatformHelper::getPowStateSig(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    int level = IOSHelper::getDianLiangValue();
    Json::Value json;
    json["level"] = level;
    return json.toStyledString();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return PlatformHelper::sdkDoForStr_C("power_state");
#endif
}

//
void PlatformHelper::saveStoreChainData(std::string IosTransactionId, std::string appStoreInvoice, std::string IosReceipt, std::string iosTotalFee, std::string userID)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
   
    IOSHelper::saveStoreChainData(IosTransactionId, appStoreInvoice, IosReceipt, iosTotalFee, userID);
    
#endif
}

std::map<std::string, std::string> PlatformHelper::getStoreChainData()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    return IOSHelper::getStoreChainData();
    
#endif
}

//清除appstore购买的数据
void PlatformHelper::clearStoreChainData()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    IOSHelper::clearStoreChainData();
    
#endif
}

void PlatformHelper::callPhone(std::string phone)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IOSHelper::callPhone(phone);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    sdkDoForVoid("callPhone",phone.c_str());
#endif
}

void PlatformHelper::setPvrEncryptionKey(unsigned int key1,unsigned int key2,unsigned int key3,unsigned int key4)
{
    ZipUtils::setPvrEncryptionKey(key1, key2, key3, key4);
}
