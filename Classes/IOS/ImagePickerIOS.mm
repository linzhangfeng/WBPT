//
//  ImagePickerIOS.mm
//  ZJH
//
//  Created by hyhy on 6/5/14.
//
//

#include "ImagePickerIOS.h"
#import <AssetsLibrary/AssetsLibrary.h>

static UIView *landscapeView = nil;

@implementation ImagePickerIOS

@synthesize imagePickerDelegate = _imagePickerDelegate;

+(ImagePickerIOS*) showImagePicker:(float) winWidth
{
    UIWindow *window = [[UIApplication sharedApplication] keyWindow];
    ImagePickerIOS *viewController = [[ImagePickerIOS alloc] init];
    //[window addSubview:viewController.view];
    [[window rootViewController] addChildViewController:viewController];
    
    [viewController chooseImageSour: winWidth];
    
    return viewController;
}

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

/*弹出选择图片来源的对话框*/
- (void) chooseImageSour:(float) winWidth
{
    
    
    UIActionSheet *chooseImageSheet = [[UIActionSheet alloc] initWithTitle:nil delegate:self cancelButtonTitle:@"取消" destructiveButtonTitle:nil otherButtonTitles:@"拍照",@"选择本地图片", nil];
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad) {
        [chooseImageSheet showInView:self.view];
    } else {
        //iphone需要手动改变UIActionSheet的方向
        UIInterfaceOrientation orientation = [[UIApplication sharedApplication] statusBarOrientation];
        int rotateNum = (orientation == UIInterfaceOrientationLandscapeLeft) ? -1 : 1;
        if (landscapeView) {
            [landscapeView removeFromSuperview];
        }
        //landscapeView = [[UIView alloc] initWithFrame:CGRectMake(-80, 80, 480, 480)];
        landscapeView = [[UIView alloc] initWithFrame:CGRectMake(-(winWidth - 320)/2.0, (winWidth - 320)/2.0, winWidth, 320)];
        landscapeView.transform = CGAffineTransformMakeRotation((rotateNum)*M_PI/2);
        //landscapeView.bounds = CGRectMake(0, 0, 480, 480);
        [[[UIApplication sharedApplication] keyWindow] addSubview:landscapeView];
        [chooseImageSheet showInView:landscapeView];
    }    
}

#pragma mark UIActionSheetDelegate Method
- (void)actionSheet:(UIActionSheet *)actionSheet clickedButtonAtIndex:(NSInteger)buttonIndex
{
    if (landscapeView) {
        [landscapeView removeFromSuperview];
    }
    switch (buttonIndex)
    {
        case 0://Take picture
            [self openImagePicker:0];
            break;
        case 1://From album
        {
            if ([ALAssetsLibrary authorizationStatus] == ALAuthorizationStatusNotDetermined) {
                
                ALAssetsLibrary *assetsLibrary = [[ALAssetsLibrary alloc] init];
                
                [assetsLibrary enumerateGroupsWithTypes:ALAssetsGroupAll usingBlock:^(ALAssetsGroup *group, BOOL *stop) {
                    
                    if (*stop) {
                        //点击“好”回调方法:
                        NSLog(@"好");
                        return;
                        
                    }
                    *stop = TRUE;
                    
                } failureBlock:^(NSError *error) {
                    
                    //点击“不允许”回调方法:
                    NSLog(@"不允许");
                    [self dismissViewControllerAnimated:YES completion:nil];
                    
                }];
            }
            
            [self openImagePicker:1];
            
            break;
        }
        default:
            break;
    }
    
    [self.view removeFromSuperview];
}

-(void) openImagePicker:(int)action
{
    UIImagePickerController * picker = [[UIImagePickerController alloc] init];
    picker.allowsEditing = YES; //获取图片后进入编辑裁剪模式
    picker.delegate = self;
    
    switch (action)
    {
        case 0:
            if ([UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera])
            {
                picker.sourceType = UIImagePickerControllerSourceTypeCamera;
                
            }
            else
            {
                NSLog(@"模拟器无法打开相机");
            }
            
            [self presentViewController:picker animated:YES completion:nil];
            break;
        case 1:
            picker.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
            if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad)
            {
                picker.contentSizeForViewInPopover = CGSizeMake(50, 460);
                [_popOver setPopoverContentSize:CGSizeMake(50, 460)];
                _popOver = [[UIPopoverController alloc] initWithContentViewController:picker];
               // [_popOver setPopoverContentSize:CGSizeMake(1000, 1000)];
                [_popOver presentPopoverFromRect:CGRectMake(0, 512, 50, 50) inView:self.view permittedArrowDirections:UIPopoverArrowDirectionUp animated:YES];
                _popOver.delegate = self;
            }
            else
            {
                [self presentViewController:picker animated:YES completion:nil];
            }
            break;
    }
}

#pragma 拍照选择照片协议方法
- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
    if(_popOver) //如有popOver，则移除
    {
        [_popOver dismissPopoverAnimated:YES];
    }
    //在回调中移除自身
    [self dismissViewControllerAnimated:YES completion:^{
        [self.view removeFromSuperview];
    }];
    
    NSString *mediaType = [info objectForKey:UIImagePickerControllerMediaType];
    
    NSData *data;
    
    if ([mediaType isEqualToString:@"public.image"]){
        
        //切忌不可直接使用originImage，因为这是没有经过格式化的图片数据，可能会导致选择的图片颠倒或是失真等现象的发生，从UIImagePickerControllerOriginalImage中的Origin可以看出，很原始，哈哈
        //获取裁剪编辑后的图片
        //UIImage *originImage = [info objectForKey:UIImagePickerControllerOriginalImage];
        UIImage *originImage = [info objectForKey:UIImagePickerControllerEditedImage];
        CGSize imageSize =  [originImage size];
        //图片压缩，因为原图都是很大的，不必要传原图
        UIImage *scaleImage = [self scaleImage:originImage toScale:(128.0/imageSize.width)];
        
        //以下这两步都是比较耗时的操作，最好开一个HUD提示用户，这样体验会好些，不至于阻塞界面
        data = UIImagePNGRepresentation(scaleImage); //将图片转换为PNG格式的二进制数据
        if (data == nil) {
            data = UIImageJPEGRepresentation(scaleImage, 1);  //将图片转换为JPG格式的二进制数据
        }
        
        //将二进制数据生成UIImage
        //    UIImage *image = [UIImage imageWithData:data];
        
        _imagePickerDelegate->onImageData( (char*)[data bytes], [data length]);
    }
    
}

- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker
{
    if(!_popOver);
//        [self dismissViewControllerAnimated:YES completion:^{
//            [self.view removeFromSuperview];
//        }];
    else
        [_popOver dismissPopoverAnimated:YES];
    [self dismissViewControllerAnimated:YES completion:^{
        [self.view removeFromSuperview];
    }];
}

/*回传过来的图片数据*/
-(void)onImageData:(UIImage *)image
{
    
}

#pragma mark- 缩放图片
-(UIImage *)scaleImage:(UIImage *)image toScale:(float)scaleSize
{
    UIGraphicsBeginImageContext(CGSizeMake(image.size.width*scaleSize,image.size.height*scaleSize));
    [image drawInRect:CGRectMake(0, 0, image.size.width * scaleSize, image.size.height *scaleSize)];
    UIImage *scaledImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    return scaledImage;
}

@end
