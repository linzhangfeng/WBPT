/****************************************************************************
 Copyright (c) 2010 cocos2d-x.org

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#import "AppController.h"
#import "platform/ios/CCEAGLView-ios.h"
#import "cocos2d.h"
#import "AppDelegate.h"
#import "RootViewController.h"
#import "RootViewControllerV.h"
#import "TalkingData.h"
#import "PlatformHelper.h"
#import "LocationGetIOS.h"
#import "BaiduMapManager.h"

#import "MyNotification.h"
#import <BaiduMapAPI_Base/BMKMapManager.h>
#import <BaiduMapAPI_Search/BMKGeocodeSearch.h>

#if (ENABLE_IOS_PAY == 1)
#import "WXApiManager.h"

#endif

@implementation AppController

#pragma mark -
#pragma mark Application lifecycle

// cocos2d application instance
static AppDelegate s_sharedApplication;

static string web_code;
static string web_codeNew;
static string invite_code;

static int dianLiangValue;
static std::map<std::string, std::string> signalValue;

static AppController *s_self;

//- (BOOL)application:(UIApplication *)application
//            openURL:(NSURL *)url
//  sourceApplication:(NSString *)sourceApplication
//         annotation:(id)annotation
//{
//    NSLog(@"sourceApplication: %@", sourceApplication);
//    NSLog(@"URL scheme:%@", [url scheme]);
//    NSLog(@"URL query: %@", [url query]);
//    web_code = "";
////    UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"打开啦"
////                                                        message:[url query]
////                                                       delegate:nil
////                                              cancelButtonTitle:@"OK"
////                                              otherButtonTitles:nil];
////    [alertView show];
//    
//    NSString* code = [url query];
//    if (code != nil)
//    {
//        std::string str = [code UTF8String];
//        web_code = str;
//    }
//    return YES;
//}


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    
    //百度地图
    BaiduMapManager *temp = [BaiduMapManager shareManager];

    //TalkingData
    [TalkingData setExceptionReportEnabled:YES];
    [TalkingData sessionStarted:@"1A924E5FEFFB44908C66FB8260700BFB" withChannelId:@"AppStore"];
    
#if (ENABLE_IOS_PAY == 1)
    //微信
    //[WXApi registerApp:@"wxf9f89e76d7b349e9"];
    
#endif

    cocos2d::Application *app = cocos2d::Application::getInstance();
    app->initGLContextAttrs();
    cocos2d::GLViewImpl::convertAttrs();

    // Override point for customization after application launch.

    // Add the view controller's view to the window and display.
    window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];

    // Init the CCEAGLView
    CCEAGLView *eaglView = [CCEAGLView viewWithFrame: [window bounds]
                                         pixelFormat: (NSString*)cocos2d::GLViewImpl::_pixelFormat
                                         depthFormat: cocos2d::GLViewImpl::_depthFormat
                                  preserveBackbuffer: NO
                                          sharegroup: nil
                                       multiSampling: NO
                                     numberOfSamples: 0 ];
    
    // Enable or disable multiple touches
    [eaglView setMultipleTouchEnabled:NO];

    // Use RootViewController manage CCEAGLView 
    _viewController = [[RootViewController alloc] initWithNibName:nil bundle:nil];
    _viewController.wantsFullScreenLayout = YES;
    
    
    s_self = self;
    _viewControllerV = [[RootViewControllerV alloc] initWithNibName:nil bundle:nil];
    _viewControllerV.wantsFullScreenLayout = YES;
    _viewControllerV.view = eaglView;

    // Set RootViewController to window
    if ( [[UIDevice currentDevice].systemVersion floatValue] < 6.0)
    {
        // warning: addSubView doesn't work on iOS6
        [window addSubview: _viewControllerV.view];
    }
    else
    {
        // use this method on ios6
        [window setRootViewController:_viewControllerV];
    }

    [window makeKeyAndVisible];

    [[UIApplication sharedApplication] setStatusBarHidden:true];

    [[UIApplication sharedApplication] setIdleTimerDisabled:YES];
    
    // IMPORTANT: Setting the GLView should be done after creating the RootViewController
    cocos2d::GLView *glview = cocos2d::GLViewImpl::createWithEAGLView(eaglView);
    cocos2d::Director::getInstance()->setOpenGLView(glview);

    app->run();


    //实时监测手机电量和信号
    [self didLoad];
    
    return YES;
}

- (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url {
    NSString *text = [[url host] stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    if (text) {
        std::string msg = [text UTF8String];
        
        const std::string findFlag = "webcode=";
        size_t cutPos = msg.find(findFlag);
        if (cutPos != std::string::npos) {
            std::string inviteCode = msg.substr(cutPos + findFlag.length());
            if (!inviteCode.empty()) {
                PlatformHelper::sdkDoForVoid_C("copyToPasteboard", inviteCode);
                PlatformHelper::showToast("房间邀请码已复制到粘贴板", 1);
            }
        }
    }
#if (ENABLE_IOS_PAY == 1)
    return [WXApi handleOpenURL:url delegate:[WXApiManager sharedManager]];
#endif
    
    return true;
}



- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation {
    
    //跳转支付宝钱包进行支付，需要将支付宝钱包的支付结果回传给SDK
    if ([url.host isEqualToString:@"safepay"]) {
#if (ENABLE_IOS_PAY == 1)
       

#endif
        return YES;
    }
    else if ([[url scheme] isEqualToString:@"shareapp"])
    {
        web_code = "";
        //    UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"打开啦"
        //                                                        message:[url query]
        //                                                       delegate:nil
        //                                              cancelButtonTitle:@"OK"
        //                                              otherButtonTitles:nil];
        //    [alertView show];
        
        NSString* code = [url query];
        if (code != nil)
        {
            std::string str = [code UTF8String];
            web_code = str;
            MyNotification::getInstance()->postNotify("game_reload");
        }
        return YES;
    }
    else
    {
#if (ENABLE_IOS_PAY == 1)
        return [WXApi handleOpenURL:url delegate:[WXApiManager sharedManager]];
#endif
    }
    
    return YES;
}


- (void)applicationWillResignActive:(UIApplication *)application {
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
     //We don't need to call this method any more. It will interupt user defined game pause&resume logic
    /* cocos2d::Director::getInstance()->pause(); */
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
     //We don't need to call this method any more. It will interupt user defined game pause&resume logic
    /* cocos2d::Director::getInstance()->resume(); */
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
    cocos2d::Application::getInstance()->applicationDidEnterBackground();
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
    cocos2d::Application::getInstance()->applicationWillEnterForeground();
}

