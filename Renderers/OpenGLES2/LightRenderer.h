// 
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS)
//
// This program is free software; It is covered by the GNU General
// Public License version 2 or any later version.
// See the GNU General Public License for more details (see LICENSE).
//--------------------------------------------------------------------


#ifndef _OE_LIGHT_RENDERER_H_
#define _OE_LIGHT_RENDERER_H_

#include <Renderers/IRenderer.h>
#include <Scene/ISceneNodeVisitor.h>
#include <Core/IListener.h>

#include <Meta/OpenGLES2.h>
#include <Math/Matrix.h>

#include <list>

using namespace std;

namespace OpenEngine {
    namespace Scene {
        class TransformationNode;
        class PointLightNode;
        class DirectionalLightNode;
        class SpotLightNode;
    }
namespace Renderers {
namespace OpenGLES2 {

using namespace OpenEngine::Scene;
using OpenEngine::Core::IListener;
using OpenEngine::Renderers::RenderingEventArg;
using namespace OpenEngine::Math;


/**
 * Short description.
 *
 * @class LightRenderer LightRenderer.h ns/iOS/Renderers/OpenGLES2/LightRenderer.h
 */
class LightRenderer: public ISceneNodeVisitor, public IListener<RenderingEventArg> {
public:
    class Light {
    public:
        enum Kind {
            DIRECTIONAL
        };

        Kind kind;
        Vector<3,float> position;
    };

private:
    int count;
    Matrix<4,4,float> transformation;
    list<Light> lights;
public:
    LightRenderer();    

    void Handle(RenderingEventArg arg);
    void VisitTransformationNode(TransformationNode *node);
    void VisitDirectionalLightNode(DirectionalLightNode *node);

    list<Light> GetLights();
};


} // NS OpenGLES2
}
} // NS OpenEngine

#endif // _OE_LIGHT_RENDERER_H_
