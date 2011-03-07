// OpenGLES 2 RenderingView
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
#include <Geometry/Mesh.h>
#include <Geometry/GeometrySet.h>
#include <Renderers/OpenGLES2/LightRenderer.h>

namespace OpenEngine {
    namespace Geometry {
        class NewGeometrySet;
        class NewMesh;
    }
    namespace Resources {
        class OpenGLES2Shader;
        class IBuffer;
    }
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
protected:
    struct MeshDecoration {
        OpenGLES2Shader* shader;
        MeshDecoration(OpenGLES2Shader* shader) : shader(shader) {}
    };

    Matrix<4,4,float> modelView;
    Matrix<4,4,float> normalMatrix;
    LightRenderer* lightRenderer;
    RenderingEventArg *arg;
    map<ISceneNode*, void*> nodeDecorations;
    string uberVert, uberFrag;

public:
    RenderingView();
    virtual ~RenderingView();
    void Handle(RenderingEventArg arg);
    void VisitTransformationNode(TransformationNode *node);
    void VisitMeshNode(MeshNode *node);
    void VisitNewMeshNode(NewMeshNode *node);
    LightRenderer* GetLightRenderer() { return lightRenderer; }

protected:
    void ApplyMaterial(Geometry::MaterialPtr mat, OpenGLES2Shader* shader);
    void ApplyGeometrySet(GeometrySetPtr geom,OpenGLES2Shader* shader);
    void ApplyGeometrySet(NewGeometrySet* geom, OpenGLES2Shader* shader);
    void ApplyMesh(Mesh* prim, OpenGLES2Shader* shader);
    void ApplyMesh(NewMesh* prim, OpenGLES2Shader* shader);
    
    MeshDecoration* DecorateMeshNode(MeshNode* node);
    MeshDecoration* DecorateMeshNode(NewMeshNode* node);
};

} // NS OpenGLES2
}
} // NS OpenEngine

#endif // _OE_RENDERING_VIEW_H_
