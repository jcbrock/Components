#include <iostream>
#include "GameObject.h"

#include "ObjectModel\MeshInstance.h"
#include "ObjectModel\RigidBody.h"

#include "RigidBodyManager.h"
#include "MeshInstanceManager.h"
#include "OpenGLManager.h"

#include <thread>
#include <chrono>

// for page size
#include <windows.h>

#include <glfw3.h>

//TODO - look at includes, think about dependencies
extern GLFWwindow* window;
RigidBodyManager rbm;
MeshInstanceManager mim;
OpenGLManager openGLm;

void UpdateSubsystems(float timeDelta)
{
    rbm.UpdateSubsystem(timeDelta);
    mim.UpdateSubsystem(timeDelta);
}

bool InitializeSubsystems(DWORD memoryPageSize)
{
    std::cout << "Memory page size on this system: " << memoryPageSize << std::endl;

    bool ok = true;

    ok = ok && openGLm.OpenGLInit();
    rbm.Initialize(memoryPageSize);
    mim.Initialize(memoryPageSize*5);

    return ok;
}

//Notes - todo, put somewhere else
//Memory management
//	Each subsystem (i.e. RigidBodyManager) will control the memory
//	management for all subsystem items (i.e. RigidBodies). Right now
//	I think each manager contains an array of the things, so that when we
//	update a subsystem, they all get updated. GameObjects will have pointers
//	to their specific rigidbody.

#pragma region Data
GLfloat leftPaddleData[] = {
    -3.0f, -1.0f, -1.0f,
    -3.0f, -1.0f, 1.0f,
    -3.0f, 1.0f, 1.0f,
    -2.5f, 1.0f, -1.0f,
    -3.0f, -1.0f, -1.0f,
    -3.0f, 1.0f, -1.0f,
    -2.5f, -1.0f, 1.0f,
    -3.0f, -1.0f, -1.0f,
    -2.5f, -1.0f, -1.0f,
    -2.5f, 1.0f, -1.0f,
    -2.5f, -1.0f, -1.0f,
    -3.0f, -1.0f, -1.0f,
    -3.0f, -1.0f, -1.0f,
    -3.0f, 1.0f, 1.0f,
    -3.0f, 1.0f, -1.0f,
    -2.5f, -1.0f, 1.0f,
    -3.0f, -1.0f, 1.0f,
    -3.0f, -1.0f, -1.0f,
    -3.0f, 1.0f, 1.0f,
    -3.0f, -1.0f, 1.0f,
    -2.5f, -1.0f, 1.0f,
    -2.5f, 1.0f, 1.0f,
    -2.5f, -1.0f, -1.0f,
    -2.5f, 1.0f, -1.0f,
    -2.5f, -1.0f, -1.0f,
    -2.5f, 1.0f, 1.0f,
    -2.5f, -1.0f, 1.0f,
    -2.5f, 1.0f, 1.0f,
    -2.5f, 1.0f, -1.0f,
    -3.0f, 1.0f, -1.0f,
    -2.5f, 1.0f, 1.0f,
    -3.0f, 1.0f, -1.0f,
    -3.0f, 1.0f, 1.0f,
    -2.5f, 1.0f, 1.0f,
    -3.0f, 1.0f, 1.0f,
    -2.5f, -1.0f, 1.0f
};
GLfloat rightPaddleData[] = {
    4.5f, -1.0f, -1.0f,
    4.5f, -1.0f, 1.0f,
    4.5f, 1.0f, 1.0f,
    5.0f, 1.0f, -1.0f,
    4.5f, -1.0f, -1.0f,
    4.5f, 1.0f, -1.0f,
    5.0f, -1.0f, 1.0f,
    4.5f, -1.0f, -1.0f,
    5.0f, -1.0f, -1.0f,
    5.0f, 1.0f, -1.0f,
    5.0f, -1.0f, -1.0f,
    4.5f, -1.0f, -1.0f,
    4.5f, -1.0f, -1.0f,
    4.5f, 1.0f, 1.0f,
    4.5f, 1.0f, -1.0f,
    5.0f, -1.0f, 1.0f,
    4.5f, -1.0f, 1.0f,
    4.5f, -1.0f, -1.0f,
    4.5f, 1.0f, 1.0f,
    4.5f, -1.0f, 1.0f,
    5.0f, -1.0f, 1.0f,
    5.0f, 1.0f, 1.0f,
    5.0f, -1.0f, -1.0f,
    5.0f, 1.0f, -1.0f,
    5.0f, -1.0f, -1.0f,
    5.0f, 1.0f, 1.0f,
    5.0f, -1.0f, 1.0f,
    5.0f, 1.0f, 1.0f,
    5.0f, 1.0f, -1.0f,
    4.5f, 1.0f, -1.0f,
    5.0f, 1.0f, 1.0f,
    4.5f, 1.0f, -1.0f,
    4.5f, 1.0f, 1.0f,
    5.0f, 1.0f, 1.0f,
    4.5f, 1.0f, 1.0f,
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
#pragma endregion


void Draw(
    const glm::mat4& mvp,
    const Buffer& vertexBuffer,
    const Buffer&  uvBuffer,
    const GLuint& textureDataHandle)
{
    //constants - handles to shader inputs

    //mixed - buffer data - data is constant, I just need to do a lookup per shape for the right buffer

    //SETUP VERTEX SHADER INPUTS


    // Send our transformation to the currently bound shader, 
    // in the "MVP" uniform

    glUniformMatrix4fv(openGLm.mMVPMatrixInputHandle, 1, GL_FALSE, &mvp[0][0]);

    // 1rst attribute buffer : vertices


    glEnableVertexAttribArray(openGLm.mVertexInputHandle);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.mBufferHandle);
    glVertexAttribPointer(
        openGLm.mVertexInputHandle,           // The attribute we want to configure
        3,                            // size
        GL_FLOAT,                     // type
        GL_FALSE,                     // normalized?
        0,                            // stride
        (void*)0                      // array buffer offset
        );

    // 2nd attribute buffer : UVs
    glEnableVertexAttribArray(openGLm.mUVCoordinateInputHandle);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer.mBufferHandle);
    glVertexAttribPointer(
        openGLm.mUVCoordinateInputHandle,                   // The attribute we want to configure
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
    glUniform1i(openGLm.mTextureInputHandle, 0);
    //END FRAG


    // Draw the triangles !
    glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles

    glDisableVertexAttribArray(openGLm.mVertexInputHandle);
}

