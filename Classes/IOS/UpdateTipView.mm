//
//  UpdateTipView.m
//  ZJH
//
//  Created by apple on 2016/12/14.
//
//

#import "UpdateTipView.h"

@implementation UpdateTipView

static UpdateTipView *tipView = NULL;

+(id)shareUpdateTipView
{
    if (tipView == NULL)
    {
        tipView = [[UpdateTipView alloc]init];
    }
    return tipView;
}

-(void) showView:(int) force Desc:(NSString *) desc VerName:(NSString*) version URL:(NSString *) url
{
    
    UIAlertController *alert = [UIAlertController alertControllerWithTitle:[NSString stringWithFormat:@"发现新版本：version(%@)", version] message:desc preferredStyle:UIAlertControllerStyleAlert];
    
    UIView *subView1 = alert.view.subviews[0];
    UIView *subView2 = subView1.subviews[0];
    UIView *subView3 = subView2.subviews[0];
    UIView *subView4 = subView3.subviews[0];
    UIView *subView5 = subView4.subviews[0];
    
    //UILabel *title = subView5.subviews[0];
    UILabel *message = subView5.subviews[1];
    
    message.textAlignment = NSTextAlignmentLeft;
    //0:强制更新 1:不强制
    if(force == 0)
    {
        [alert addAction:[UIAlertAction actionWithTitle:@"立即更新" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action){
            [[UIApplication sharedApplication] openURL:[NSURL URLWithString:url]];
            
            //退出游戏
            [self presentViewController:alert animated:YES completion:nil];
            
        }]];

        [alert addAction:[UIAlertAction actionWithTitle:@"退出游戏" style:UIAlertActionStyleDestructive handler:^(UIAlertAction * _Nonnull action){
            
            //退出游戏
            cocos2d::Director::getInstance()->end();
            exit(0);
        }]];
        
        
    }else
    {
    
        [alert addAction:[UIAlertAction actionWithTitle:@"立即更新" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action){
            
            cocos2d::__NotificationCenter::getInstance()->postNotification("notForceUpdateNext");
            [self.view removeFromSuperview];
            
            [[UIApplication sharedApplication] openURL:[NSURL URLWithString:url]];
            
        }]];
        
        [alert addAction:[UIAlertAction actionWithTitle:@"稍后更新" style:UIAlertActionStyleCancel handler:^(UIAlertAction * _Nonnull action){
            
            cocos2d::__NotificationCenter::getInstance()->postNotification("notForceUpdateNext");

            [self.view removeFromSuperview];
        }]];
    }
    
    [self presentViewController:alert animated:YES completion:nil];

}

-(void) removeAlertViewController
{
    [self dismissViewControllerAnimated:NO completion:NULL];
}
@end
