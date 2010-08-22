

#include "iOSEnvironment.h"
#include <Meta/iOS.h>
#import <Foundation/Foundation.h>


namespace OpenEngine {
namespace Display {
    
    using namespace Devices;

iOSEnvironment::iOSEnvironment() {
    
}

IFrame& iOSEnvironment::CreateFrame() {
    frame = new iOSFrame();
    touch = new iOSTouch();
    return *frame;
}

    Devices::IMouse* iOSEnvironment::GetMouse() { THROW(); }
    Devices::IKeyboard* iOSEnvironment::GetKeyboard() { THROW(); }
    Devices::IJoystick* iOSEnvironment::GetJoystick() { THROW(); }
    Devices::iOSTouch* iOSEnvironment::GetTouch() {
        return touch;
    }

    void iOSEnvironment::Handle(Core::ProcessEventArg arg) { 
        frame->Handle(arg);
    }
    void iOSEnvironment::Handle(Core::InitializeEventArg arg) { 
        frame->Handle(arg);
        frame->SetTouch(touch);
    }
    void iOSEnvironment::Handle(Core::DeinitializeEventArg arg) { 
        frame->Handle(arg);
    }

    string iOSEnvironment::GetResourcePath() {
        NSString *path = [[[NSBundle mainBundle] resourcePath] stringByAppendingString:@"/"];
        const char *cpath = [path UTF8String];
        return string(cpath);
    }

}
}
