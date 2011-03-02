#ifndef _OPENENGINE_OPENGLES_H_
#define _OPENENGINE_OPENGLES_H_

#include <OpenGLES/ES2/gl.h>

#include <Core/Exceptions.h>
#include <Utils/Convert.h>
#include <string>

using OpenEngine::Core::Exception;
using OpenEngine::Utils::Convert;
using std::string;


/**
 *  Should never be used in the code, use CHECK_FOR_GLES2_ERROR(); instead
 */
inline void CHECK_FOR_GLES2_ERROR(const std::string file, const int line) {
    GLenum errorCode = glGetError();
    if (errorCode != GL_NO_ERROR) {
        
        throw Exception("[file:" + file +
                        " line:" + Convert::ToString(line) +
                        "] OpenGLES Error: " + Convert::ToString(errorCode));

    }
}

#define CHECK_FOR_GLES2_ERROR(); CHECK_FOR_GLES2_ERROR(__FILE__,__LINE__);

#endif
