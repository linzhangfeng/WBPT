//
//  LocationGetIOS.h
//  ZJH
//
//  Created by apple on 16/7/21.
//
//

#import <UIKit/UIKit.h>
#import <CoreLocation/CoreLocation.h>

@interface LocationGetIOS : UIViewController

@property(nonatomic, strong) CLLocationManager *locationManager;

@property std::string provice;
@property std::string city;
@property std::string qu;

@property double longitude;
@property double latitude;


+(id) shareLocationGetIOS;

@end
