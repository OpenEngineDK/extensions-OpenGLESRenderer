#include "RenderingView.h"

#include <Logging/Logger.h>
#include <Scene/ISceneNode.h>
#include <Scene/TransformationNode.h>
#include <Scene/MeshNode.h>
#include <Display/IViewingVolume.h>
#include <Resources/DataBlock.h>

namespace OpenEngine {
namespace Renderers {
namespace OpenGLES2 {
    
    using namespace Geometry;
    using namespace Display;
    
    GLuint LoadShader(GLenum type, const char *shaderSrc) {
        GLuint shader;
        GLint compiled;
        
        shader = glCreateShader(type);
        glShaderSource(shader, 1, &shaderSrc, NULL);
        glCompileShader(shader);
        
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled)
            logger.error << "compile err" << logger.end;
        
        return shader;
    }
    
    RenderingView::RenderingView() : modelView(Matrix<4,4,float>()) {
        lightRenderer = new LightRenderer();        
    }
    
    void RenderingView::VisitTransformationNode(TransformationNode *node) {
        Matrix<4,4,float> oldModel = modelView;        
        Matrix<4,4,float> t = node->GetTransformationMatrix();
        Matrix<4,4,float> inverse;
        modelView = t * modelView ;
        
        inverse = modelView.GetInverse().GetTranspose();
        
        shaderProgram->SetUniform("mv_matrix",modelView);
        shaderProgram->SetUniform("inv_matrix",inverse);
        node->VisitSubNodes(*this);
        shaderProgram->SetUniform("mv_matrix",oldModel);
        inverse = oldModel.GetInverse().GetTranspose();
        shaderProgram->SetUniform("inv_matrix",inverse);
        modelView = oldModel;
        //logger.info << modelView << logger.end;
    }

    void RenderingView::ApplyGeometrySet(OpenGLES2ShaderPtr shader, GeometrySetPtr geom) {
        if (geom == NULL) {
            return;
        }

        AttributeBlocks attrs = geom->GetAttributeLists();
        AttributeBlocks::iterator attrItr = attrs.begin();
        while (attrItr != attrs.end()){
            GLint loc = shader->GetAttributeID(attrItr->first);
            if (loc != -1){
                IDataBlockPtr block = attrItr->second;
                glVertexAttribPointer(loc, block->GetDimension(), block->GetType(), GL_FALSE, 0, block->GetVoidData());
                glEnableVertexAttribArray(loc);
                CHECK_FOR_GLES2_ERROR();
            }
            attrItr++;
        }
    }

    void RenderingView::ApplyMaterial(OpenGLES2ShaderPtr shader, MaterialPtr mat) {
        if (mat->Get2DTextures().size()) {

            GLuint texID = (*mat->Get2DTextures().begin()).second->GetID();
            
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texID);
            shader->SetUniform("texture1",0);
            
        }
        CHECK_FOR_GLES2_ERROR();

    }

    void RenderingView::ApplyMesh(Mesh* prim) {
        if (prim == NULL) {
            return;
        }

        ApplyGeometrySet(shaderProgram, prim->GetGeometrySet());
        ApplyMaterial(shaderProgram, prim->GetMaterial());

        IndicesPtr indexBuffer = prim->GetIndices();
        
        GLsizei count = prim->GetDrawingRange();
        Geometry::Type type = prim->GetType();
        GLuint offset = prim->GetIndexOffset();
        CHECK_FOR_GLES2_ERROR();
        
        //logger.info << indexBuffer->ToString() << logger.end;
          
        //glDrawArrays(GL_TRIANGLES, 0, count);
        //glDrawArrays(GL_LINE_STRIP, 0, count);
        
        glDrawElements(type, count, GL_UNSIGNED_SHORT, indexBuffer->GetData() + offset);
        
        
    }
    
    void RenderingView::VisitMeshNode(MeshNode *node) {
        ApplyMesh(node->GetMesh().get());
        node->VisitSubNodes(*this);
        CHECK_FOR_GLES2_ERROR();                    
    }

    

    void RenderingView::Handle(RenderingEventArg arg) {
        if (arg.renderer.GetCurrentStage() == IRenderer::RENDERER_PROCESS) {

            glCullFace(GL_BACK);
            //glCullFace(GL_FRONT);
            glEnable(GL_CULL_FACE);
            
            glEnable(GL_DEPTH_TEST);

            shaderProgram->ApplyShader();
            
            IRenderCanvas& canvas = arg.canvas;
            IViewingVolume* viewingVolume = canvas.GetViewingVolume();
            
            if (viewingVolume) {
                Matrix<4,4,float> projectionMatrix = viewingVolume->GetProjectionMatrix();
                Matrix<4,4,float> viewMatrix = viewingVolume->GetViewMatrix();
                modelView = viewMatrix;
                shaderProgram->SetUniform("proj_matrix",projectionMatrix);
                
                //logger.info << projectionMatrix << logger.end;
            }
            
            list<LightRenderer::Light> lights = lightRenderer->GetLights();
            for (list<LightRenderer::Light>::iterator itr = lights.begin();
                 itr != lights.end();
                 itr++) {
                LightRenderer::Light l = *itr;
                switch (l.kind) {
                    case LightRenderer::Light::DIRECTIONAL:
                        shaderProgram->SetUniform("u_dir_light_pos",l.position);
                        break;
                    default:
                        break;
                }
            }
            
            //glUseProgram(program);
            CHECK_FOR_GLES2_ERROR();

            arg.canvas.GetScene()->Accept(*this);


            shaderProgram->ReleaseShader();
            
            CHECK_FOR_GLES2_ERROR();
        
        } else if (arg.renderer.GetCurrentStage() == IRenderer::RENDERER_INITIALIZE) {
            
            if (shaderProgram) {
                logger.info << "Time to load shader" << logger.end;
                shaderProgram->Load();
            }
            CHECK_FOR_GLES2_ERROR();

        }
    }

}
}
}
