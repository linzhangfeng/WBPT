#import <UIKit/UIKit.h>

@class RootViewController;

@class RootViewControllerV;


@interface AppController : NSObject <UIApplicationDelegate> {
    UIWindow *window;
}

+(void)changeRootViewControllerH;//静态方法(修改屏幕为横屏)
+(void)changeRootViewControllerV;//静态方法(修改屏幕为竖屏)

+(std::string)loadCode;

+(std::string)loadCodeNew;

//推荐好友的拉起
+(std::string)loadInviteCode;

-(void) getBatteryInfo;

-(void)getSignalStrength;

-(void) didLoad;

+(int)getDianLiangValue;
+(std::map<std::string, std::string> ) getSignalValue;

+(void)callPhone:(std::string) phone;

@property(nonatomic, readonly) RootViewController* viewController;
@property(nonatomic, readonly) RootViewControllerV* viewControllerV;

@end


