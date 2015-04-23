// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>
extern GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//#include <common/shader.hpp>
//#include <common/texture.hpp>

// Include GLM
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

#include <iostream>
#include <mutex>
#include <array>
#include <memory>
#include <unordered_map>
#include <string>

#include "common\texture.hpp"
#include "common\shader.hpp"
#include "GameObject2.h"
#include "Buffer.h"

bool OpenGLInit();

//Only do an event if a character is facing a certain direction. Do this by comparing vectors:
//Just testing dot product to see if the eye to point / normal vectors are facing each other or away
bool VecAngleTest(float maxAngle, glm::vec3 v1, glm::vec3 v2)
{
    //so when 2 vectors are facing the same direction, the dot product is one
    if (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z > .95) //can't be above 1, right? so this is fine I think
    {
        return true;
    }
    return false;
}

void windowToObjectf(const glm::vec3& windowCoord, GLdouble * x, GLdouble * y, GLdouble * z) {
    GLdouble mv[16];
    glGetDoublev(GL_PROJECTION_MATRIX, mv);
    GLdouble p[16];
    glGetDoublev(GL_PROJECTION_MATRIX, p);


    glm::mat4 modelViewMatrix;
    glm::mat4 projectionMatrix;
    std::array <GLint, 4> viewport;
    glGetFloatv(GL_MODELVIEW_MATRIX, glm::value_ptr(modelViewMatrix));
    glGetFloatv(GL_PROJECTION_MATRIX, glm::value_ptr(projectionMatrix));
    glGetIntegerv(GL_VIEWPORT, &viewport.front());

    //glm::vec3 ret(0, 0, 0);
    //float* ret = new float[3];
    //  GLdouble* foo = glm::value_ptr(modelViewMatrix);
    int success = gluUnProject(windowCoord.x, windowCoord.y, windowCoord.z, mv, p, &viewport.front(), x, y, z);
    //RASSERT(succes == GL_TRUE);
    //GL_RASSERT();
    std::cout << "success: " << success << std::endl;

    //return ret;
}


void onMouseButton(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {

        double* xpos = new double();
        double* ypos = new double();

        glfwGetCursorPos(window, xpos, ypos);
        // ... some code
        std::cout << "Right click! (" << *xpos << ", " << *ypos << ")" << std::endl;

        // glm::vec3 windowPos(1.0, 1.0, 0);
        float xf = (float)(*xpos);
        float yf = (float)(*ypos);
        //float yf = (float)std::numeric_limits<double>::infinity();
        glm::vec3 windowPos(xf, yf, 0);
        GLdouble * coords = new GLdouble[3];
        windowToObjectf(windowPos, &coords[0], &coords[1], &coords[2]);
        //Ok, how am I going to define my scene? i.e. how do I make opengl match what my internal data is?
        std::cout << "windowToObjectf returned: (" << coords[0] << ", " << coords[1] << ", " << coords[2] << ")" << std::endl;
        delete coords;
        delete xpos;
        delete ypos;
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        double* xpos = new double();
        double* ypos = new double();

        glfwGetCursorPos(window, xpos, ypos);
        // ... some code
        std::cout << "Left click! (" << *xpos << ", " << *ypos << ")" << std::endl;

        delete xpos;
        delete ypos;
    }

}

/*
glm::vec3 splitVector(glm::vec3 point1, glm::vec3 point2)
{
return glm::vec3((point2.x - point1.x) / 2, (point2.y - point1.y) / 2, (point2.z - point1.z) / 2);

}
*/

struct ShaderVariables
{
    // Create and compile our GLSL program from the shaders
    //GLuint programID = LoadShaders("TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader");
};

// One of these for the entire program (handles to vertex inputs aren't going to change, regardless of object)
class VertexShaderAttributeHandleManager
{
public:
    //INPUTS INTO VERTEX SHADER
    GLuint mMVPMatrixInputHandle;// = glGetUniformLocation(programID, "MVP"); //uniform mat4 (same for each vertex)
    GLuint mVertexInputHandle;// = glGetAttribLocation(programID, "vertexPosition_modelspace"); //attribute vec3 (different - per vertex obviously)
    GLuint mUVCoordinateInputHandle;// = glGetAttribLocation(programID, "vertexUV"); //attribute vec2 (different - UV coordinate for current vertex)

