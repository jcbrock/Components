#include "OpenGLManager.h"

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <common/shader.hpp>
#include <common/texture.hpp>

// Include GLM
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);

void OpenGLManager::InitOtherStuff()
{
    programID = LoadProgramShaders("Shaders/TransformVertexShader.vertexshader", "Shaders/TextureFragmentShader.fragmentshader");
    // Use our shader
    glUseProgram(programID);
    mTexture = LoadProgramDDS("uvtemplate.DDS");
    mMVPMatrixInputHandle = glGetUniformLocation(programID, "MVP");
    mVertexInputHandle = glGetAttribLocation(programID, "vertexPosition_modelspace");
    mUVCoordinateInputHandle = glGetAttribLocation(programID, "vertexUV");
    mTextureInputHandle = glGetUniformLocation(programID, "myTextureSampler");

}
bool OpenGLManager::OpenGLInit()
{

    // Initialise GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return false;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);


    // Open a window and create its OpenGL context
    window = glfwCreateWindow(1024, 768, "Tutorial 05 - Textured Cube", NULL, NULL);
    if (window == NULL){
        fprintf(stderr, "Failed to open GLFW window.\n");
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return false;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    InitOtherStuff();
    return true;
}

GLuint OpenGLManager::LoadProgramShaders(const char * vertex_file_path, const char * fragment_file_path)
{
   return LoadShaders(vertex_file_path, fragment_file_path);
}

GLuint OpenGLManager::LoadProgramDDS(const char * imagePath)
{
    return loadDDS(imagePath);
}

void OpenGLManager::Draw(
    const glm::mat4& mvp,
    const GLuint& vertexBufferHandle,
    const GLuint&  uvBufferHandle,
    const GLuint& textureDataHandle)
{
    UpdateVertexShader(mvp, vertexBufferHandle, uvBufferHandle);
    UpdateFragmentShader(textureDataHandle);

    // Draw the triangles !
    glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles

    glDisableVertexAttribArray(mVertexInputHandle);

    glBindBuffer(GL_ARRAY_BUFFER, 0); //clear the binding
}

void OpenGLManager::UpdateVertexShader(
    const glm::mat4& mvp,
    const GLuint& vertexBufferHandle,
    const GLuint&  uvBufferHandle)
{
    //constants - handles to shader inputs

    //mixed - buffer data - data is constant, I just need to do a lookup per shape for the right buffer

    //SETUP VERTEX SHADER INPUTS

    // Send our transformation to the currently bound shader, 
    // in the "MVP" uniform
    glUniformMatrix4fv(mMVPMatrixInputHandle, 1, GL_FALSE, &mvp[0][0]);

    // 1rst attribute buffer : vertices

    glEnableVertexAttribArray(mVertexInputHandle);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);
    glVertexAttribPointer(
        mVertexInputHandle,           // The attribute we want to configure
        3,                            // size
        GL_FLOAT,                     // type
        GL_FALSE,                     // normalized?
        0,                            // stride
        (void*)0                      // array buffer offset
        );

    // 2nd attribute buffer : UVs
    glEnableVertexAttribArray(mUVCoordinateInputHandle);
    glBindBuffer(GL_ARRAY_BUFFER, uvBufferHandle);
    glVertexAttribPointer(
        mUVCoordinateInputHandle,                   // The attribute we want to configure
        2,                            // size : U+V => 2
        GL_FLOAT,                     // type
        GL_FALSE,                     // normalized?
        0,                            // stride
        (void*)0                      // array buffer offset
        );
}

void OpenGLManager::UpdateFragmentShader(const GLuint& textureDataHandle)
{
    //SETUP FRAGMENT SHADER INPUTS
    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureDataHandle);
    // Set our "myTextureSampler" sampler to user Texture Unit 0
    glUniform1i(mTextureInputHandle, 0);
    //END FRAG

    // Draw the triangles !
    glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles

    glDisableVertexAttribArray(mVertexInputHandle);

    glBindBuffer(GL_ARRAY_BUFFER, 0); //clear the binding
}