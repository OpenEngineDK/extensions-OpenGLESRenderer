#include "LightRenderer.h"

#include <Logging/Logger.h>
#include <Scene/TransformationNode.h>

namespace OpenEngine {
namespace Renderers {
namespace OpenGLES2 {

LightRenderer::LightRenderer() {
    
}
void LightRenderer::Handle(RenderingEventArg arg) {
    
    count = 0;
    transformation = Matrix<4,4,float>();    
    lights.clear();

    arg.canvas.GetScene()->Accept(*this);        

    // Do stuff with light?
}

void LightRenderer::VisitTransformationNode(TransformationNode *node) {    
    Matrix<4,4,float> m = node->GetTransformationMatrix();
    Matrix<4,4,float> oldT = transformation;
    transformation = m * transformation;
    
    node->VisitSubNodes(*this);

    transformation = oldT;
}

void LightRenderer::VisitDirectionalLightNode(DirectionalLightNode *node) {
    Vector<4,float> v(0,0,0,1);
    Vector<4,float> p = transformation.GetTranspose() * v;

    Light l;
    l.kind = Light::DIRECTIONAL;
    l.position = Vector<3,float>(p[0],p[1],p[2]);
    
    lights.push_back(l);
    
}

    list<LightRenderer::Light> LightRenderer::GetLights() {
    return lights;
}

}
}
}
