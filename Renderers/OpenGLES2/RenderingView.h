// 
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS)
//
// This program is free software; It is covered by the GNU General
// Public License version 2 or any later version.
// See the GNU General Public License for more details (see LICENSE).
//--------------------------------------------------------------------


#ifndef _OE_RENDERING_VIEW_H_
#define _OE_RENDERING_VIEW_H_

#include <Renderers/IRenderingView.h>
#include <Meta/OpenGLES2.h>
#include <Resources/IShaderResource.h>

namespace OpenEngine {
namespace Renderers {
namespace OpenGLES2 {
    
    using namespace OpenEngine::Resources;
    using namespace OpenEngine::Scene;

/**
 * Short description.
 *
 * @class RenderingView RenderingView.h ns/iOS/Renderers/OpenGLES2/RenderingView.h
 */
class RenderingView : public IRenderingView {
private:
    GLuint program;
    IShaderResourcePtr shaderProgram;
public:
    RenderingView();
    void Handle(RenderingEventArg arg);
    void SetMainProgram(IShaderResourcePtr prog) { shaderProgram = prog; }
    void VisitTransformationNode(TransformationNode *node);
    void VisitMeshNode(MeshNode *node);
};

} // NS OpenGLES2
}
} // NS OpenEngine

#endif // _OE_RENDERING_VIEW_H_
