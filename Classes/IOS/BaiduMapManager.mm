//
//  BaiduMapManager.m
//  ZJH
//
//  Created by 黄宇 on 16/10/13.
//
//

#import "BaiduMapManager.h"
#include <BaiduMapAPI_Utils/BMKGeometry.h>

@interface BaiduGeoCode()<BMKGeoCodeSearchDelegate>
@end
@implementation BaiduGeoCode
-(id)init {
    if (self = [super init]) {
        _geoCodeSearch = [[BMKGeoCodeSearch alloc]init];
        _geoCodeSearch.delegate = self;
    }
    return self;
}
-(void)getCoordinateByAddress:(NSString*)city address:(NSString *)address  block:(CoordinateBlock)block{
    tempcoordinateblock = Block_copy(block);
    Block_release(block);
    
    BMKGeoCodeSearchOption *op = [[BMKGeoCodeSearchOption alloc]init];
    op.address = address;
    op.city = city;
    if (![_geoCodeSearch geoCode:op]) {
        NSLog(@"reverseGeoCode failed");
    }
}

- (void)onGetGeoCodeResult:(BMKGeoCodeSearch *)searcher result:(BMKGeoCodeResult *)result errorCode:(BMKSearchErrorCode)error {
    if (error == BMK_SEARCH_NO_ERROR) {
        if (tempcoordinateblock) {
            tempcoordinateblock(result.location);
        }
    }
}

-(void)getAddressByCoordinate:(float)latitude longitude:(float)longitude block:(AddressBlock)block{
    BMKReverseGeoCodeOption *op = [[BMKReverseGeoCodeOption alloc]init];
    CLLocationCoordinate2D coordinate = CLLocationCoordinate2DMake(latitude, longitude);
    op.reverseGeoPoint = coordinate;
    if (![_geoCodeSearch reverseGeoCode:op]) {
        NSLog(@"GeoCode failed");
    }
    tempaddressblock = Block_copy(block);
    Block_release(block);
}

- (void)onGetReverseGeoCodeResult:(BMKGeoCodeSearch *)searcher result:(BMKReverseGeoCodeResult *)result errorCode:(BMKSearchErrorCode)error {
    if (error == BMK_SEARCH_NO_ERROR) {
        if (tempaddressblock) {
            tempaddressblock(result.addressDetail);
        }
    }
}
//- (void)aaaa:(CoordinateBlock)block {
//    tempcoordinateblock = block;
//}
//-(void) bbb {
//    tempcoordinateblock(CLLocationCoordinate2DMake(11.11, 12.33));
//}
@end

@interface BaiduMapManager()<BMKLocationServiceDelegate> {
    BMKMapManager *_mapManager;
    BMKLocationService *_locationServer;
}

@end

static BaiduMapManager *_instance = NULL;
@implementation BaiduMapManager

+(id) shareManager {
    if (_instance == NULL) {
        _instance = [[BaiduMapManager alloc]init];
    }
    return _instance;
}

-(id) init {
    if (self = [super init]) {
        _mapManager = [[BMKMapManager alloc]init];
        
        BOOL ret = [_mapManager start:@"qQ0nQN6yr4FhQXm1PzDLnGtBrDd0ETEu" generalDelegate:nil];
        if (!ret) {
            NSLog(@"baidu mapmanager start failed!");
        }
        
        [self startUpdateLocation];
    }
    return self;
}

- (void)didUpdateBMKUserLocation:(BMKUserLocation *)userLocation {
    _latitude = userLocation.location.coordinate.latitude;
    _longitude = userLocation.location.coordinate.longitude;
    
    __block BaiduMapManager *manager = self;
    [self getAddressByCoordinate:_latitude longitude:_longitude block:^(BMKAddressComponent* addressDetail) {
        manager.streetName = [addressDetail.streetName cStringUsingEncoding:NSUTF8StringEncoding];
        manager.streetNumber = [addressDetail.streetNumber cStringUsingEncoding:NSUTF8StringEncoding];
        manager.province = [addressDetail.province cStringUsingEncoding:NSUTF8StringEncoding];
        manager.city = [addressDetail.city cStringUsingEncoding:NSUTF8StringEncoding];
        manager.district = [addressDetail.district cStringUsingEncoding:NSUTF8StringEncoding];
    }];
}

-(void)startUpdateLocation {
    if (_locationServer == NULL) {
        _locationServer = [[BMKLocationService alloc]init];
        _locationServer.desiredAccuracy = kCLLocationAccuracyBest;
        _locationServer.distanceFilter = 100.0f;
        _locationServer.delegate = self;
    }
    
    [_locationServer startUserLocationService];
}

-(void)stopUpdateLocation {
    if (_locationServer) {
        [_locationServer stopUserLocationService];
    }
    _locationServer = NULL;
}

-(float)getDistanceByStart:(CLLocationCoordinate2D)start end:(CLLocationCoordinate2D)end {
    BMKMapPoint point1 = BMKMapPointForCoordinate(start);
    BMKMapPoint point2 = BMKMapPointForCoordinate(end);
    CLLocationDistance distance = BMKMetersBetweenMapPoints(point1,point2);
    return distance;
}

-(void)getCoordinateByAddress:(NSString*)city address:(NSString *)address block:(CoordinateBlock)block {
    BaiduGeoCode *geoCode = [[BaiduGeoCode alloc]init];
    [geoCode getCoordinateByAddress:city address:address block:block];
}

-(void)getAddressByCoordinate:(float)latitude longitude:(float)longitude block:(AddressBlock)block {
    BaiduGeoCode *geoCode = [[BaiduGeoCode alloc]init];
    [geoCode getAddressByCoordinate:latitude longitude:longitude block:block];
}
@end
