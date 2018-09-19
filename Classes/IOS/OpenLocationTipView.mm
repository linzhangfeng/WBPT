//
//  OpenLocationTipView.m
//  ZJH
//
//  Created by apple on 2016/12/14.
//
//

#import "OpenLocationTipView.h"

@implementation OpenLocationTipView

static OpenLocationTipView *tipView = NULL;

+(id)shareOpenTipView
{
    if (tipView == NULL)
    {
        tipView = [[OpenLocationTipView alloc]init];
    }
    return tipView;
}

-(void) showView;
{
    
    UIAlertController *alert = [UIAlertController alertControllerWithTitle:@"" message:@"请在设置-隐私-定位服务中开启定位功能" preferredStyle:UIAlertControllerStyleAlert];
    
    [alert addAction:[UIAlertAction actionWithTitle:@"关闭" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action){
        
            [self.view removeFromSuperview];
            
    }]];
    
    [self presentViewController:alert animated:YES completion:nil];

}

-(void) removeAlertViewController
{
    [self dismissViewControllerAnimated:NO completion:NULL];
}
@end
