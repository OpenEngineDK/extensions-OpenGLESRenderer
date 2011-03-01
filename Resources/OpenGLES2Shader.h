// 
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS)
//
// This program is free software; It is covered by the GNU General
// Public License version 2 or any later version.
// See the GNU General Public License for more details (see LICENSE).
//--------------------------------------------------------------------

#ifndef _OpenGL_ES_2_Shader_H_
#define _OpenGL_ES_2_Shader_H_

#include <Resources/IShaderResource.h>
#include <Resources/IResourcePlugin.h>
#include <Meta/OpenGLES2.h>
#include <Logging/Logger.h>

namespace OpenEngine {
namespace Resources {
/**
 * Short description.
 *
 * @class OpenGLES2Shader OpenGLES2Shader.h ons/iOS/Resources/OpenGLES2Shader.h
 */
class OpenGLES2Shader /* : public IShaderResource*/ {
private:
    string vertexShader, fragmentShader;
    
    GLuint LoadShader(string file, GLenum type);
    GLuint programID;
    
    
public:
    OpenGLES2Shader(string vertex, string fragment);
    
    void ApplyShader();
    void ReleaseShader();

    GLint GetUniformLocation(string name);

    void SetUniform(string name, int arg, bool force = false);
    void SetUniform(string name, float value, bool force = false);
    void SetUniform(string name, Vector<2, float> value, bool force = false);
    void SetUniform(string name, Vector<3, float> value, bool force = false);
    void SetUniform(string name, Vector<4, float> value, bool force = false);
    void SetUniform(string name, Matrix<4, 4, float> value, bool force = false);
    
    void GetUniform(string name, int &value);
    void GetUniform(string name, float &value);
    void GetUniform(string name, Vector<2, float> &value);
    void GetUniform(string name, Vector<3, float> &value);
    void GetUniform(string name, Vector<4, float> &value);
    void GetUniform(string name, Matrix<4, 4, float> &value);
    
    void SetTexture(string name, ITexture2DPtr tex, bool force = false);
    void SetTexture(string name, ITexture3DPtr tex, bool force = false);

    void GetTexture(string name, ITexture2DPtr &tex);
    void GetTexture(string name, ITexture3DPtr &tex);

    int GetUniformID(string name);

    TextureList GetTextures();

    void SetAttribute(string name, IDataBlockPtr values) { throw "error"; };
    void SetAttribute(string name, Vector<3, float> value);

    bool HasAttribute(string name) { throw "error"; }

    void BindAttribute(int id, string name);

    void VertexAttribute(int id, Vector<3,float> vec);
    
    int GetAttributeID(const string name);

    int GetShaderModel()  { throw "error"; }
    bool HasVertexSupport()  { throw "error"; }
    bool HasGeometrySupport()  { throw "error"; }
    bool HasFragmentSupport()  { throw "error"; }

    void Load();
    void Unload();

};

typedef boost::shared_ptr<OpenGLES2Shader> OpenGLES2ShaderPtr;

class GLES2ShaderPlugin : public IResourcePlugin<OpenGLES2Shader> {
public:
    GLES2ShaderPlugin() {
        this->AddExtension("glsl");
    }
    OpenGLES2ShaderPtr CreateResource(string file);
};
    
} // NS Resources
} // NS OpenEngine

#endif //_OpenGL_ES_2_Shader_H_
