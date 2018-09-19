//
//  LocationGetIOS.m
//  ZJH
//
//  Created by apple on 16/7/21.
//
//
/*集成说明：
 
 1、在plist添加
 NSLocationAlwaysUsageDescription ＝ YES
 NSLocationWhenInUseUsageDescription ＝ YES
 2、导入CCLocationManager.h头文件
 */

#define IS_IOS7 ([[[UIDevice currentDevice] systemVersion] floatValue] >= 7)
#define IS_IOS8 ([[[UIDevice currentDevice] systemVersion] floatValue] >= 8)

#import "LocationGetIOS.h"

@interface LocationGetIOS()<CLLocationManagerDelegate>

@end

static LocationGetIOS*location = NULL;

@implementation LocationGetIOS

+(id) shareLocationGetIOS;
{
    if (location == NULL)
    {
        location = [[LocationGetIOS alloc]init];
    }
    return location;
    
}
-(id) init;
{
    if ((self = [super init]))
    {
        if (IS_IOS8)
        {
            [UIApplication sharedApplication].idleTimerDisabled = TRUE;
            self.locationManager = [[CLLocationManager alloc] init];
            //[self.locationManager  requestAlwaysAuthorization ];        //NSLocationAlwaysUsageDescription
            [self.locationManager requestWhenInUseAuthorization];     //NSLocationWhenInUseDescription
            self.locationManager.delegate = self;
        }
        

        // 设置定位精度
        // kCLLocationAccuracyNearestTenMeters:精度10米
        // kCLLocationAccuracyHundredMeters:精度100 米
        // kCLLocationAccuracyKilometer:精度1000 米
        // kCLLocationAccuracyThreeKilometers:精度3000米
        // kCLLocationAccuracyBest:设备使用电池供电时候最高的精度
        // kCLLocationAccuracyBestForNavigation:导航情况下最高精度，一般要有外接电源时才能使用
        self.locationManager.desiredAccuracy = kCLLocationAccuracyHundredMeters;
        
        // distanceFilter是距离过滤器，为了减少对定位装置的轮询次数，位置的改变不会每次都去通知委托，而是在移动了足够的距离时才通知委托程序
        // 它的单位是米，这里设置为至少移动100再通知委托处理更新;
        self.locationManager.distanceFilter = 100.0f; // 如果设为kCLDistanceFilterNone，则每秒更新一次;
        
        // 开启定位:设置 > 隐私 > 位置 > 定位服务
        if ([CLLocationManager locationServicesEnabled]) {
            // 启动位置更新
            // 开启位置更新需要与服务器进行轮询所以会比较耗电，在不需要时用stopUpdatingLocation方法关闭;
            [self.locationManager startUpdatingLocation];
        }
        else {
            //NSLog(@"请开启定位功能！");
        }
    }
    
    return self;
}

// 地理位置发生改变时触发
- (void)locationManager:(CLLocationManager *)manager didUpdateToLocation:(CLLocation *)newLocation fromLocation:(CLLocation *)oldLocation
{
    // 获取经纬度
    NSLog(@"纬度ios: == %.2f 经度: == %.2f ", newLocation.coordinate.latitude, newLocation.coordinate.longitude);
   
    
    self.latitude = newLocation.coordinate.latitude;
    self.longitude = newLocation.coordinate.longitude;
    
    CLGeocoder *geocoder = [[CLGeocoder alloc] init];
    //根据经纬度反向地理编译出地址信息
    [geocoder reverseGeocodeLocation:newLocation completionHandler:^(NSArray *array, NSError *error)
     {
         if (array.count > 0)
         {
             CLPlacemark *placemark = [array objectAtIndex:0];
             //将获得的所有信息显示到label上
             //获取城市
             self.provice = [placemark.administrativeArea UTF8String];
             self.city = [placemark.locality UTF8String];
             self.qu = [placemark.subLocality UTF8String];
             printf("city location＝＝ %s\n %s\n %s\n", self.provice.c_str(), self.city.c_str(), self.qu.c_str() );
            }
     }];

    // 停止位置更新
    [manager stopUpdatingLocation];
}

// 定位失误时触发
- (void)locationManager:(CLLocationManager *)manager didFailWithError:(NSError *)error
{
    NSLog(@"error:%@",error);
}


/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
