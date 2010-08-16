
#include <Meta/iOS.h>

#include "RenderCanvas.h"
#include <Renderers/IRenderer.h>



namespace OpenEngine {
namespace Display {
namespace OpenGLES2 {

    RenderCanvas::RenderCanvas() {  }

    void RenderCanvas::Handle(Display::InitializeEventArg arg) { 
        width = arg.canvas.GetWidth();
        height = arg.canvas.GetHeight();
        ((IListener<Renderers::InitializeEventArg>*)renderer)->Handle(Renderers::InitializeEventArg(*this));

    } 
    void RenderCanvas::Handle(Display::ProcessEventArg arg) {
        if (renderer == NULL) THROW();

        ((IListener<Renderers::ProcessEventArg>*)renderer)
        ->Handle(Renderers::ProcessEventArg(*this, arg.start, arg.approx));
        //CopyToTexture();
    }
    
    void RenderCanvas::Handle(DeinitializeEventArg arg) { 

        ((IListener<Renderers::DeinitializeEventArg>*)renderer)->Handle(Renderers::DeinitializeEventArg(*this));

    }
    void RenderCanvas::Handle(Display::ResizeEventArg arg) { THROW(); }

    unsigned int RenderCanvas::GetWidth() const { THROW(); }
    unsigned int RenderCanvas::GetHeight() const { THROW(); }
    void RenderCanvas::SetWidth(const unsigned int width) { THROW(); }
    void RenderCanvas::SetHeight(const unsigned int height) { THROW(); }
    ITexture2DPtr RenderCanvas::GetTexture() { THROW(); }


}
}
}