    static VertexShaderAttributeHandleManager& Instance()
    {
        static VertexShaderAttributeHandleManager mInstance;
        return mInstance;
    }

private:
    VertexShaderAttributeHandleManager() {};
    ~VertexShaderAttributeHandleManager() {};
    VertexShaderAttributeHandleManager(const VertexShaderAttributeHandleManager&) = delete;
    //TODO - delete =, and move funcs
};

// One of these for the entire program (handles to vertex inputs aren't going to change, regardless of object)
class FragmentShaderAttributeHandleManager
{
public:
    //INPUTS INTO FRAGMENT SHADER
    //fragment shader gets UV coords from vertex shader - varying vec2 UV;
    //we need to pass in texture that gets sampled from though (obviously stays same for all fragments)
    //data comes from elsewhere, this is just a handle to the myTextureSampler shader input
    GLuint mTextureInputHandle;// = glGetUniformLocation(programID, "myTextureSampler"); //uniform sampler2D myTextureSampler;

    static FragmentShaderAttributeHandleManager& Instance()
    {
        static FragmentShaderAttributeHandleManager mInstance;
        return mInstance;
    }

private:
    FragmentShaderAttributeHandleManager() {};
    ~FragmentShaderAttributeHandleManager() {};
    FragmentShaderAttributeHandleManager(const FragmentShaderAttributeHandleManager&) = delete;
    //TODO - delete =, and move funcs
};

std::unordered_map<std::string, GLuint> textures; //texture name, textureHandle <- handle it is bound to in LoadDDS

//vector of (vertextBufferData or uvBufferData, gl_array_buffer_id it is bound to)
//could be reused by objects - how to bind to an shape? shape could store index into vector
//or I could have a key, like a name (or ID), and maybe the above vector is a hashtable?
//  yeah - i'll do a name for now, but keep that shit UNIQUE.

void BindBuffersToData(const std::string name, const std::unique_ptr<Buffer>& buffer)
{
    std::cout << "Binding buffer for: " << name << std::endl;

    glBindBuffer(GL_ARRAY_BUFFER, buffer->mBufferHandle); //binds vertexbuffer variable to GL_ARRAY_BUFFER. Future opengl calls will operate on this buffer
    glBufferData(GL_ARRAY_BUFFER, buffer->mBufferSize, buffer->mBufferData, GL_STATIC_DRAW); //do shit to bound buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0); //clear the binding, so we don't accendently make modifcations to the bound buffer
}

void Draw(
    const glm::mat4& mvp,
    const std::unique_ptr<Buffer>& vertexBuffer,
    const std::unique_ptr<Buffer>&  uvBuffer,
    const GLuint& textureDataHandle)
{
    //constants - handles to shader inputs

    //mixed - buffer data - data is constant, I just need to do a lookup per shape for the right buffer

    //changes per shape - MVP

    //SETUP VERTEX SHADER INPUTS

    // Send our transformation to the currently bound shader, 
    // in the "MVP" uniform

    glUniformMatrix4fv(VertexShaderAttributeHandleManager::Instance().mMVPMatrixInputHandle, 1, GL_FALSE, &mvp[0][0]);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(VertexShaderAttributeHandleManager::Instance().mVertexInputHandle);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->mBufferHandle);
    glVertexAttribPointer(
        VertexShaderAttributeHandleManager::Instance().mVertexInputHandle,           // The attribute we want to configure
        3,                            // size
        GL_FLOAT,                     // type
        GL_FALSE,                     // normalized?
        0,                            // stride
        (void*)0                      // array buffer offset
        );

    // 2nd attribute buffer : UVs
    glEnableVertexAttribArray(VertexShaderAttributeHandleManager::Instance().mUVCoordinateInputHandle);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer->mBufferHandle);
    glVertexAttribPointer(
        VertexShaderAttributeHandleManager::Instance().mUVCoordinateInputHandle,                   // The attribute we want to configure
        2,                            // size : U+V => 2
        GL_FLOAT,                     // type
        GL_FALSE,                     // normalized?
        0,                            // stride
        (void*)0                      // array buffer offset
        );

    //SETUP FRAGMENT SHADER INPUTS
    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureDataHandle);
    // Set our "myTextureSampler" sampler to user Texture Unit 0
    glUniform1i(FragmentShaderAttributeHandleManager::Instance().mTextureInputHandle, 0);
    //END FRAG


    // Draw the triangles !
    glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles

    glDisableVertexAttribArray(VertexShaderAttributeHandleManager::Instance().mVertexInputHandle);
    //glDisableVertexAttribArray(vertShaderAttributeHandles.mUVCoordinate);
}

