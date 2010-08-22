#include "iOSFrame.h"

#include "OEView.h"

#include <UIKit/UIKit.h>
#include <Logging/Logger.h>

namespace OpenEngine {
namespace Display {

using Core::Exception;
    
iOSFrame::iOSFrame() : fc(FrameCanvas(*this)) {
}


    bool iOSFrame::IsFocused() const { THROW(); }

    unsigned int iOSFrame::GetWidth() const { 
        CGRect b = ((OEView*)view).bounds;
        return b.size.width;
    }
    unsigned int iOSFrame::GetHeight() const { 
        CGRect b = ((OEView*)view).bounds;
        return b.size.height;
    }
    unsigned int iOSFrame::GetDepth() const { THROW(); }

    FrameOption iOSFrame::GetOptions() const { THROW(); }
    bool iOSFrame::GetOption(const FrameOption option) const { THROW(); }

    void iOSFrame::SetWidth(const unsigned int width) { THROW(); }
    void iOSFrame::SetHeight(const unsigned int height) { THROW(); }
    void iOSFrame::SetDepth(const unsigned int depth) { THROW(); }
    void iOSFrame::SetOptions(const FrameOption options) { THROW(); }
    void iOSFrame::ToggleOption(const FrameOption option) { THROW(); }

    void iOSFrame::Handle(Core::InitializeEventArg arg) { 
        CGRect frame = [[UIScreen mainScreen] bounds];
        UIWindow *window = [[UIWindow alloc] initWithFrame:frame];
        [window setBackgroundColor:[UIColor whiteColor]];
        [window makeKeyAndVisible];
        view = [[OEView alloc] initWithFrame:frame];
        [window addSubview:view];
        
        //NSLog(@"frame window: %@",[UIApplication sharedApplication].keyWindow);
        
        //[application.keyWindow addSubview:self.viewController.view];
        [view setFramebuffer];
        ((IListener<Display::InitializeEventArg>*)canvas)->Handle(Display::InitializeEventArg(fc));
    }
    void iOSFrame::Handle(Core::ProcessEventArg arg) { 
        OEView *v = view;
        [v setFramebuffer];
        ((IListener<Display::ProcessEventArg>*)canvas)->Handle(ProcessEventArg(fc, arg.start, arg.approx));
        if (![v presentFramebuffer])
            logger.error << "Failed" << logger.end;
        
    }
    void iOSFrame::Handle(Core::DeinitializeEventArg arg) { 
        ((IListener<Display::DeinitializeEventArg>*)canvas)->Handle(DeinitializeEventArg(fc));
    }

    void iOSFrame::SetTouch(iOSTouch *t) { [(OEView*)view setOETouch:t]; }

}
}
