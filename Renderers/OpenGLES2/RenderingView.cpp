#include "RenderingView.h"

#include <Logging/Logger.h>
#include <Scene/ISceneNode.h>
#include <Scene/TransformationNode.h>
#include <Scene/MeshNode.h>
#include <Geometry/Mesh.h>
#include <Geometry/GeometrySet.h>

namespace OpenEngine {
namespace Renderers {
namespace OpenGLES2 {
    
    using namespace Geometry;

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
    
    RenderingView::RenderingView() : modelView(Matrix<4,4,float>()) {}
    
    void RenderingView::VisitTransformationNode(TransformationNode *node) {
        Matrix<4,4,float> oldModel = modelView;        
        Matrix<4,4,float> t = node->GetTransformationMatrix();
        modelView = t;
        shaderProgram->SetUniform("mv_matrix",t);
        node->VisitSubNodes(*this);
        shaderProgram->SetUniform("mv_matrix",oldModel);
        modelView = oldModel;
        logger.info << modelView << logger.end;
    }
    
    void RenderingView::VisitMeshNode(MeshNode *node) {
        
        MeshPtr mesh = node->GetMesh();
        GeometrySetPtr geom = mesh->GetGeometrySet();
    
        IDataBlockPtr v = geom->GetVertices();
        void* vPtr = v->GetVoidDataPtr();
        
        glVertexAttribPointer(0, v->GetDimension(), GL_FLOAT, GL_FALSE, 0, vPtr);
        
        IndicesPtr indexBuffer = mesh->GetIndices();
        
        GLsizei count = mesh->GetDrawingRange();
        Geometry::Type type = mesh->GetType();
        GLuint offset = mesh->GetIndexOffset();
        CHECK_FOR_GLES2_ERROR();
        
        //logger.info << indexBuffer->ToString() << logger.end;
          
        //glDrawArrays(GL_TRIANGLES, 0, count);
        //glDrawArrays(GL_LINE_STRIP, 0, count);
        
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, indexBuffer->GetData() + offset);
        CHECK_FOR_GLES2_ERROR();
    }
    

    void RenderingView::Handle(RenderingEventArg arg) {
        if (arg.renderer.GetCurrentStage() == IRenderer::RENDERER_PROCESS) {
            static const GLfloat squareVertices[] = {
                0.0, 0.5, 0.0,
                -0.5, -0.5, 0.0,
                0.5, -0.5, 0.0
            };
            
       //     static const GLubyte squareColors[] = {
//                255, 255,   0, 255,
//                0,   255, 255, 255,
//                0,     0,   0,   0,
//                255,   0, 255, 255,
//            };
            
            shaderProgram->ApplyShader();
            //glUseProgram(program);
            CHECK_FOR_GLES2_ERROR();

            // Update attribute values.
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, squareVertices);
            glEnableVertexAttribArray(0);
            
            
            //glVertexAttribPointer(ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, 1, 0, squareColors);
            //glEnableVertexAttribArray(ATTRIB_COLOR);
            CHECK_FOR_GLES2_ERROR();

//            glDrawArrays(GL_TRIANGLES, 0, 3);
            
            
            arg.canvas.GetScene()->Accept(*this);

            
            
            shaderProgram->ReleaseShader();
            
            CHECK_FOR_GLES2_ERROR();
        
        } else if (arg.renderer.GetCurrentStage() == IRenderer::RENDERER_INITIALIZE) {
            
            if (shaderProgram) {
                logger.info << "Time to load shader" << logger.end;
                shaderProgram->Load();
                shaderProgram->BindAttribute(0,"vPosition");
            }
            
            CHECK_FOR_GLES2_ERROR();

        }
    }

}
}
}
