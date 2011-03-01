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
#include <Resources/OpenGLES2Shader.h>
#include <Geometry/Mesh.h>
#include <Geometry/GeometrySet.h>
#include <Renderers/OpenGLES2/LightRenderer.h>

namespace OpenEngine {
namespace Renderers {
namespace OpenGLES2 {
    
    using namespace Resources;
    using namespace Scene;
    using namespace Geometry;
    using namespace Display;

/**
 * Short description.
 *
 * @class RenderingView RenderingView.h ns/iOS/Renderers/OpenGLES2/RenderingView.h
 */
class RenderingView : public IRenderingView {
private:
    GLuint program;
    OpenGLES2ShaderPtr shaderProgram;
    Matrix<4,4,float> modelView;
    LightRenderer* lightRenderer;

public:
    RenderingView();
    void Handle(RenderingEventArg arg);
    void SetMainProgram(OpenGLES2ShaderPtr prog) { shaderProgram = prog; }
    void VisitTransformationNode(TransformationNode *node);
    void VisitMeshNode(MeshNode *node);
    inline void ApplyMaterial(OpenGLES2ShaderPtr shader, Geometry::MaterialPtr mat);
    void ApplyGeometrySet(OpenGLES2ShaderPtr shader, GeometrySetPtr geom);
    void ApplyMesh(Mesh* prim);
    
    LightRenderer* GetLightRenderer() { return lightRenderer; }
};

} // NS OpenGLES2
}
} // NS OpenEngine

#endif // _OE_RENDERING_VIEW_H_