void Draw(MeshInstance* meshInstance, RigidBody* rigidBody)
{
    Draw(rigidBody->mMVPForScene, meshInstance->mVertices, meshInstance->mUVBuffer, meshInstance->mTextureHandle);
}

void SetupGameObject(GameObject& obj,
    int vertDataSize,
    GLfloat* vertData,
    int uvDataSize,
    GLfloat* uvData,
    GLuint textureHandle)
{
    // SETUP RIGID BODIES

    RigidBody* objRB = rbm.CreateRigidBody();
    objRB->mName = obj.GetName() + "RB"; //TODO - don't do plus
    obj.SetRigidBodyComponent(objRB);

    // MESH INSTANCE

    MeshInstance* objMI = mim.CreateMeshInstance();
    objMI->SetName(obj.GetName() + "MI"); //TODO - don't do plus
    objMI->mVertices.Initialize(vertDataSize, vertData);
    objMI->mUVBuffer.Initialize(uvDataSize, uvData);
    objMI->mTextureHandle = textureHandle;
    obj.SetMeshInstanceComponent(objMI);
}

int main()
{
    // INIT SUBSYSTEMS - OpenGL, RigidyBodyManager, MeshInstanceManager

    SYSTEM_INFO si;
    GetSystemInfo(&si);

    InitializeSubsystems(si.dwPageSize);

    // SETUP GAME OBJECTS

    GameObject leftPaddle("LeftPaddle");
    GameObject rightPaddle("RightPaddle");
    GameObject ball("Ball");

    SetupGameObject(leftPaddle, sizeof(leftPaddleData), leftPaddleData, sizeof(g_uv_buffer_data), g_uv_buffer_data, openGLm.mTexture);
    SetupGameObject(rightPaddle, sizeof(rightPaddleData), rightPaddleData, sizeof(g_uv_buffer_data), g_uv_buffer_data, openGLm.mTexture);
    SetupGameObject(ball, sizeof(ballData), ballData, sizeof(g_uv_buffer_data), g_uv_buffer_data, openGLm.mTexture);

    leftPaddle.DebugPrint();
    rightPaddle.DebugPrint();
    ball.DebugPrint();
    rbm.DebugPrint();
    mim.DebugPrint();
    //mim.DestroyMeshInstance(0);
    //mim.DebugPrint();

    // ALL SETUP - ENTER LOOP

    bool endGameLoop = false;
    float timeDelta = 1;
    reinterpret_cast<RigidBody*>(ball.GetRigidBodyComponent())->mDirection = glm::vec4(-1, 0, 0, 0);
    while (!endGameLoop)
    {
        
        timeDelta += .1;
        std::cout << "Timedelta: " << timeDelta << std::endl;
        rbm.DebugPrint();


        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Update subsystems
        UpdateSubsystems(timeDelta);

        // How do I handle drawing? Is that part updating MeshInstance Subysystem? or is that responsbility just compute MVPs?
        Draw(reinterpret_cast<MeshInstance*>(ball.GetMeshInstanceComponent()), reinterpret_cast<RigidBody*>(ball.GetRigidBodyComponent()));
        Draw(reinterpret_cast<MeshInstance*>(leftPaddle.GetMeshInstanceComponent()), reinterpret_cast<RigidBody*>(leftPaddle.GetRigidBodyComponent()));
        Draw(reinterpret_cast<MeshInstance*>(rightPaddle.GetMeshInstanceComponent()), reinterpret_cast<RigidBody*>(rightPaddle.GetRigidBodyComponent()));

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

        std::this_thread::sleep_for(std::chrono::milliseconds(5000));

        //endGameLoop = true;
    }

    //TODO - RELEASE STUFF

    system("pause");
    return 0;
}