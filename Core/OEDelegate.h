#import <UIkit/UIKit.h>

#import <Display/OEViewController.h>

@interface OEDelegate : NSObject <UIApplicationDelegate> {

    UIWindow *window;
    OEViewController *viewController;

}

@property (nonatomic, retain) OEViewController *viewController;

@end
