
#import "OEDelegate.h"

#include "iOSEngine.h"
#include <Logging/Logger.h>

using namespace OpenEngine::Core;

@implementation OEDelegate

- (BOOL)application:(UIApplication *)application
didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    self.viewController = [[OEViewController alloc] init];

    
//    window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
//    [window setBackgroundColor:[UIColor whiteColor]];
//
//    [window makeKeyAndVisible];
//
//    [application.keyWindow addSubview:self.viewController.view];
    
    NSTimer *timer = [NSTimer scheduledTimerWithTimeInterval:0.1 
                                                      target:self
                                                    selector:@selector(tick) 
                                                    userInfo:nil
                                                     repeats:YES];
    [timer retain];
    
    // initialize
    _iOSEngine->DoStart();

    NSLog(@"[] %@",[[NSBundle mainBundle] pathForResource:@"test" ofType:@"glsl"]);
    
    //NSLog(@"Start ticking!");
    
    return YES;
}


- (void)tick {
    _iOSEngine->Tick();
}

@synthesize viewController;

@end
