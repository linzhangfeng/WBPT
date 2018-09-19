//
//  WXApiManager.mm
//  SDKSample
//
//  Created by Jeason on 16/07/2015.
//
//

#include "ZJH.h"
#import "WXApiManager.h"
#include "../../IOSToast.h"
#include "MyNotification.h"

@implementation WXApiManager

#pragma mark - LifeCycle
+(instancetype)sharedManager {
    static dispatch_once_t onceToken;
    static WXApiManager *instance;
    dispatch_once(&onceToken, ^{
        instance = [[WXApiManager alloc] init];
    });
    return instance;
}

- (void)dealloc {
    self.delegate = nil;
    [super dealloc];
}

#pragma mark - WXApiDelegate
- (void)onResp:(BaseResp *)resp {
    if ([resp isKindOfClass:[SendMessageToWXResp class]]) {
        if (_delegate
            && [_delegate respondsToSelector:@selector(managerDidRecvMessageResponse:)]) {
            SendMessageToWXResp *messageResp = (SendMessageToWXResp *)resp;
            [_delegate managerDidRecvMessageResponse:messageResp];
        }
    } else if ([resp isKindOfClass:[SendAuthResp class]]) {
        if (_delegate
            && [_delegate respondsToSelector:@selector(managerDidRecvAuthResponse:)]) {
            SendAuthResp *authResp = (SendAuthResp *)resp;
            [_delegate managerDidRecvAuthResponse:authResp];
        }
        
//        SendAuthResp *temp = (SendAuthResp *)resp;
//        auto *url = new char[200];
//        sprintf(url,"https://api.weixin.qq.com/sns/oauth2/access_token?appid=%s&code=%s&grant_type=authorization_code",WX_APPID,[temp.code UTF8String]);
//        Json::Value json;
//        CCHttpAgent::getInstance()->sendHttpUrlPost([=](std::string tag)
//                                                    {
//                                                        CCHttpPacket *packet = CCHttpAgent::getInstance()->packets[tag];
//                                                        if (packet) {
//                                                            Json::Value json = packet->recvVal["data"];
//                                                            if (!json.isNull()) {
//                                                                string dd = json["access_token"].toStyledString();
//                                                                string cc = json["openid"].toStyledString();
//                                                                UserDefault::getInstance()->setStringForKey(WX_ACESS_TOKEN, json["access_token"].toStyledString());
//                                                                UserDefault::getInstance()->setStringForKey(WX_REFRESH_TOKEN, json["refresh_token"].toStyledString());
//                                                                UserDefault::getInstance()->setStringForKey(WX_OPENID, json["openid"].toStyledString());
//                                                            }
//                                                        }
//                                                    },url,json.toStyledString(), "wx_refreshtoken");
        
    } else if ([resp isKindOfClass:[AddCardToWXCardPackageResp class]]) {
        if (_delegate
            && [_delegate respondsToSelector:@selector(managerDidRecvAddCardResponse:)]) {
            AddCardToWXCardPackageResp *addCardResp = (AddCardToWXCardPackageResp *)resp;
            [_delegate managerDidRecvAddCardResponse:addCardResp];
        }
    }else if([resp isKindOfClass:[PayResp class]]){
        //支付返回结果，实际支付结果需要去微信服务器端查询
//        NSString *strMsg,*strTitle = [NSString stringWithFormat:@"支付结果"];
//        
//        switch (resp.errCode) {
//            case WXSuccess:
//                strMsg = @"支付结果：成功！";
//                NSLog(@"支付成功－PaySuccess，retcode = %d", resp.errCode);
//                break;
//                
//            default:
//                strMsg = [NSString stringWithFormat:@"支付结果：失败！retcode = %d, retstr = %@", resp.errCode,resp.errStr];
//                NSLog(@"错误，retcode = %d, retstr = %@", resp.errCode,resp.errStr);
//                break;
//        }
//        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:strTitle message:strMsg delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil, nil];
//        [alert show];
//        [alert release];
        
        int errCode = resp.errCode;
        if (errCode == WXSuccess) {
            //
            MyNotification::getInstance()->postNotify("cz_succ_notify");
        } else if (errCode == WXErrCodeCommon) {
            //
        } else if (errCode == WXErrCodeUserCancel) {
            IOSToast::showToast("取消支付");
        } else if (errCode == WXErrCodeSentFail) {
            IOSToast::showToast("发送失败");
        } else if (errCode == WXErrCodeAuthDeny) {
            IOSToast::showToast("授权失败");
        } else if (errCode == WXErrCodeUnsupport) {
            IOSToast::showToast("微信不支持");
        }
    }
}

- (void)onReq:(BaseReq *)req {
    if ([req isKindOfClass:[GetMessageFromWXReq class]]) {
        if (_delegate
            && [_delegate respondsToSelector:@selector(managerDidRecvGetMessageReq:)]) {
            GetMessageFromWXReq *getMessageReq = (GetMessageFromWXReq *)req;
            [_delegate managerDidRecvGetMessageReq:getMessageReq];
        }
    } else if ([req isKindOfClass:[ShowMessageFromWXReq class]]) {
        if (_delegate
            && [_delegate respondsToSelector:@selector(managerDidRecvShowMessageReq:)]) {
            ShowMessageFromWXReq *showMessageReq = (ShowMessageFromWXReq *)req;
            [_delegate managerDidRecvShowMessageReq:showMessageReq];
        }
    } else if ([req isKindOfClass:[LaunchFromWXReq class]]) {
        if (_delegate
            && [_delegate respondsToSelector:@selector(managerDidRecvLaunchFromWXReq:)]) {
            LaunchFromWXReq *launchReq = (LaunchFromWXReq *)req;
            [_delegate managerDidRecvLaunchFromWXReq:launchReq];
        }
    }
}

@end