- (void)applicationWillTerminate:(UIApplication *)application {
    /*
     Called when the application is about to terminate.
     See also applicationDidEnterBackground:.
     */
}


+(void)changeRootViewControllerH{
    CCEAGLView *__glView = (CCEAGLView *)s_self.viewControllerV.view;
    s_self.viewControllerV.view = nil;
    s_self.viewController.view = __glView;
    
    if ([[UIDevice currentDevice].systemVersion floatValue] < 6.0)
    {
        [s_self->window addSubview:s_self.viewController.view];
    }else{
        [s_self->window setRootViewController:s_self.viewController];
    }
    

}

+(void)changeRootViewControllerV{
    CCEAGLView *__glView = (CCEAGLView *)s_self.viewController.view;
    s_self.viewController.view = nil;
    s_self.viewControllerV.view = __glView;
    
    if ([[UIDevice currentDevice].systemVersion floatValue] < 6.0)
    {
        [s_self->window addSubview:s_self.viewControllerV.view];
    }else{
         [s_self->window setRootViewController:s_self.viewControllerV];
    }
   
}

//推荐好友的拉起
+(std::string)loadInviteCode
{
    string temp = invite_code;
    invite_code = "";
    
    return temp;
}

+(std::string)loadCodeNew
{
    string temp = web_codeNew;
    web_codeNew = "";
    
    return temp;
}

+(std::string)loadCode{
    
    string temp = web_code;
    web_code = "";
    
    return temp;
}

+(void)callPhone:(std::string)phone{
    NSString *telNumber = [NSString stringWithFormat:@"tel:%s", phone.c_str()];
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:telNumber]];
}

- (NSDictionary*)dictionaryFromQuery:(NSString*)query usingEncoding:(NSStringEncoding)encoding {
    
    NSCharacterSet* delimiterSet = [NSCharacterSet characterSetWithCharactersInString:@"&"];
    NSMutableDictionary* pairs = [NSMutableDictionary dictionary];
    NSScanner* scanner = [[NSScanner alloc] initWithString:query];
    while (![scanner isAtEnd]) {
        NSString* pairString = nil;
        [scanner scanUpToCharactersFromSet:delimiterSet intoString:&pairString];
        [scanner scanCharactersFromSet:delimiterSet intoString:NULL];
        NSArray* kvPair = [pairString componentsSeparatedByString:@"="];
        if (kvPair.count == 2) {
            NSString* key = [[kvPair objectAtIndex:0]
                             stringByReplacingPercentEscapesUsingEncoding:encoding];
            NSString* value = [[kvPair objectAtIndex:1]
                               stringByReplacingPercentEscapesUsingEncoding:encoding];
            [pairs setObject:value forKey:key];
        }
    }
    
    return [NSDictionary dictionaryWithDictionary:pairs];
}

