#include <iostream>
#include "GameObject.h"

#include "ObjectModel\MeshInstance.h"
#include "ObjectModel\RigidBody.h"

#include "RigidBodyManager.h"
#include "MeshInstanceManager.h"
#include "OpenGLManager.h"

#include "Timer.h"

#include <thread>
#include <chrono>

// for page size / vsync stuff
#include <windows.h>

// vsync stuff
//#include "wglext.h"
#include <GL\wglew.h>

#include <glfw3.h>

//TODO - look at includes, think about dependencies
extern GLFWwindow* window;
RigidBodyManager rbm;
MeshInstanceManager mim;
OpenGLManager openGLm;
unsigned int framesUntilPrint = 100;
Timer timer;

class FrameTimeRunningAvg
{
public:
    FrameTimeRunningAvg(uint8_t maxNumOfElements);
    ~FrameTimeRunningAvg();

    //other constructors?
    FrameTimeRunningAvg() = delete;
    FrameTimeRunningAvg(const FrameTimeRunningAvg&) = delete;
    // FrameTimeRunningAvg&(FrameTimeRunningAvg&&) = delete;

    void AddElement(__int64 element);
    double GetRunningAvg() const;

private:
    double mRunningAvg = 0;
    uint32_t mCurrentSum = 0;
    uint16_t* mData;
    uint8_t mCurrentNumOfElements = 0;
    uint8_t mMaNumberOfElements = 0;
    uint8_t currentIndex = 0;

    uint32_t framesUnder5 = 0;
    uint32_t framesUnder15 = 0;
    uint32_t frames15to20 = 0;
    uint32_t framesOver20 = 0;
};

FrameTimeRunningAvg::FrameTimeRunningAvg(uint8_t maxNumOfElements) : mMaNumberOfElements(maxNumOfElements)
{
    mData = new uint16_t[mMaNumberOfElements];
    std::memset(mData, 0, sizeof(uint16_t)* mMaNumberOfElements);
    //init to 0?
}

FrameTimeRunningAvg::~FrameTimeRunningAvg()
{
    delete[] mData;
}

void FrameTimeRunningAvg::AddElement(__int64 element)
{
    // Since this is a running average of frame time ms, we're assuming very small
    // numbers (i.e. under 5000ms for sure). If it is over 5000ms, I probably put
    // a breakpoint and am debugging - so just discard.

    // element should never be less than 0, otherwise my currentime - previoustime is really f'd up
    if (element < 0 || element > 5000)
    {
        return;
    }

    //so increasing 1 bit means I double the space of my previous
    // thus the first bit increase, I can store 2 of my previous max number
    // the next bit increase, I can store 4 of the original max number
    // etc..
    // 1 byte increase means I can store 2^8 (256) times of the original max
    // 2 byte increase means I can store 2^16 times of the original max
    // so int32 can store 2^16 of MAX_INT16
    // so if I store int16 in the array, and only have like 10 elements,
    // then I need like 3 extra bits for my sum total to never overflow


    // Again, since low numbers, I know the sum isn't going to overflow.
    // I could do the add/remove averages approach, but that is susceptiable to
    // floating point error

    if (mCurrentNumOfElements == mMaNumberOfElements)
    {
        // currentAvg -= mData[currentIndex] / (double)mMaNumberOfElements;
        mCurrentSum -= mData[currentIndex];
        mData[currentIndex] = (uint16_t)element; //this is safe because I cap it at 5000 max anyway
        //currentAvg += mData[currentIndex] / (double)mMaNumberOfElements;
        mCurrentSum += mData[currentIndex];
    }
    else
    {
        mData[currentIndex] = (uint16_t)element;
        mCurrentSum += mData[currentIndex];
        ++mCurrentNumOfElements;
        //currentAvg = 0;
        //for (int i = 0; i < mCurrentNumOfElements; i++)
        //{
        //    currentAvg += mData[i] / mCurrentNumOfElements;
        //}
    }

    mRunningAvg = (double)mCurrentSum / mCurrentNumOfElements;

    if (currentIndex + 1 == mMaNumberOfElements)
        currentIndex = 0;
    else
        ++currentIndex;


    //static unsigned int framesUntilPrint



    if (element < 5)
        ++framesUnder5;
    else if (element >= 5 && element < 15)
        ++framesUnder15;
    else if (element >= 15 && element < 20)
        ++frames15to20;
    else
        ++framesOver20;


    --framesUntilPrint;
    if (framesUntilPrint == 0)
    {
        std::cout << "framesUnder5: " << framesUnder5 << std::endl;
        std::cout << "frames5to15:  " << framesUnder15 << std::endl;
        std::cout << "frames15to20:  " << frames15to20 << std::endl;
        std::cout << "framesOver20:  " << framesOver20 << std::endl;

        framesUntilPrint = 2500;
    }



}

double FrameTimeRunningAvg::GetRunningAvg() const
{
    return mRunningAvg;
    //return currentAvg;
}



void UpdateSubsystems(double timeDelta)
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
    mim.Initialize(memoryPageSize * 5);
    timer.Initialize();

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
    const GLuint& vertexBufferHandle,
    const GLuint&  uvBufferHandle,
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
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);
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
    glBindBuffer(GL_ARRAY_BUFFER, uvBufferHandle);
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

    glBindBuffer(GL_ARRAY_BUFFER, 0); //clear the binding


}

