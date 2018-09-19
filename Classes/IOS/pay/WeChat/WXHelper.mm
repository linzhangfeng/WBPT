#include "ZJH.h"
#include "WXHelper.h"

#include "WXApi.h"
#include <objc/objc.h>

#define WX_APPID "wx80e3685a6940bc04"
#define WX_LOGIN_ACESS_TOKEN "wx_login_acess_token"
#define WX_LOGIN_REFRESH_TOKEN "wx_login_refresh_token"

@interface WXPayInstance : NSObject <WXApiDelegate>
{
    //nothing
}
@end

@implementation WXPayInstance

- (NSString *)purchaseWithUID:(NSInteger) uid
              tradeType:(NSInteger) tradeType
                goodsId:(NSInteger) goodsId
             tradeValue:(NSInteger) tradeValue
              tradeAddr:(NSInteger) tradeAddr
              goodsName:(NSString *) goodsName
{
    return @"暂时用不到";
}


#pragma mark - WXApiDelegate
- (void)onReq:(BaseReq *)req
{
    //
}

- (void)onResp:(BaseResp *)resp
{
    //
}

@end

#pragma mark - WXHelper

WXHelper *WXHelper::sm_WXHelper = NULL;

WXHelper::WXHelper()
: m_payInstance(NULL)
{
    
}

WXHelper::~WXHelper()
{
    [m_payInstance release];
}

bool WXHelper::init()
{
    m_payInstance = [[WXPayInstance alloc] init];
    if (m_payInstance == NULL) {
        return false;
    }

    return true;
}

WXHelper *WXHelper::sharedWXHelper()
{
    if (!WXHelper::sm_WXHelper) {
        WXHelper::sm_WXHelper = new WXHelper();
        sm_WXHelper->init();
    }
    return WXHelper::sm_WXHelper;
}

void WXHelper::WXShareText(const char *shareTitle, const char *shareContent)
{
    weiXinShareTip();
    
    SendMessageToWXReq *req = [[SendMessageToWXReq alloc] init];
    req.bText = YES;
    req.scene = WXSceneSession;
    req.text = [NSString stringWithUTF8String:shareContent];
    [WXApi sendReq: req];
}

void WXHelper::purchase(const int tradeType, const int goodsId, const int tradeValue, std::string goodsName)
{
    /*
    NSInteger uid = ZJHModel::getInstance()->uid;
    NSInteger tradeAddr = 1;
    [m_payInstance purchaseWithUID:uid
                         tradeType:tradeType
                           goodsId:goodsId
                        tradeValue:tradeValue
                         tradeAddr:tradeAddr
                         goodsName:[NSString stringWithUTF8String:goodsName.c_str()]];
     */
}

void WXHelper::WXShare(const char *shareTitle, const char *shareContent, const char *shareUrl, const char *thumbImage)
{
    weiXinShareTip();
    
    WXMediaMessage *message = [WXMediaMessage message];
    message.title = [NSString stringWithUTF8String:shareTitle];
    message.description = [NSString stringWithUTF8String:shareContent];
    
    NSString *filePath = [[NSString alloc] initWithUTF8String: Utils::getImageSavePath(thumbImage).c_str()];
    
    if ([filePath isEqualToString:@""])
    {
        filePath = [[NSString alloc] initWithUTF8String: FileUtils::getInstance()->fullPathForFilename("IosShareIcon/temp_icon.png").c_str()];
    }
    
    UIImage *image = [[UIImage alloc] initWithContentsOfFile:filePath];
    [message setThumbImage: image];
    
    WXWebpageObject *webpageObject = [WXWebpageObject object];
    webpageObject.webpageUrl = [NSString stringWithUTF8String:shareUrl];
    message.mediaObject = webpageObject;
    
    SendMessageToWXReq *req = [[SendMessageToWXReq alloc] init];
    req.bText = NO;
    req.message = message;
    req.scene = WXSceneSession;
    
    [WXApi sendReq: req];
}

void WXHelper::WXFriendsCircleShare(const char *shareTitle, const char *shareContent, const char *shareUrl, const char *thumbImage)
{
    //显示玩呗斗牌
    [WXApi registerApp:@"wxf9f89e76d7b349e9"];
    
    if ([WXApi isWXAppInstalled] == false && ZJHModel::getInstance()->isOpenIPV6() == true)
    {
        PlatformHelper::showToast("暂未开放");
        return;
        
    }else if([WXApi isWXAppInstalled] == false && ZJHModel::getInstance()->isOpenIPV6() == false)
    {
        PlatformHelper::showToast("未安装微信");
        return;
    }

    
    WXMediaMessage *message = [WXMediaMessage message];
    message.title = [NSString stringWithUTF8String:shareTitle];
    message.description = [NSString stringWithUTF8String:shareContent];
    
    
    NSString *filePath = [[NSString alloc] initWithUTF8String: Utils::getImageSavePath(thumbImage).c_str()];
    
    UIImage *image = [[UIImage alloc] initWithContentsOfFile:filePath];
    [message setThumbImage: image];
    
    WXWebpageObject *webpageObject = [WXWebpageObject object];
    webpageObject.webpageUrl = [NSString stringWithUTF8String:shareUrl];
    message.mediaObject = webpageObject;
    
    SendMessageToWXReq *req = [[SendMessageToWXReq alloc] init];
    req.bText = NO;
    req.message = message;
    req.scene = WXSceneTimeline;
    
    [WXApi sendReq: req];
}

