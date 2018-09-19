//
//  UpdateTipView.h
//  ZJH
//
//  Created by apple on 2016/12/14.
//
//

#import <UIKit/UIKit.h>

@interface UpdateTipView : UIViewController

-(void) showView:(int) force Desc:(NSString *) desc VerName:(NSString*) version URL:(NSString *) url;

+(id)shareUpdateTipView;

-(void) removeAlertViewController;

@end
