// 
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS)
//
// This program is free software; It is covered by the GNU General
// Public License version 2 or any later version.
// See the GNU General Public License for more details (see LICENSE).
//--------------------------------------------------------------------

#include <Resources/IShaderResource.h>
#include <Resources/IResourcePlugin.h>
#include <Meta/OpenGLES2.h>

namespace OpenEngine {
namespace Resources {
/**
 * Short description.
 *
 * @class OpenGLES2Shader OpenGLES2Shader.h ons/iOS/Resources/OpenGLES2Shader.h
 */
class OpenGLES2Shader : public IShaderResource {
private:
    string filename;
    string vertex_filename;
    string fragment_filename;
    
    void LoadResource(string file);
    GLuint LoadShader(string file, GLenum type);
    GLuint programID;
    
    
public:
    OpenGLES2Shader(string file);
    
    void ApplyShader();
    void ReleaseShader();

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

class GLES2ShaderPlugin : public IResourcePlugin<IShaderResource> {
public:
    GLES2ShaderPlugin() {
        this->AddExtension("glsl");
    }
    IShaderResourcePtr CreateResource(string file) {
        return IShaderResourcePtr(new OpenGLES2Shader(file));
    }
};
    
} // NS Resources
} // NS OpenEngine
