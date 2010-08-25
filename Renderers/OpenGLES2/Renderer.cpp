#include "Renderer.h"

#include <Meta/iOS.h>
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


     void Renderer::ApplyViewingVolume(Display::IViewingVolume& volume) {
         // nop
     }
    
    GLint Renderer::GLInternalColorFormat(ColorFormat f){
        switch (f) {
            case ALPHA:
                return GL_ALPHA;
            case LUMINANCE: 
                return GL_LUMINANCE;
            case LUMINANCE_ALPHA: 
                return GL_LUMINANCE_ALPHA;
            case BGR:
            case RGB: 
                return GL_RGB;
            case BGRA: 
            case RGBA: 
                return GL_RGBA;
//            case ALPHA_COMPRESSED: return GL_COMPRESSED_ALPHA;
//            case LUMINANCE_COMPRESSED: return GL_COMPRESSED_LUMINANCE;
//            case LUMINANCE32F: return GL_R32F;
//            case LUMINANCE_ALPHA_COMPRESSED: return GL_COMPRESSED_LUMINANCE_ALPHA;
//            case RGB_COMPRESSED: return GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
//            case RGBA_COMPRESSED: return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
//            case RGB32F: return GL_RGB32F;
//            case RGBA32F: return GL_RGBA32F;
            case DEPTH: return GL_DEPTH_COMPONENT;
            default: 
                logger.warning << "Unsupported color format: " << f << logger.end;
                logger.warning << "Defaulting to RGBA." << logger.end;
        }
        return GL_RGBA;
    }
    
    GLenum Renderer::GLColorFormat(ColorFormat f){
        switch (f) {
            case ALPHA:
            case ALPHA_COMPRESSED:
                return GL_ALPHA;
            case LUMINANCE: 
            case LUMINANCE_COMPRESSED: 
            case LUMINANCE32F:
                return GL_LUMINANCE;
            case LUMINANCE_ALPHA: 
            case LUMINANCE_ALPHA_COMPRESSED: 
                return GL_LUMINANCE_ALPHA;
            case RGB: 
            case RGB32F: 
            case RGB_COMPRESSED: 
                return GL_RGB;
            case RGBA: 
            case RGBA_COMPRESSED: 
            case RGBA32F: 
                return GL_RGBA;
//            case BGR: 
//                return GL_BGR;
//            case BGRA: 
//                return GL_BGRA;
            case DEPTH: 
                return GL_DEPTH_COMPONENT;
            default: 
                logger.warning << "Unsupported color format: " << f << logger.end;
                logger.warning << "Defaulting to RGBA." << logger.end;
        }
        return GL_RGBA;
    }
    
    
     void Renderer::LoadTexture(ITexture2DPtr texr) { LoadTexture(texr.get()); }
     void Renderer::LoadTexture(ITexture2D* texr) { 
         // check for null pointers
         if (texr == NULL) return;
         
         // check if textures has already been bound.
         if (texr->GetID() != 0) return;
         
         // signal we need the texture data if not loaded.
         bool loaded = true;
         if (texr->GetVoidDataPtr() == NULL){
             loaded = false;
             texr->Load(); //@todo: what the #@!%?
         }
         
         
         // Generate and bind the texture id.
         GLuint texid;
         glGenTextures(1, &texid);
         CHECK_FOR_GLES2_ERROR();
         
         
         texr->SetID(texid);
         glBindTexture(GL_TEXTURE_2D, texid);
         CHECK_FOR_GLES2_ERROR();
         
         SetupTexParameters(texr);
         CHECK_FOR_GLES2_ERROR();
         
//         SetTextureCompression(texr);
         GLint internalFormat = GLInternalColorFormat(texr->GetColorFormat());
         GLenum colorFormat = GLColorFormat(texr->GetColorFormat());
         
         logger.info << "Load: " << internalFormat << " " << colorFormat << logger.end;
         
         glTexImage2D(GL_TEXTURE_2D,
                      0, // mipmap level
                      internalFormat,
                      texr->GetWidth(),
                      texr->GetHeight(),
                      0, // border
                      colorFormat,
                      texr->GetType(),
                      texr->GetVoidDataPtr());
         CHECK_FOR_GLES2_ERROR();
         
         glBindTexture(GL_TEXTURE_2D, 0);
         
         // Return the texture in the state we got it.
         if (!loaded)
             texr->Unload();
         
     }
    
    void Renderer::SetupTexParameters(ITexture2D* tex){
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        CHECK_FOR_GLES2_ERROR();
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,         tex->GetWrapping());
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,         tex->GetWrapping());
//        if (tex->UseMipmapping()){
//            glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP,    GL_TRUE);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, tex->GetFiltering());
//        }else{
//            glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP,    GL_FALSE);
//            if (tex->GetFiltering() == NONE)
//                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//            else
//                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//        }
//        if (tex->GetFiltering() == NONE)
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//        else
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

        
    }

    
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