int foo2()
{
    /*if (!OpenGLInit())
    {
        return -1;
    }*/
    
    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders("TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader");

    // Get a handle for our "MVP" uniform, vertex/uv inputs to vertext shader
    VertexShaderAttributeHandleManager::Instance().mMVPMatrixInputHandle = glGetUniformLocation(programID, "MVP");
    VertexShaderAttributeHandleManager::Instance().mVertexInputHandle = glGetAttribLocation(programID, "vertexPosition_modelspace");
    VertexShaderAttributeHandleManager::Instance().mUVCoordinateInputHandle = glGetAttribLocation(programID, "vertexUV");

    // Get a handle for our "myTextureSampler" uniform
    FragmentShaderAttributeHandleManager::Instance().mTextureInputHandle = glGetUniformLocation(programID, "myTextureSampler");

    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    // Camera matrix
    //glm::vec3 cameraPos = glm::vec3(4, 3, 3);
    glm::vec3 cameraPos = glm::vec3(0, 0, 10);
    glm::mat4 View = glm::lookAt(
        cameraPos, // Camera is at (4,3,3), in World Space
        glm::vec3(0, 0, 0), // and looks at the origin
        glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
        );
    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model = glm::mat4(1.0f);
    // Our ModelViewProjection : multiplication of our 3 matrices
    glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

    // Load the texture using any two methods
    //GLuint Texture = loadBMP_custom("uvtemplate.bmp");
    GLuint Texture = loadDDS("uvtemplate.DDS");



    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    GLfloat leftPaddleData[] = {
        -3.0f, -1.0f, -1.0f,
        -3.0f, -1.0f, 1.0f,
        -3.0f, 1.0f, 1.0f,
        -2.0f, 1.0f, -1.0f,
        -3.0f, -1.0f, -1.0f,
        -3.0f, 1.0f, -1.0f,
        -2.0f, -1.0f, 1.0f,
        -3.0f, -1.0f, -1.0f,
        -2.0f, -1.0f, -1.0f,
        -2.0f, 1.0f, -1.0f,
        -2.0f, -1.0f, -1.0f,
        -3.0f, -1.0f, -1.0f,
        -3.0f, -1.0f, -1.0f,
        -3.0f, 1.0f, 1.0f,
        -3.0f, 1.0f, -1.0f,
        -2.0f, -1.0f, 1.0f,
        -3.0f, -1.0f, 1.0f,
        -3.0f, -1.0f, -1.0f,
        -3.0f, 1.0f, 1.0f,
        -3.0f, -1.0f, 1.0f,
        -2.0f, -1.0f, 1.0f,
        -2.0f, 1.0f, 1.0f,
        -2.0f, -1.0f, -1.0f,
        -2.0f, 1.0f, -1.0f,
        -2.0f, -1.0f, -1.0f,
        -2.0f, 1.0f, 1.0f,
        -2.0f, -1.0f, 1.0f,
        -2.0f, 1.0f, 1.0f,
        -2.0f, 1.0f, -1.0f,
        -3.0f, 1.0f, -1.0f,
        -2.0f, 1.0f, 1.0f,
        -3.0f, 1.0f, -1.0f,
        -3.0f, 1.0f, 1.0f,
        -2.0f, 1.0f, 1.0f,
        -3.0f, 1.0f, 1.0f,
        -2.0f, -1.0f, 1.0f
    };
    GLfloat rightPaddleData[] = {
        3.0f, -1.0f, -1.0f,
        3.0f, -1.0f, 1.0f,
        3.0f, 1.0f, 1.0f,
        5.0f, 1.0f, -1.0f,
        3.0f, -1.0f, -1.0f,
        3.0f, 1.0f, -1.0f,
        5.0f, -1.0f, 1.0f,
        3.0f, -1.0f, -1.0f,
        5.0f, -1.0f, -1.0f,
        5.0f, 1.0f, -1.0f,
        5.0f, -1.0f, -1.0f,
        3.0f, -1.0f, -1.0f,
        3.0f, -1.0f, -1.0f,
        3.0f, 1.0f, 1.0f,
        3.0f, 1.0f, -1.0f,
        5.0f, -1.0f, 1.0f,
        3.0f, -1.0f, 1.0f,
        3.0f, -1.0f, -1.0f,
        3.0f, 1.0f, 1.0f,
        3.0f, -1.0f, 1.0f,
        5.0f, -1.0f, 1.0f,
        5.0f, 1.0f, 1.0f,
        5.0f, -1.0f, -1.0f,
        5.0f, 1.0f, -1.0f,
        5.0f, -1.0f, -1.0f,
        5.0f, 1.0f, 1.0f,
        5.0f, -1.0f, 1.0f,
        5.0f, 1.0f, 1.0f,
        5.0f, 1.0f, -1.0f,
        3.0f, 1.0f, -1.0f,
        5.0f, 1.0f, 1.0f,
        3.0f, 1.0f, -1.0f,
        3.0f, 1.0f, 1.0f,
        5.0f, 1.0f, 1.0f,
        3.0f, 1.0f, 1.0f,
        5.0f, -1.0f, 1.0f
    };
    GLfloat ballData[] = {
        1.5f, -0.5, -1.0f,
        1.5f, -0.5f, 1.0f,
        1.5f, 0.5f, 1.0f,
        2.0f, 0.5f, -1.0f,
        1.5f, -0.5f, -1.0f,
        1.5f, 0.5f, -1.0f,
        2.0f, -0.5f, 1.0f,
        1.5f, -0.5f, -1.0f,
        2.0f, -0.5f, -1.0f,
        2.0f, 0.5f, -1.0f,
        2.0f, -0.5f, -1.0f,
        1.5f, -0.5f, -1.0f,
        1.5f, -0.5f, -1.0f,
        1.5f, 0.5f, 1.0f,
        1.5f, 0.5f, -1.0f,
        2.0f, -0.5f, 1.0f,
        1.5f, -0.5f, 1.0f,
        1.5f, -0.5f, -1.0f,
        1.5f, 0.5f, 1.0f,
        1.5f, -0.5f, 1.0f,
        2.0f, -0.5f, 1.0f,
        2.0f, 0.5f, 1.0f,
        2.0f, -0.5f, -1.0f,
        2.0f, 0.5f, -1.0f,
        2.0f, -0.5f, -1.0f,
        2.0f, 0.5f, 1.0f,
        2.0f, -0.5f, 1.0f,
        2.0f, 0.5f, 1.0f,
        2.0f, 0.5f, -1.0f,
        1.5f, 0.5f, -1.0f,
        2.0f, 0.5f, 1.0f,
        1.5f, 0.5f, -1.0f,
        1.5f, 0.5f, 1.0f,
        2.0f, 0.5f, 1.0f,
        1.5f, 0.5f, 1.0f,
        2.0f, -0.5f, 1.0f
    };
    // Two UV coordinatesfor each vertex. They were created withe Blender.
    GLfloat g_uv_buffer_data[] = {
        0.000059f, 1.0f - 0.000004f,
        0.000103f, 1.0f - 0.336048f,
        0.335973f, 1.0f - 0.335903f,
        1.000023f, 1.0f - 0.000013f,
        0.667979f, 1.0f - 0.335851f,
        0.999958f, 1.0f - 0.336064f,
        0.667979f, 1.0f - 0.335851f,
        0.336024f, 1.0f - 0.671877f,
        0.667969f, 1.0f - 0.671889f,
        1.000023f, 1.0f - 0.000013f,
        0.668104f, 1.0f - 0.000013f,
        0.667979f, 1.0f - 0.335851f,
        0.000059f, 1.0f - 0.000004f,
        0.335973f, 1.0f - 0.335903f,
        0.336098f, 1.0f - 0.000071f,
        0.667979f, 1.0f - 0.335851f,
        0.335973f, 1.0f - 0.335903f,
        0.336024f, 1.0f - 0.671877f,
        1.000004f, 1.0f - 0.671847f,
        0.999958f, 1.0f - 0.336064f,
        0.667979f, 1.0f - 0.335851f,
        0.668104f, 1.0f - 0.000013f,
        0.335973f, 1.0f - 0.335903f,
        0.667979f, 1.0f - 0.335851f,
        0.335973f, 1.0f - 0.335903f,
        0.668104f, 1.0f - 0.000013f,
        0.336098f, 1.0f - 0.000071f,
        0.000103f, 1.0f - 0.336048f,
        0.000004f, 1.0f - 0.671870f,
        0.336024f, 1.0f - 0.671877f,
        0.000103f, 1.0f - 0.336048f,
        0.336024f, 1.0f - 0.671877f,
        0.335973f, 1.0f - 0.335903f,
        0.667969f, 1.0f - 0.671889f,
        1.000004f, 1.0f - 0.671847f,
        0.667979f, 1.0f - 0.335851f
    };

    std::list<GameObject2*> worldGameObjects;

    //TODO - I need to handle delta time at some point

    GameObject2 leftPaddleGO;
    leftPaddleGO.InitializeMeshInstance(sizeof(leftPaddleData), &leftPaddleData[0], sizeof(g_uv_buffer_data), &g_uv_buffer_data[0], Texture);
    leftPaddleGO.InitializeRigidBodyInstance(1.0f, 2.0f, glm::vec4(-2.5, 0, 0, 1), glm::vec4(0, 1.0f, 0, 0), 0.0f);
    leftPaddleGO.GetRigidBody()->mName = "leftPaddle";

    GameObject2 rightPaddleGO;
    rightPaddleGO.InitializeMeshInstance(sizeof(rightPaddleData), &rightPaddleData[0], sizeof(g_uv_buffer_data), &g_uv_buffer_data[0], Texture);
    rightPaddleGO.InitializeRigidBodyInstance(1.0f, 2.0f, glm::vec4(4.0f, 0, 0, 1), glm::vec4(0, 1.0f, 0, 0), 0.0f);
    rightPaddleGO.GetRigidBody()->mName = "rightPaddle";

    //go.mMeshInstance->mVertices = std::make_unique<Buffer>(sizeof(ballData), &ballData[0]);
    //go.mMeshInstance->mUVBuffer = std::make_unique<Buffer>(sizeof(g_uv_buffer_data), &g_uv_buffer_data[0]);

    //oh mother fucker - I'm creating a Buffer, then doing a copy I think - copy constructor
    //I didn't define one for Buffer, so it is just doing a bitwise copy - copies the pointer and shit,
    //but when the temp gets deleted, the pointer still exists, but points to garbage. DAMMNIT
    //go.mMeshInstance->mVertices = std::make_unique<Buffer>(Buffer(sizeof(ballData), &ballData[0]));        

    //I really need to work out of some source control or something
    // BindBuffersToData2("Ball vertices", ballGO.GetMeshInstance()->mVertices);
    //BindBuffersToData2("Ball UV mapping", ballGO.GetMeshInstance()->mUVBuffer);

    GameObject2 ballGO;
    ballGO.InitializeMeshInstance(sizeof(ballData), &ballData[0], sizeof(g_uv_buffer_data), &g_uv_buffer_data[0], Texture);
    ballGO.InitializeRigidBodyInstance(1.0f, 2.0f, glm::vec4(1.75, 0, 0, 1), glm::vec4(-1.0f, 0, 0, 0), 0.005f);
    ballGO.GetRigidBody()->mName = "ball";

    worldGameObjects.push_back(&leftPaddleGO);
    worldGameObjects.push_back(&rightPaddleGO);
    worldGameObjects.push_back(&ballGO);

    for (const auto& obj : worldGameObjects)
    {
        BindBuffersToData(obj->GetRigidBody()->mName + " vertices", obj->GetMeshInstance()->mVertices);
        BindBuffersToData(obj->GetRigidBody()->mName + " uv mapping", obj->GetMeshInstance()->mUVBuffer);
    }

    std::mutex mutex;
    std::unique_lock<std::mutex> lock(mutex);

    //KeyboardInput ki;
    //std::thread mBackgroundWorkerKeyboard(&KeyboardInput::Loop, std::ref(ki), window);

   /* std::unique_ptr<GameRuleset> rules(new PongNormalRuleset());
    MovementManager::Instance().Initialize(rules, md);
    for (GameObject2* obj : worldGameObjects)
    {
        MovementManager::Instance().worldGameObjects[obj->GetRigidBody()->mName] = obj;
    }*/


    bool locked = true;
    unsigned long long loopCount = 0;
    std::chrono::steady_clock::time_point tp = std::chrono::steady_clock::now();


    glm::mat4 ballTranslationMatrix = glm::mat4(1.0f);
    glm::mat4 leftPaddleTranslationMatrix = glm::mat4(1.0f);
    glm::mat4 rightPaddleTranslationMatrix = glm::mat4(1.0f);
    do{
        MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around


        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
        glUseProgram(programID);

        leftPaddleTranslationMatrix = glm::translate(
            leftPaddleTranslationMatrix,
            glm::vec3(leftPaddleGO.GetRigidBody()->mDirection * leftPaddleGO.GetRigidBody()->mSpeed));

        leftPaddleGO.GetMeshInstance()->mMVPForScene = Projection * View * leftPaddleTranslationMatrix;
        leftPaddleGO.GetRigidBody()->mPositionWorldCoord += leftPaddleGO.GetRigidBody()->mDirection * leftPaddleGO.GetRigidBody()->mSpeed;

        rightPaddleTranslationMatrix = glm::translate(
            rightPaddleTranslationMatrix,
            glm::vec3(rightPaddleGO.GetRigidBody()->mDirection * rightPaddleGO.GetRigidBody()->mSpeed));

        rightPaddleGO.GetMeshInstance()->mMVPForScene = Projection * View * rightPaddleTranslationMatrix;
        rightPaddleGO.GetRigidBody()->mPositionWorldCoord += rightPaddleGO.GetRigidBody()->mDirection * rightPaddleGO.GetRigidBody()->mSpeed;

        ballTranslationMatrix = glm::translate(
            ballTranslationMatrix,
            glm::vec3(ballGO.GetRigidBody()->mDirection * ballGO.GetRigidBody()->mSpeed));

        ballGO.GetMeshInstance()->mMVPForScene = Projection * View * ballTranslationMatrix;
        ballGO.GetRigidBody()->mPositionWorldCoord += ballGO.GetRigidBody()->mDirection * ballGO.GetRigidBody()->mSpeed;

      

        Draw(ballGO.GetMeshInstance()->mMVPForScene, ballGO.GetMeshInstance()->mVertices, ballGO.GetMeshInstance()->mUVBuffer, ballGO.GetMeshInstance()->mTextureHandle);
        Draw(leftPaddleGO.GetMeshInstance()->mMVPForScene, leftPaddleGO.GetMeshInstance()->mVertices, leftPaddleGO.GetMeshInstance()->mUVBuffer, leftPaddleGO.GetMeshInstance()->mTextureHandle);
        Draw(rightPaddleGO.GetMeshInstance()->mMVPForScene, rightPaddleGO.GetMeshInstance()->mVertices, rightPaddleGO.GetMeshInstance()->mUVBuffer, rightPaddleGO.GetMeshInstance()->mTextureHandle);


        //if (CollisionEngine::Instance().DetectAndResolveCollisions(ballGO, worldGameObjects))
        {
            //TODO - I'm not doing any jitter checks, wouldn't surprise
            //me if it collided with the same obect over and over at some point
            //might need to bump it in the other direction a bit to get it out
            //of "inside" the paddle
            //std::cout << "Collision detected " << std::endl;
            //updater.WakeUp();
        }


        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();


        // ++loopCount;
    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
    glfwWindowShouldClose(window) == 0);

    // Cleanup VBO and shader
    for (const auto& obj : worldGameObjects)
    {
        glDeleteBuffers(1, &obj->GetMeshInstance()->mVertices->mBufferHandle);
        glDeleteBuffers(1, &obj->GetMeshInstance()->mVertices->mBufferHandle);
    }

    glDeleteProgram(programID);
    glDeleteTextures(1, &FragmentShaderAttributeHandleManager::Instance().mTextureInputHandle);

    //delete sphereVerts;
    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}