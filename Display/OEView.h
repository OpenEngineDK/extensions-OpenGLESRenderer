#import <UIKit/UIKit.h>

#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#include <Devices/iOSTouch.h>

using namespace OpenEngine::Devices;

@interface OEView : UIView {
    EAGLContext *context;
    // The pixel dimensions of the CAEAGLLayer.
    GLint framebufferWidth;
    GLint framebufferHeight;
    
    // The OpenGL ES names for the framebuffer and renderbuffer used to render to this view.
    GLuint defaultFramebuffer, colorRenderbuffer;
    GLuint depthRenderbuffer;

    iOSTouch *oeTouch;
}

- (void)createFramebuffer;
- (void)setFramebuffer;
- (BOOL)presentFramebuffer;
- (void)setOETouch:(iOSTouch *)t;

@end