- (BOOL)application:(UIApplication *)application continueUserActivity:(NSUserActivity *)userActivity restorationHandler:(void (^)(NSArray *restorableObjects))restorationHandler
{
    //ios 拉起控制
    if ([userActivity.activityType isEqualToString:NSUserActivityTypeBrowsingWeb ])
    {
    
        NSURL *webUrl = userActivity.webpageURL;
        
        if ([webUrl.host isEqualToString:@"wbsl.boxwan.cn"])
        {
            NSString* canShu = [webUrl query];
            if ([canShu  containsString:@"webCode"])
            {
                //1:游戏邀请拉起
                NSRange deng = [canShu rangeOfString:@"="];
                NSString *code = [canShu substringFromIndex:deng.location+1];
                web_codeNew = [code UTF8String];
                MyNotification::getInstance()->postNotify("game_reload_new");
                
            }else if([canShu  containsString:@"inviteCode"])
            {
                //2:推荐好友拉起
                NSRange deng = [canShu rangeOfString:@"="];
                NSString *code = [canShu substringFromIndex:deng.location+1];
                invite_code = [code UTF8String];
                MyNotification::getInstance()->postNotify("invite_load_Inter");
            }
            
        }else
        {
            //不能识别 safrai打开
            [[UIApplication sharedApplication] openURL:webUrl];
        }
    }
    
    return YES;
}

-(void) getBatteryInfo;
{
    int level = [UIDevice currentDevice].batteryLevel * 100.0;
    
    dianLiangValue = level;
    
}

-(void) didLoad;
{
    [[UIDevice currentDevice] setBatteryMonitoringEnabled:YES];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(getBatteryInfo) name:UIDeviceBatteryStateDidChangeNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(getBatteryInfo) name:UIDeviceBatteryLevelDidChangeNotification object:nil];
    [NSTimer scheduledTimerWithTimeInterval:5.0f target:self selector:@selector(getBatteryInfo) userInfo:nil repeats:YES];
    
    //[NSTimer scheduledTimerWithTimeInterval:5.0f target:self selector:@selector(getSignalStrength) userInfo:nil repeats:YES];
    [self getSignalStrength];
}

-(void)getSignalStrength
{
    NSArray *subviews = [[[[UIApplication sharedApplication] valueForKey:@"statusBar"] valueForKey:@"foregroundView"]subviews];
    
    NSNumber *dataNetworkItemView = nil;
    NSString *dataNetworkItemView2 = nil;
    
    for (id subview in subviews) {
        if([subview isKindOfClass:[NSClassFromString(@"UIStatusBarDataNetworkItemView") class]]) {
            dataNetworkItemView = subview;
            dataNetworkItemView2 = subview;
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
            NSLog(@"WIFI");
            netString = "WIFI";
            break;
    }
    
    string signalStrength;
    
    if(netString == "WIFI")
    {
        int signalV = [[dataNetworkItemView2 valueForKey:@"_wifiStrengthBars"] intValue];
        if(signalV == 1)
        {
            signalStrength = "20";
            
        }else if (signalV == 2)
        {
            signalStrength = "50";
            
        }else
        {
            signalStrength = "100";
        }
        
    }else if(netString == "2G")
    {
        signalStrength = "100";
        
    }else if(netString == "3G")
    {
        signalStrength = "100";
        
    }else if(netString == "4G")
    {
        signalStrength = "100";
    }else
    {
        signalStrength = "0";
    }

    
    std::map<std::string, std::string> mapT;
    mapT["level"] = signalStrength;
    mapT["netType"] = netString;

    signalValue = mapT;
    
    printf("AAA == %s %s\n", signalStrength.c_str(), netString.c_str());
    
}

+(int)getDianLiangValue
{
    return  dianLiangValue;
}

+(std::map<std::string, std::string> ) getSignalValue
{
    return signalValue;
}

#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
     */
}


- (void)dealloc {
    [window release];
    [super dealloc];
}

@end
