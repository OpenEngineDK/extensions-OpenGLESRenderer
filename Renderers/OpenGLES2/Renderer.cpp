#include "Renderer.h"

#include <Meta/iOS.h>
#include <Meta/OpenGLES2.h>
#include <Logging/Logger.h>
#include <Display/IViewingVolume.h>

namespace OpenEngine {
namespace Renderers {
namespace OpenGLES2 {

using namespace Display;

    Renderer::Renderer() : backgroundColor(Vector<4,float>(1.0f)) { }

    void Renderer::Handle(Renderers::InitializeEventArg arg) { 
        this->stage = RENDERER_INITIALIZE;
        this->initialize.Notify(RenderingEventArg(arg.canvas, *this));
        this->stage = RENDERER_PREPROCESS;
        
    }
    void Renderer::Handle(Renderers::DeinitializeEventArg arg) { 
        // stuff...
    }
    void Renderer::Handle(Renderers::ProcessEventArg arg) { 
        Vector<4,float> bgc = backgroundColor;
        glClearColor(bgc[0], bgc[1], bgc[2], bgc[3]);
        
        // Clear the screen and the depth buffer.
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
            
        IViewingVolume* volume = arg.canvas.GetViewingVolume();
        // If no viewing volume is set for the viewport ignore it.
        if (volume != NULL) {
            volume->SignalRendering(arg.approx);
            
            // Set viewport size 
            Vector<4,int> d(0, 0, arg.canvas.GetWidth(), arg.canvas.GetHeight());
            glViewport((GLsizei)d[0], (GLsizei)d[1], (GLsizei)d[2], (GLsizei)d[3]);
            CHECK_FOR_GLES2_ERROR();
            
            // apply the volume
            ApplyViewingVolume(*volume);
        }
        CHECK_FOR_GLES2_ERROR();
        
        // run the processing phases
        RenderingEventArg rarg(arg.canvas, *this, arg.start, arg.approx);
        
        this->preProcess.Notify(rarg);
        this->stage = RENDERER_PROCESS;
        this->process.Notify(rarg);
        this->stage = RENDERER_POSTPROCESS;
        this->postProcess.Notify(rarg);
        this->stage = RENDERER_PREPROCESS;
        
    }
    IEvent<RenderingEventArg>& Renderer::InitializeEvent() { return initialize; }
    IEvent<RenderingEventArg>& Renderer::PreProcessEvent() { return preProcess; }
    IEvent<RenderingEventArg>& Renderer::ProcessEvent() { return process; }
    IEvent<RenderingEventArg>& Renderer::PostProcessEvent() { return postProcess; }
    IEvent<RenderingEventArg>& Renderer::DeinitializeEvent() { return deinitialize; }

     bool Renderer::BufferSupport() { THROW(); }
     bool Renderer::FrameBufferSupport() { THROW(); }



    void Renderer::SetBackgroundColor(Vector<4,float> color) { backgroundColor = color; }
    Vector<4,float> Renderer::GetBackgroundColor() { return backgroundColor; }


     void Renderer::ApplyViewingVolume(Display::IViewingVolume& volume) { THROW(); }
     void Renderer::LoadTexture(ITexture2DPtr texr) { THROW(); }
     void Renderer::LoadTexture(ITexture2D* texr) { THROW(); }
     void Renderer::LoadTexture(ITexture3DPtr texr) { THROW(); }
     void Renderer::LoadTexture(ITexture3D* texr) { THROW(); }
     void Renderer::RebindTexture(ITexture2DPtr texr, unsigned int x, unsigned int y, unsigned int w, unsigned int h) { THROW(); }
     void Renderer::RebindTexture(ITexture2D* texr, unsigned int x, unsigned int y, unsigned int w, unsigned int h) { THROW(); }
     void Renderer::RebindTexture(ITexture3DPtr texr, unsigned int x, unsigned int y, unsigned int z, unsigned int w, unsigned int h, unsigned int d) { THROW(); }
     void Renderer::RebindTexture(ITexture3D* texr, unsigned int x, unsigned int y, unsigned int z, unsigned int w, unsigned int h, unsigned int d) { THROW(); }
     void Renderer::BindFrameBuffer(FrameBuffer* fb) { THROW(); }
     void Renderer::BindDataBlock(IDataBlock* bo) { THROW(); }
     void Renderer::DrawFace(FacePtr face) { THROW(); }
     void Renderer::DrawFace(FacePtr face, Vector<3,float> color, float width ) { THROW(); }
     void Renderer::DrawLine(Line line, Vector<3,float> color, float width ) { THROW(); }
     void Renderer::DrawPoint(Vector<3,float> point, Vector<3,float> color , float size) { THROW(); }
     void Renderer::DrawSphere(Vector<3,float> center, float radius, Vector<3,float> color) { THROW(); }


}
}
}
