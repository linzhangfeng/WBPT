//
//  ImagePicker.cpp
//  ZJH
//
//  Created by hyhy on 6/5/14.
//
//

#include "ImageHelper.h"
#include <AssetsLibrary/AssetsLibrary.h>
#include "ZJH.h"

#pragma mark [ ImageHelperImplIOS ]

@interface ImageHelperImplIOS : UIViewController<UIImagePickerControllerDelegate, UINavigationControllerDelegate>
{
    //
}
@end

@implementation ImageHelperImplIOS

-(id) init
{
    if ((self = [super init]))
    {
        
    }
    return self;
}

- (void) dealloc
{
    [self release];
    [super dealloc];
}

-(void) openCamera
{
    UIImagePickerController *picker = [[UIImagePickerController alloc] init];
    picker.allowsEditing = YES;
    picker.delegate = self;
    
    if ([UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera])
    {
        picker.sourceType = UIImagePickerControllerSourceTypeCamera;        
    }
    else
    {
        NSLog(@"模拟器无法打开相机");
    }
    [self presentViewController:picker animated:YES completion:nil];
}

-(void) openPhotoLibrary
{
    UIImagePickerController *picker = [[UIImagePickerController alloc] init];
    picker.allowsEditing = YES;
    picker.delegate = self;
    picker.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
    [self presentViewController:picker animated:YES completion:nil];
}

-(void) openSavedPhotosAlbum
{
    UIImagePickerController *picker = [[UIImagePickerController alloc] init];
    picker.allowsEditing = YES;
    picker.delegate = self;
    picker.sourceType = UIImagePickerControllerSourceTypeSavedPhotosAlbum;
    [self presentViewController:picker animated:YES completion:nil];
}

-(void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
    [self dismissViewControllerAnimated:YES completion:^{
        [self.view removeFromSuperview];
    }];
    
    NSString *mediaType = [info objectForKey:UIImagePickerControllerMediaType];
    
    NSData *data;
    
    if ([mediaType isEqualToString:@"public.image"]){
        
        //切忌不可直接使用originImage，因为这是没有经过格式化的图片数据，可能会导致选择的图片颠倒或是失真等现象的发生，从UIImagePickerControllerOriginalImage中的Origin可以看出，很原始
        //获取裁剪编辑后的图片
        //UIImage *originImage = [info objectForKey:UIImagePickerControllerOriginalImage];
        UIImage *originImage = [info objectForKey:UIImagePickerControllerEditedImage];
        CGSize imageSize =  [originImage size];
        //图片压缩，因为原图都是很大的，不必要传原图
        ///@todo test 先不裁剪
        
        UIImage *scaleImage = originImage;
        
        if (imageSize.width >= 1080)
        {
            scaleImage = [self scaleImage:originImage toScale:(720.0/imageSize.width)];
        }
        
        std::string postFix = "png";
        
        //以下这两步都是比较耗时的操作，最好开一个HUD提示用户，这样体验会好些，不至于阻塞界面
        data = UIImagePNGRepresentation(scaleImage); //将图片转换为PNG格式的二进制数据
        if (data == nil) {
            data = UIImageJPEGRepresentation(scaleImage, 1);  //将图片转换为JPG格式的二进制数据
            postFix = "jpg";
        }
        
        const char *buffer = (char *)[data bytes];
        char path[512];
        if (data.length) {
            snprintf(path, sizeof(path), "%simg%ld.%s", Utils::getTmpFoldPath().c_str(), time(NULL), postFix.c_str());
            CCHttpAgent::getInstance()->_makeMultiDir(path);
            FILE *fp = fopen(path, "wb+");
            if (fp) {
                fwrite(buffer, data.length, 1, fp);
                fclose(fp);
                
                __NotificationCenter::getInstance()->postNotification("image_notify", __String::create(path));
            }
        }
        
        ///@todo 在进游戏时删掉tmp文件夹里的内容
    }
    
}

-(UIImage *)scaleImage:(UIImage *)image toScale:(float)scaleSize
{
    UIGraphicsBeginImageContext(CGSizeMake(image.size.width*scaleSize,image.size.height*scaleSize));
    [image drawInRect:CGRectMake(0, 0, image.size.width * scaleSize, image.size.height *scaleSize)];
    UIImage *scaledImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    return scaledImage;
}


@end


#pragma mark [ ImageHelper ]
void ImageHelper::openCamera()
{
    UIWindow *window = [[UIApplication sharedApplication] keyWindow];
    ImageHelperImplIOS *impl = [[ImageHelperImplIOS alloc] init];
    [[window rootViewController] addChildViewController:impl];
    [impl openCamera];
}

void ImageHelper::openPhotoLibrary()
{
    UIWindow *window = [[UIApplication sharedApplication] keyWindow];
    ImageHelperImplIOS *impl = [[ImageHelperImplIOS alloc] init];
    [[window rootViewController] addChildViewController:impl];
    [impl openPhotoLibrary];
}

void ImageHelper::openSavedPhotosAlbum()
{
    UIWindow *window = [[UIApplication sharedApplication] keyWindow];
    ImageHelperImplIOS *impl = [[ImageHelperImplIOS alloc] init];
    [[window rootViewController] addChildViewController:impl];
    [impl openSavedPhotosAlbum];
}
