#include <Core/iOSEngine.h>

#import "OEDelegate.h"


namespace OpenEngine {
namespace Core {

iOSEngine::iOSEngine() {
    pool = [[NSAutoreleasePool alloc] init];
}
    
void iOSEngine::Start() {
    
    _iOSEngine = this;

    //TickEngine::Start();

    int argc = 0;
    char *argv[] = {};        

    OEDelegate *d = [[OEDelegate alloc ] init];

    
    UIApplicationMain(argc,argv,nil,@"OEDelegate");
    [pool release];    
}
    
void iOSEngine::DoStart() {
    TickEngine::Start();
}

 iOSEngine* _iOSEngine;

}
}

