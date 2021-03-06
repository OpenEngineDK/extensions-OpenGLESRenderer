// 
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS)
//
// This program is free software; It is covered by the GNU General
// Public License version 2 or any later version.
// See the GNU General Public License for more details (see LICENSE).
//--------------------------------------------------------------------


#ifndef _OE_RENDERER_H_
#define _OE_RENDERER_H_

#include <Renderers/IRenderer.h>
#include <Meta/OpenGLES2.h>

namespace OpenEngine {
namespace Renderers {
namespace OpenGLES2 {

    using Core::Event;
    using OpenEngine::Resources::ColorFormat;


/**
 * Short description.
 *
 * @class Renderer Renderer.h ns/iOS/Renderers/OpenGLES2/Renderer.h
 */
class Renderer : public IRenderer {
private:
    // Event lists for the rendering phases.
    Event<RenderingEventArg> initialize;
    Event<RenderingEventArg> preProcess;
    Event<RenderingEventArg> process;
    Event<RenderingEventArg> postProcess;
    Event<RenderingEventArg> deinitialize;

    Vector<4,float> backgroundColor;

public:    
    Renderer();

    
    static inline GLint GLInternalColorFormat(ColorFormat f);
    static inline GLenum GLColorFormat(ColorFormat f);
    inline void SetupTexParameters(ITexture2D* tex);


    
    void Handle(Renderers::InitializeEventArg arg);
    void Handle(Renderers::DeinitializeEventArg arg);
    void Handle(Renderers::ProcessEventArg arg);
    // void Handle(ResizeEventArg arg);

    /**
     * Event lists for the rendering phases.
     */

    virtual IEvent<RenderingEventArg>& InitializeEvent();
    virtual IEvent<RenderingEventArg>& PreProcessEvent();
    virtual IEvent<RenderingEventArg>& ProcessEvent();
    virtual IEvent<RenderingEventArg>& PostProcessEvent();
    virtual IEvent<RenderingEventArg>& DeinitializeEvent();


    virtual bool BufferSupport();
    virtual bool FrameBufferSupport();



    virtual void SetBackgroundColor(Vector<4,float> color);
    virtual Vector<4,float> GetBackgroundColor();


    virtual void ApplyViewingVolume(Display::IViewingVolume& volume);
    virtual void LoadTexture(ITexture2DPtr texr);
    virtual void LoadTexture(ITexture2D* texr);
    virtual void LoadTexture(ITexture3DPtr texr);
    virtual void LoadTexture(ITexture3D* texr);
    virtual void RebindTexture(ITexture2DPtr texr, unsigned int x, unsigned int y, unsigned int w, unsigned int h);
    virtual void RebindTexture(ITexture2D* texr, unsigned int x, unsigned int y, unsigned int w, unsigned int h);
    virtual void RebindTexture(ITexture3DPtr texr, unsigned int x, unsigned int y, unsigned int z, unsigned int w, unsigned int h, unsigned int d);
    virtual void RebindTexture(ITexture3D* texr, unsigned int x, unsigned int y, unsigned int z, unsigned int w, unsigned int h, unsigned int d);
    virtual void BindFrameBuffer(FrameBuffer* fb);
    virtual void BindDataBlock(IDataBlock* bo);
    virtual void DrawFace(FacePtr face);
    virtual void DrawFace(FacePtr face, Vector<3,float> color, float width = 1);
    virtual void DrawLine(Line line, Vector<3,float> color, float width = 1);
    virtual void DrawPoint(Vector<3,float> point, Vector<3,float> color , float size);
    virtual void DrawSphere(Vector<3,float> center, float radius, Vector<3,float> color);
    virtual void RebindDataBlock(Resources::IDataBlockPtr ptr, unsigned int start, unsigned int end) { throw "error";}


};

} // NS OpenGLES2
}
} // NS OpenEngine

#endif // _OE_RENDERER_H_
