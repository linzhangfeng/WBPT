//
//  BaiduMapManager.h
//  ZJH
//
//  Created by 黄宇 on 16/10/13.
//
//
//#import <Foundation/Foundation.h>
#import <BaiduMapAPI_Location/BMKLocationService.h>
#import <BaiduMapAPI_Search/BMKGeocodeSearchOption.h>
#import <BaiduMapAPI_Search/BMKGeocodeSearch.h>

typedef void (^CoordinateBlock)(CLLocationCoordinate2D);
typedef void (^AddressBlock)(BMKAddressComponent *);

@interface BaiduGeoCode : NSObject {
    CoordinateBlock tempcoordinateblock;
    AddressBlock tempaddressblock;
}
@property (nonatomic,strong) BMKGeoCodeSearch *geoCodeSearch;
//@property (copy) CoordinateBlock tempcoordinateblock;
//@property (copy) AddressBlock tempaddressblock;

-(id)init;
-(void)getCoordinateByAddress:(NSString*)city address:(NSString *)address block:(CoordinateBlock)block;
-(void)getAddressByCoordinate:(float)latitude longitude:(float)longitude block:(AddressBlock)block;
@end

@interface BaiduMapManager : NSObject
@property std::string city;
@property std::string province;
@property std::string district;
@property std::string streetName;
@property std::string streetNumber;

@property (nonatomic,assign) float latitude;
@property (nonatomic,assign) float longitude;

+(id) shareManager;
-(id) init;
-(void)startUpdateLocation;
-(void)stopUpdateLocation;
-(float)getDistanceByStart:(CLLocationCoordinate2D)start end:(CLLocationCoordinate2D)end;
-(void)getCoordinateByAddress:(NSString*)city address:(NSString *)address block:(CoordinateBlock)block;
-(void)getAddressByCoordinate:(float)latitude longitude:(float)longitude block:(AddressBlock)block;
@end