void Draw(MeshInstance* meshInstance, RigidBody* rigidBody)
{
    // currentTime = timer.GetTime();

    Draw(rigidBody->mMVPForScene,
        meshInstance->GetVertBufferHandle(),
        meshInstance->GetUVBufferHandle(),
        meshInstance->GetTextureHandle());
    //  if (framesUntilPrint == 1)
    //  {
    //      __int64 currentTime2 = timer.GetTime();
    //      std::cout << "time of Draw sample: " << currentTime2 - currentTime << std::endl;

    //  }
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
    objRB->mName = obj.GetName() + "RB";
    obj.SetRigidBodyComponent(objRB);

    // MESH INSTANCE

    MeshInstance* objMI = mim.CreateMeshInstance();
    objMI->SetName(obj.GetName() + "MI");
    objMI->SetTexture(uvDataSize, uvData, textureHandle);
    objMI->SetVertexData(vertDataSize, vertData);
    obj.SetMeshInstanceComponent(objMI);
}

//bool WGLExtensionSupported(const char *extension_name)
//{
//    // this is pointer to function which returns pointer to string with list of all wgl extensions
//    PFNWGLGETEXTENSIONSSTRINGEXTPROC _wglGetExtensionsStringEXT = NULL;
//
//    // determine pointer to wglGetExtensionsStringEXT function
//    _wglGetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC)wglGetProcAddress("wglGetExtensionsStringEXT");
//
//    if (strstr(_wglGetExtensionsString(), extension_name) == NULL)
//    {
//        // string was not found
//        return false;
//    }
//
//    // extension is supported
//    return true;
//}



int main()
{
    //PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
    // wglSwapIntervalEXT(false); // false to disable, true to enable



    /*
    PFNWGLSWAPINTERVALEXTPROC       wglSwapIntervalEXT = NULL;
    PFNWGLGETSWAPINTERVALEXTPROC    wglGetSwapIntervalEXT = NULL;

    if (WGLExtensionSupported("WGL_EXT_swap_control"))
    {
    // Extension is supported, init pointers.
    wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)LogGetProcAddress("wglSwapIntervalEXT");

    // this is another function from WGL_EXT_swap_control extension
    wglGetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC)LogGetProcAddress("wglGetSwapIntervalEXT");

    }*/
    // INIT SUBSYSTEMS - OpenGL, RigidyBodyManager, MeshInstanceManager

    SYSTEM_INFO si;
    GetSystemInfo(&si);

    InitializeSubsystems(si.dwPageSize);

    //note this has to be done after openGL context is initialized. move this to somewhere better
#ifdef _WIN32
    // Turn off vertical screen sync under Windows.
    // (I.e. it uses the WGL_EXT_swap_control extension)
    typedef BOOL(WINAPI *PFNWGLSWAPINTERVALEXTPROC)(int interval);
    PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;
    wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
    if (wglSwapIntervalEXT)
        wglSwapIntervalEXT(0);
#endif

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
    float avgDelta = 0;
    RunningAvg ra(10);

    __int64 previousTime = 0;
    reinterpret_cast<RigidBody*>(ball.GetRigidBodyComponent())->mDirection = glm::vec4(-1, 0, 0, 0);
    reinterpret_cast<RigidBody*>(ball.GetRigidBodyComponent())->mSpeed = 0.001f;


    while (!endGameLoop)
    {

        if (reinterpret_cast<RigidBody*>(ball.GetRigidBodyComponent())->mPositionWorldCoord.x < -3 ||
            reinterpret_cast<RigidBody*>(ball.GetRigidBodyComponent())->mPositionWorldCoord.x > 3)
        {
            reinterpret_cast<RigidBody*>(ball.GetRigidBodyComponent())->mDirection.x *= -1;
        }

        __int64 currentTime = timer.GetTime();


        //rbm.DebugPrint();

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Update subsystems
        ra.AddElement(currentTime - previousTime);
        previousTime = currentTime;
        UpdateSubsystems(ra.GetRunningAvg());

        void* ballMI = ball.GetMeshInstanceComponent();
        void* ballBR = ball.GetRigidBodyComponent();

        MeshInstance* meshInstance = reinterpret_cast<MeshInstance*>(ballMI);
        RigidBody* rigidBody = reinterpret_cast<RigidBody*>(ballBR);

        // How do I handle drawing? Is that part updating MeshInstance Subysystem? or is that responsbility just compute MVPs?
        Draw(reinterpret_cast<MeshInstance*>(ball.GetMeshInstanceComponent()), reinterpret_cast<RigidBody*>(ball.GetRigidBodyComponent()));
        Draw(reinterpret_cast<MeshInstance*>(leftPaddle.GetMeshInstanceComponent()), reinterpret_cast<RigidBody*>(leftPaddle.GetRigidBodyComponent()));
        Draw(reinterpret_cast<MeshInstance*>(rightPaddle.GetMeshInstanceComponent()), reinterpret_cast<RigidBody*>(rightPaddle.GetRigidBodyComponent()));

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

        //std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        //endGameLoop = true;
    }

    //TODO - RELEASE STUFF

    system("pause");
    return 0;
}