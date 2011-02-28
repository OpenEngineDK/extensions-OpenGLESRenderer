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

    void RenderingView::ApplyGeometrySet(GeometrySetPtr geom) {
        if (geom == NULL) {
            return;
        }
        IDataBlockPtr v = geom->GetVertices();
        void* vPtr = v->GetVoidDataPtr();
        
        glVertexAttribPointer(vertexLoc, v->GetDimension(), GL_FLOAT, GL_FALSE, 0, vPtr);
        glEnableVertexAttribArray(vertexLoc);
        CHECK_FOR_GLES2_ERROR();
        
        IDataBlockPtr n = geom->GetNormals();
        void* nPtr = n->GetVoidDataPtr();
        
        if (normalLoc != -1) {
            glVertexAttribPointer(normalLoc, n->GetDimension(), GL_FLOAT, GL_FALSE, 0, nPtr);
            glEnableVertexAttribArray(normalLoc);
            CHECK_FOR_GLES2_ERROR();
        }
        IDataBlockPtr fstTexC = geom->GetDataBlock("texCoord0");
        if (fstTexC == NULL)
            return;
        //logger.info << fstTexC->ToString() << logger.end;
        
        void* tPtr = fstTexC->GetVoidDataPtr();
        
        glVertexAttribPointer(texCoordLoc, fstTexC->GetDimension(), GL_FLOAT, GL_FALSE, 0, tPtr);
        glEnableVertexAttribArray(texCoordLoc);
        
        CHECK_FOR_GLES2_ERROR();

    }

    void RenderingView::ApplyMesh(Mesh* prim) {
        if (prim == NULL) {
            return;
        }

        ApplyGeometrySet(prim->GetGeometrySet());
        

        ApplyMaterial(prim->GetMaterial());

        

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

    void RenderingView::ApplyMaterial(MaterialPtr mat) {
        if (mat->Get2DTextures().size()) {
            //glEnable(GL_TEXTURE_2D);

            GLuint texID = (*mat->Get2DTextures().begin()).second->GetID();
            
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texID);
            shaderProgram->SetUniform("texture1",0);
            
        }
        CHECK_FOR_GLES2_ERROR();

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
                //shaderProgram->BindAttribute(0,"v_position");
                //shaderProgram->BindAttribute(1,"vNormal");
                //shaderProgram->BindAttribute(2,"vTexCoord");

                vertexLoc = shaderProgram->GetAttributeID("a_position");
                normalLoc = shaderProgram->GetAttributeID("a_normal");
                texCoordLoc = shaderProgram->GetAttributeID("a_texcoord");
            }
            
            
            
            CHECK_FOR_GLES2_ERROR();

        }
    }

}
}
}