bool WXHelper::checkIsInstallWeiXin()
{
    //应用宝的ID
    return [WXApi isWXAppInstalled];
}

void WXHelper::weiXinShareTip()
{
    //老的ID
    [WXApi registerApp:@"wx80e3685a6940bc04"];
    
    if ([WXApi isWXAppInstalled] == false && ZJHModel::getInstance()->isOpenIPV6() == true)
    {
        PlatformHelper::showToast("暂未开放");
        return;
        
    }else if([WXApi isWXAppInstalled] == false && ZJHModel::getInstance()->isOpenIPV6() == false)
    {
        PlatformHelper::showToast("未安装微信");
        return;
    }
    
}
void WXHelper::WXShareImage(const char *shareTitle, const char *filePath)
{
    weiXinShareTip();
    
    WXMediaMessage *message = [WXMediaMessage message];
    message.title = [NSString stringWithUTF8String:shareTitle];
    
    NSString *filePathStr = [[NSString alloc] initWithUTF8String: filePath];
    WXImageObject *imageObject = [WXImageObject object];
    NSData *imageData = [NSData dataWithContentsOfFile:filePathStr];
    imageObject.imageData = imageData;
    message.mediaObject = imageObject;
    
    UIImage *image = [[UIImage alloc] initWithData:imageData];
    float scaleSize = MIN(250 / image.size.width, 250 / image.size.height);
    
    UIGraphicsBeginImageContext(CGSizeMake(image.size.width*scaleSize,image.size.height*scaleSize));
    [image drawInRect:CGRectMake(0, 0, image.size.width * scaleSize, image.size.height *scaleSize)];
    UIImage *scaledImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    [message setThumbImage:scaledImage];
    [image release];
    
    SendMessageToWXReq *req = [[SendMessageToWXReq alloc] init];
    req.bText = NO;
    req.message = message;
    req.scene = WXSceneSession;
    
    [WXApi sendReq: req];
}

void WXHelper::WXFriendsCircleShareImage(const char *shareTitle, const char *filePath)
{
    //显示玩呗斗牌
    [WXApi registerApp:@"wxf9f89e76d7b349e9"];
    
    if ([WXApi isWXAppInstalled] == false && ZJHModel::getInstance()->isOpenIPV6() == true)
    {
        PlatformHelper::showToast("暂未开放");
        return;
        
    }else if([WXApi isWXAppInstalled] == false && ZJHModel::getInstance()->isOpenIPV6() == false)
    {
        PlatformHelper::showToast("未安装微信");
        return;
    }
    

    WXMediaMessage *message = [WXMediaMessage message];
    message.title = [NSString stringWithUTF8String:shareTitle];
    
    NSString *filePathStr = [[NSString alloc] initWithUTF8String: filePath];
    WXImageObject *imageObject = [WXImageObject object];
    NSData *imageData = [NSData dataWithContentsOfFile:filePathStr];
    imageObject.imageData = imageData;
    message.mediaObject = imageObject;
    
    UIImage *image = [[UIImage alloc] initWithData:imageData];
    float scaleSize = MIN(250 / image.size.width, 250 / image.size.height);
    
    UIGraphicsBeginImageContext(CGSizeMake(image.size.width*scaleSize,image.size.height*scaleSize));
    [image drawInRect:CGRectMake(0, 0, image.size.width * scaleSize, image.size.height *scaleSize)];
    UIImage *scaledImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    [message setThumbImage:scaledImage];
    [image release];
    
    SendMessageToWXReq *req = [[SendMessageToWXReq alloc] init];
    req.bText = NO;
    req.message = message;
    req.scene = WXSceneTimeline;
    
    [WXApi sendReq: req];
}

void WXHelper::requestPay(std::string orderInfo)
{
    /*
    Json::Value json = Utils::ParseJsonStr(orderInfo);
    
    PayReq *req = [[[PayReq alloc] init] autorelease];
    req.openID      = [NSString stringWithUTF8String: json["appid"].asString().c_str()];
    req.partnerId   = [NSString stringWithUTF8String: json["partnerid"].asString().c_str()];
    req.prepayId    = [NSString stringWithUTF8String: json["prepayid"].asString().c_str()];
    req.nonceStr    = [NSString stringWithUTF8String: json["noncestr"].asString().c_str()];
    req.timeStamp   = atoi(json["timestamp"].asCString());
    req.package     = [NSString stringWithUTF8String: json["package"].asString().c_str()];
    req.sign        = [NSString stringWithUTF8String: json["sgin"].asString().c_str()];
    
    [WXApi sendReq:req];
     */
    
}

void WXHelper::WXLoading()
{
    //应用宝的ID
    [WXApi registerApp:@"wxf9f89e76d7b349e9"];
    
    if (![WXApi isWXAppSupportApi]) {
        PlatformHelper::showToast("暂未开放");
        return;
    }
    if ([WXApi isWXAppInstalled]) {
        SendAuthReq* req = [[[SendAuthReq alloc] init] autorelease];
        req.scope = @"snsapi_userinfo"; // @"post_timeline,sns"
        req.state = @"xxx";
        [WXApi sendReq:req];
    } else {
        PlatformHelper::showToast("暂未开放");
    }
}
