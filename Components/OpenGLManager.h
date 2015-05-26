#ifndef OPEN_GL_MANAGER_H
#define OPEN_GL_MANAGER_H

#include <GL/glew.h>
#include <glm/glm.hpp> //for glm::mat4& mvp,

class OpenGLManager
{

public:
    //void Initialize(unsigned int maxSize);
    void  Draw(
        const glm::mat4& mvp,
        const GLuint& vertexBufferHandle,
        const GLuint&  uvBufferHandle,
        const GLuint& textureDataHandle);

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

private:
    void UpdateVertexShader(const glm::mat4& mvp,
        const GLuint& vertexBufferHandle,
        const GLuint&  uvBufferHandle);
    void UpdateFragmentShader(const GLuint& textureDataHandle);
};

#endif