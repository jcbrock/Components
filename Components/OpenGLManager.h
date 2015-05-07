#ifndef OPEN_GL_MANAGER_H
#define OPEN_GL_MANAGER_H

#include <GL/glew.h>

class OpenGLManager
{

public:
    //void Initialize(unsigned int maxSize);

    bool OpenGLInit();
    void InitOtherStuff();

    //In shader.cpp
    GLuint LoadProgramShaders(const char * vertex_file_path, const char * fragment_file_path);
    //In texture.cpp
    GLuint LoadProgramDDS(const char * imagePath);

    GLuint programID = -1;
    GLuint mMVPMatrixInputHandle = -1;
    GLuint mVertexInputHandle = -1;
    GLuint mUVCoordinateInputHandle = -1;
    GLuint mTextureInputHandle = -1;
    GLuint mTexture = -1;
    };

#endif