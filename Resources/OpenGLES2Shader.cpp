
#include "OpenGLES2Shader.h"
#include <Meta/iOS.h>
#include <Resources/File.h>
#include <Resources/DirectoryManager.h>
#include <Logging/Logger.h>
#include <fstream>

namespace OpenEngine {
namespace Resources {

    using namespace std;
    
    OpenGLES2Shader::OpenGLES2Shader(string file) : filename(file) {
        
    }
    
    void OpenGLES2Shader::ApplyShader() {
        glUseProgram(programID);
    }
    void OpenGLES2Shader::ReleaseShader() { 
        glUseProgram(0);
    }
    
    void OpenGLES2Shader::SetUniform(string name, int arg, bool force) { 
        GLint loc = glGetUniformLocation(programID, name.c_str());
        glUniform1i(loc, arg);
    }
    void OpenGLES2Shader::SetUniform(string name, float value, bool force) { THROW(); }
    void OpenGLES2Shader::SetUniform(string name, Vector<2, float> value, bool force ) { THROW(); }
    void OpenGLES2Shader::SetUniform(string name, Vector<3, float> value, bool force ) { THROW(); }
    void OpenGLES2Shader::SetUniform(string name, Vector<4, float> value, bool force ) { THROW(); }
    void OpenGLES2Shader::SetUniform(string name, Matrix<4, 4, float> value, bool force ) { 
        float v[16];
        value.ToArray(v);
        GLint loc = glGetUniformLocation(programID, name.c_str());
        glUniformMatrix4fv(loc, 1, GL_FALSE, v);
    
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
        logger.error << "Attribute not found: " << name << logger.end;
        return loc;
    }
    void OpenGLES2Shader::Load() { 
        LoadResource(filename);
        GLuint vid = LoadShader(vertex_filename, GL_VERTEX_SHADER);
        GLuint fid = LoadShader(fragment_filename, GL_FRAGMENT_SHADER);
        
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
        GLint maxAt;
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAt);
        logger.info << "Max attributes " << maxAt << logger.end;
        
        GLuint shader = glCreateShader(type);
        
        string fname = DirectoryManager::FindFileInPath(file);
        
        GLchar *str = File::ReadShader<GLchar>(fname);
        
        const GLchar *Cstr = str;
        
        glShaderSource(shader, 1, &Cstr, NULL);
        free(str);
        
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
    
    void OpenGLES2Shader::LoadResource(string res) {
        ifstream* in = File::Open(res);
        
        
        char buf[255], file[255];
        int line = 0;
        
        while (!in->eof()) {
            ++line;
            in->getline(buf, 255);
            
            string type = string(buf,5);
            if (type.empty() || buf[0] == '#')
                continue;
            
            if (type == "vert:") {
                if (sscanf(buf, "vert: %s", file) == 1)
                    vertex_filename = string(file);                
            } else if (type == "frag:")
                if (sscanf(buf, "frag: %s", file) == 1)
                    fragment_filename = string(file);
        }
        
        in->close();
        delete in;
        
        
    }
    
    void OpenGLES2Shader::Unload() { THROW(); }
    
    
    
}
}