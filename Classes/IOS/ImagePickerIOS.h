//
//  ImagePickerIOS.h
//  ZJH
//
//  Created by hyhy on 6/5/14.
//
//

#ifndef __ZJH__ImagePickerIOS__
#define __ZJH__ImagePickerIOS__

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import "ImagePicker.h"
#include "cocos2d.h"

@protocol ImagePickerDelegateIOS <NSObject>

-(void)onImageData:(UIImage *)image;

@end

@interface ImagePickerIOS : UIViewController<UIActionSheetDelegate, UIImagePickerControllerDelegate, UINavigationControllerDelegate, UIPopoverControllerDelegate, ImagePickerDelegateIOS>
{
    ImagePickerDelegate * _imagePickerDelegate;
    UIPopoverController * _popOver;
}

@property(nonatomic, assign) ImagePickerDelegate * imagePickerDelegate;

+(ImagePickerIOS *) showImagePicker:(float) winWidth;

-(void)chooseImageSour:(float) winWidth;

-(void)openImagePicker:(int) action;

@end



#endif /* defined(__ZJH__ImagePickerIOS__) */
