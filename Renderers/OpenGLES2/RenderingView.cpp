#include "RenderingView.h"

#include <Logging/Logger.h>
#include <Scene/ISceneNode.h>
#include <Scene/TransformationNode.h>
#include <Scene/MeshNode.h>
#include <Display/IViewingVolume.h>
#include <Resources/DataBlock.h>
#include <Resources/DirectoryManager.h>
#include <Resources/File.h>
#include <Resources/OpenGLES2Shader.h>
#include <Resources/OpenGLESBuffer.h>
#include <Resources/OpenGLESIndices.h>

namespace OpenEngine {
namespace Renderers {
namespace OpenGLES2 {
    
    using namespace Display;
    using namespace Geometry;
    using namespace Resources;
    
    RenderingView::RenderingView() 
        : modelView(Matrix<4,4,float>()), 
          normalMatrix(modelView.GetInverse().GetTranspose()), 
          arg(NULL){
        lightRenderer = new LightRenderer();

        // Load the über shaders
                string filename = DirectoryManager::FindFileInPath("Shaders/ESUberShader.glsl");
        ifstream* in = File::Open(filename);

        char buf[255], file[255];
        int line = 0;

        string vertexFilename, fragFilename;
        
        while (!in->eof()) {
            ++line;
            in->getline(buf, 255);
            
            string type = string(buf,5);
            if (type.empty() || buf[0] == '#')
                continue;
            
            if (type == "vert:") {
                if (sscanf(buf, "vert: %s", file) == 1)
                    vertexFilename = string(file);                
            } else if (type == "frag:")
                if (sscanf(buf, "frag: %s", file) == 1)
                    fragFilename = string(file);
        }
        
        in->close();
        delete in;

        vertexFilename = DirectoryManager::FindFileInPath(vertexFilename);
        char *vertShdr = File::ReadShader<char>(vertexFilename);
        uberVert = string(vertShdr);

        fragFilename = DirectoryManager::FindFileInPath(fragFilename);
        char *fragShdr = File::ReadShader<char>(fragFilename);
        uberFrag = string(fragShdr);
    }
    
    RenderingView::~RenderingView() {
        delete lightRenderer;
        // @TODO clean up decorators
    }

    void RenderingView::VisitTransformationNode(TransformationNode *node) {
        // Store old matrices
        Matrix<4,4,float> oldModel = modelView;
        Matrix<4,4,float> oldNormal = normalMatrix;

        // Calc new ones
        Matrix<4,4,float> t = node->GetTransformationMatrix();
        modelView = t * modelView;        
        normalMatrix = modelView.GetInverse().GetTranspose();
        
        node->VisitSubNodes(*this);

        // Restore old matrices
        normalMatrix = oldNormal;
        modelView = oldModel;
    }

    void RenderingView::ApplyGeometrySet(GeometrySetPtr geom, OpenGLES2Shader* shader) {
        if (geom == NULL) return;

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

    void RenderingView::ApplyMaterial(MaterialPtr mat, OpenGLES2Shader* shader) {
        if (mat == NULL) return;

        GLint texUnit = 0;

        map<string, ITexture2DPtr> texs2D = mat->Get2DTextures();
        map<string, ITexture2DPtr>::iterator itr = texs2D.begin();
        while (itr != texs2D.end()) {
            GLint loc = shader->GetUniformLocation(itr->first);
            if (loc != -1){
                ITexture2DPtr tex = itr->second;
                glActiveTexture(GL_TEXTURE0 + texUnit);
                glBindTexture(GL_TEXTURE_2D, tex->GetID());
                shader->SetUniform(itr->first, texUnit);
                texUnit++;
                CHECK_FOR_GLES2_ERROR();
            }
            itr++;
        }

        // Apply lights
        list<LightRenderer::Light> lights = lightRenderer->GetLights();
        for (list<LightRenderer::Light>::iterator itr = lights.begin();
             itr != lights.end();
             itr++) {
            LightRenderer::Light l = *itr;
            switch (l.kind) {
            case LightRenderer::Light::DIRECTIONAL:
                shader->SetUniform("u_dir_light_pos",l.position);
                break;
            default:
                break;
            }
        }
    }

    void RenderingView::ApplyMesh(Mesh* prim, OpenGLES2Shader* shader) {
        if (prim == NULL) {
            return;
        }

        shader->ApplyShader();
        shader->SetUniform("proj_matrix", arg->canvas.GetViewingVolume()->GetProjectionMatrix());
        shader->SetUniform("mv_matrix",modelView);
        shader->SetUniform("norm_matrix",normalMatrix);
        shader->SetUniform("useLight", 1);

        ApplyGeometrySet(prim->GetGeometrySet(), shader);
        ApplyMaterial(prim->GetMaterial(), shader);

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
        map<ISceneNode*, void*>::const_iterator itr = nodeDecorations.find(node);
        MeshDecoration* deco;
        if (itr == nodeDecorations.end())
            deco = DecorateMeshNode(node);
        else
            deco = (MeshDecoration*) itr->second;

        ApplyMesh(node->GetMesh().get(), deco->shader);
        node->VisitSubNodes(*this);
        CHECK_FOR_GLES2_ERROR();                    
    }

    void RenderingView::Handle(RenderingEventArg arg) {
        this->arg = &arg;
        if (arg.renderer.GetCurrentStage() == IRenderer::RENDERER_PROCESS) {

            glCullFace(GL_BACK);
            glEnable(GL_CULL_FACE);
            
            glEnable(GL_DEPTH_TEST);

            IRenderCanvas& canvas = arg.canvas;
            IViewingVolume* viewingVolume = canvas.GetViewingVolume();
            
            if (viewingVolume) {
                Matrix<4,4,float> viewMatrix = viewingVolume->GetViewMatrix();
                modelView = viewMatrix;
                normalMatrix = viewMatrix.GetInverse().GetTranspose();
                
                //logger.info << projectionMatrix << logger.end;
            }

            arg.canvas.GetScene()->Accept(*this);

            CHECK_FOR_GLES2_ERROR();
        
        } else if (arg.renderer.GetCurrentStage() == IRenderer::RENDERER_INITIALIZE) {
            CHECK_FOR_GLES2_ERROR();

        }
    }

    /*** DECORATER METHODS ***/
    
    RenderingView::MeshDecoration* RenderingView::DecorateMeshNode(MeshNode* node){
        logger.info << "Uh met a mesh node in need of decoration" << logger.end;


        MeshDecoration* deco = new MeshDecoration(new OpenGLES2Shader(uberVert, uberFrag));
        deco->shader->Load();

        nodeDecorations[node] = deco;

        return deco;
    }

}
}
}
