
#include "OpenGLES2Shader.h"
#include <Meta/iOS.h>
#include <Resources/File.h>
#include <Resources/DirectoryManager.h>
#include <Logging/Logger.h>
#include <fstream>

namespace OpenEngine {
namespace Resources {

    using namespace std;
    
    OpenGLES2Shader::OpenGLES2Shader(string vertex, string fragment) 
        : vertexShader(vertex), fragmentShader(fragment) {
        
    }
    
    void OpenGLES2Shader::ApplyShader() {
        glUseProgram(programID);
    }
    void OpenGLES2Shader::ReleaseShader() { 
        glUseProgram(0);
    }

    GLint OpenGLES2Shader::GetUniformLocation(string name) {
        GLint loc = glGetUniformLocation(programID, name.c_str());
        if (loc == -1)
            logger.error << "Uniform not found: " << name << logger.end;
        return loc;
    }
    
    void OpenGLES2Shader::SetUniform(string name, int value, bool force) { 
        glUniform1i(GetUniformLocation(name), value);
    }
    void OpenGLES2Shader::SetUniform(string name, float value, bool force) { 
        glUniform1f(GetUniformLocation(name), value);
    }
    void OpenGLES2Shader::SetUniform(string name, Vector<2, float> value, bool force ) { 
        glUniform2fv(GetUniformLocation(name), 2, value.ToArray());
    }
    void OpenGLES2Shader::SetUniform(string name, Vector<3, float> value, bool force ) { 
        glUniform3fv(GetUniformLocation(name), 3, value.ToArray());
    }
    void OpenGLES2Shader::SetUniform(string name, Vector<4, float> value, bool force ) { 
        glUniform4fv(GetUniformLocation(name), 4, value.ToArray());
    }
    void OpenGLES2Shader::SetUniform(string name, Matrix<4, 4, float> value, bool force ) { 
        glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, value.ToArray());
    }
    
    void OpenGLES2Shader::GetUniform(string name, int &value) { THROW(); }
    void OpenGLES2Shader::GetUniform(string name, float &value) { THROW(); }
    void OpenGLES2Shader::GetUniform(string name, Vector<2, float> &value) { THROW(); }
    void OpenGLES2Shader::GetUniform(string name, Vector<3, float> &value) { THROW(); }
    void OpenGLES2Shader::GetUniform(string name, Vector<4, float> &value) { THROW(); }
    void OpenGLES2Shader::GetUniform(string name, Matrix<4, 4, float> &value) { THROW(); }
    
    void OpenGLES2Shader::SetTexture(string name, ITexture2DPtr tex, bool force ) { THROW(); }
    void OpenGLES2Shader::SetTexture(string name, ITexture3DPtr tex, bool force ) { THROW(); }
    
    void OpenGLES2Shader::GetTexture(string name, ITexture2DPtr &tex) { THROW(); }
    void OpenGLES2Shader::GetTexture(string name, ITexture3DPtr &tex) { THROW(); }
    
    int OpenGLES2Shader::GetUniformID(string name) { THROW(); }
    
    TextureList OpenGLES2Shader::GetTextures() { THROW(); }
    
    void OpenGLES2Shader::SetAttribute(string name, Vector<3, float> value) { THROW(); }
    
    void OpenGLES2Shader::BindAttribute(int idx, string name) { 
        logger.info << "Bind attributes!" << logger.end;
        glBindAttribLocation(programID, idx, name.c_str());
    }
    
    void OpenGLES2Shader::VertexAttribute(int id, Vector<3,float> vec) { THROW(); }
    
    GLint OpenGLES2Shader::GetAttributeID(const string name) { 
        const char *str = name.c_str();
        GLint loc = glGetAttribLocation(programID, str);
        if (loc == -1)
            logger.error << "Attribute not found: " << name << logger.end;
        return loc;
    }
    void OpenGLES2Shader::Load() { 
        GLuint vid = LoadShader(vertexShader, GL_VERTEX_SHADER);
        GLuint fid = LoadShader(fragmentShader, GL_FRAGMENT_SHADER);
        
        programID = glCreateProgram();
        glAttachShader(programID, vid);
        glAttachShader(programID, fid);
        
        glLinkProgram(programID);
        
        GLint linked;
        glGetProgramiv(programID, GL_LINK_STATUS, &linked);
        if (!linked) {
            logger.error << "Failed to link program" << logger.end;
        }
        
    }
    
    GLuint OpenGLES2Shader::LoadShader(string file, GLenum type) {
        GLuint shader = glCreateShader(type);
        
        const GLchar *Cstr = file.c_str();
        glShaderSource(shader, 1, &Cstr, NULL);
        
        glCompileShader(shader);
        
        GLint compiled;
        
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            logger.error << file << logger.end;
            logger.error << "Failed to compile shader!" << logger.end;
            GLsizei bufsize;
            const int maxBufSize = 100;
            char buffer[maxBufSize];
            glGetShaderInfoLog(shader, maxBufSize, &bufsize, buffer);
            logger.error << "compile errors: " << buffer << logger.end;            
        }
        
        
        return shader;
    }
    
    void OpenGLES2Shader::Unload() { THROW(); }
    
    
    
}
}
