
#import "OEViewController.h"
#import "OEView.h"

@implementation OEViewController

- (void)loadView {
    NSLog(@"Time to load the view!");
    self.view = [[OEView alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
}

@end
