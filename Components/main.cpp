#include <iostream>
#include <vector>
#include <thread>

#include "common\LoggingDefines.h"
#include "GameObject.h"

#include "ObjectModel\MeshInstance.h"
#include "ObjectModel\RigidBody.h"

#include "ComponentManager.h"
#include "OpenGLManager.h"

#include "Timer.h"
#include "FrameTimeRunningAvg.h"

#include <thread>
#include <chrono>
#include <stdint.h>

//Input handling stuff
#include "Input\KeyboardInput.h"
#include "Handle.h"

//Event stuff
#include "EventSystem\Event.h"
#include "EventSystem\EventData.h"
#include "EventSystem\EventEnums.h"
#include "EventSystem\EventQueue.h"
#include "EventSystem\EventMemoryPoolManager.h"

// for page size
#include <windows.h>

// vsync stuff
#include <GL\wglew.h>

#include <glfw3.h>

//TODO - look at includes, think about dependencies

//	Each subsystem (i.e. ComponentManager<RigidBody>) will control the memory
//	management for all subsystem items (i.e. RigidBodies).
//	Each manager contains an array of all objects of that type, so that when we
//	update a subsystem, they all get updated. GameObjects will have pointers
//	to their specific rigidbody.

extern GLFWwindow* window;
ComponentManager<RigidBody> gRigidBodyMgr;
ComponentManager<MeshInstance> gMeshInstanceMgr;
EventMemoryPoolManager gEventMgr;
OpenGLManager gOpenGLMgr;
Timer gTimer;
int32_t gFrameCount = 0;


// Data - put in data.cpp for organization
extern GLfloat gLeftPaddleData[];
extern GLfloat gRightPaddleData[];
extern GLfloat gBallData[];
extern GLfloat gUVBufferData[];
extern size_t gLeftPaddleDataSize;
extern size_t gRightPaddleDataSize;
extern size_t gBallDataSize;
extern size_t gUVBufferDataSize;

bool InitializeSubsystems(DWORD memoryPageSize)
{
    std::cout << "Memory page size on this system: " << memoryPageSize << std::endl;
    //LOG_SIMPLE("Memory page size on this system: %s", memoryPageSize); what format is a DWORD?

    bool ok = true;

    ok = ok && gOpenGLMgr.OpenGLInit();
    gEventMgr.Initialize(memoryPageSize);
    gRigidBodyMgr.Initialize(memoryPageSize);
    gMeshInstanceMgr.Initialize(memoryPageSize * 5);

    /*ComponentManager<MeshInstance> gTest;
    gTest.Initialize(memoryPageSize * 5);
    MeshInstance* m = gTest.CreateComponent();
    gTest.DebugPrint();
    gTest.UpdateSubsystem(123);
    gTest.MarkComponentForDelete(m);
    gTest.UpdateSubsystem(123);
    gTest.DebugPrint();

    ComponentManager<RigidBody> gTestR;
    gTestR.Initialize(memoryPageSize * 5);
    RigidBody* r = gTestR.CreateComponent();
    gTestR.DebugPrint();
    gTestR.UpdateSubsystem(123);
    gTestR.MarkComponentForDelete(r);
    gTestR.UpdateSubsystem(123);
    gTestR.DebugPrint();*/


    gMeshInstanceMgr.Initialize(memoryPageSize * 5);


    gTimer.Initialize();

    return ok;
}

void UpdateSubsystems(double timeDelta)
{
    gRigidBodyMgr.UpdateSubsystem(timeDelta);
    gMeshInstanceMgr.UpdateSubsystem(timeDelta);
}

void Draw(const GameObject& obj)
{
    MeshInstance* mi = reinterpret_cast<MeshInstance*>(obj.GetMeshInstanceComponent());
    RigidBody* rb = reinterpret_cast<RigidBody*>(obj.GetRigidBodyComponent());

    if (mi && rb)
    {
        gOpenGLMgr.Draw(rb->mMVPForScene,
            mi->GetVertBufferHandle(),
            mi->GetUVBufferHandle(),
            mi->GetTextureHandle());
    }
    else
    {
        // Log unable to draw obj message
    }
}

void SetupGameObject(GameObject& obj,
    int vertDataSize,
    GLfloat* vertData,
    int uvDataSize,
    GLfloat* uvData,
    GLuint textureHandle)
{
    // Setup rigid body

    RigidBody* objRB = gRigidBodyMgr.CreateComponent();
    objRB->SetName(obj.GetName() + "RB");
    obj.SetRigidBodyComponent(objRB);

    // Setup mesh instance

    MeshInstance* objMI = gMeshInstanceMgr.CreateComponent();
    objMI->SetName(obj.GetName() + "MI");
    objMI->SetTexture(uvDataSize, uvData, textureHandle);
    objMI->SetVertexData(vertDataSize, vertData);
    obj.SetMeshInstanceComponent(objMI);
}

EventQueue eventQueue;
void HandleEvent(Event& evt)
{
    switch (evt.GetType())
    {
    case EventType::UP_ARROW_PRESSED:
    {
        CollisionData* data = dynamic_cast<CollisionData*>(evt.GetData());
        reinterpret_cast<RigidBody*>(data->obj2->GetRigidBodyComponent())->mDirection *= -1;
        break;
    }
    case EventType::MOVE_PADDLE:
    {
        // so objects are gunna move regardless of if they got an event or not,
        // THAT is what the update loop is about. It is fine if I move this here and it isn't
        // a batched type thing...

        //only way to improve is if I handled multiple of the same type of events at once...
        //maybe not a bad idea...? I'll save that for later - TODO

        MovePaddleData* data = dynamic_cast<MovePaddleData*>(evt.GetData());
        reinterpret_cast<RigidBody*>(data->obj1->GetRigidBodyComponent())->mPositionWorldCoord.x += data->destX;
        reinterpret_cast<RigidBody*>(data->obj1->GetRigidBodyComponent())->mPositionWorldCoord.y += data->destY;
        break;
    }
    case EventType::PRINT_DEBUG:
    {

        LOG_SIMPLE("DEBUG PRINTING");
        gEventMgr.DebugPrint();
    }
    //default:
    }

    //todo, remove event from queue
    evt.SetHasBeenProcessed(true);// = ;
}

std::unordered_map<PongGameHandle, GameObject*> gWorldObjects;

bool someFunc()
{
    return true;
}

int main()
{
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

    //todo - have a singleton, or some sort of world query helper... otherwise I can only access these guys in this scope
    // maybe I have some sort of ID system so I don't need to actually access the real objec,t but i have a feeling i'll need the 
    // data so I can search it for certain critera...see what the book says

    // I'll have handles for my world objects - hard code the handles for now rather than doing a lookup

    GameObject leftPaddle("LeftPaddle");
    GameObject rightPaddle("RightPaddle");
    GameObject ball("Ball");

    SetupGameObject(leftPaddle, gLeftPaddleDataSize, gLeftPaddleData, gUVBufferDataSize, gUVBufferData, gOpenGLMgr.mTexture);
    SetupGameObject(rightPaddle, gRightPaddleDataSize, gRightPaddleData, gUVBufferDataSize, gUVBufferData, gOpenGLMgr.mTexture);
    SetupGameObject(ball, gBallDataSize, gBallData, gUVBufferDataSize, gUVBufferData, gOpenGLMgr.mTexture);

    gWorldObjects.insert(std::make_pair<PongGameHandle, GameObject*>(PongGameHandle::LEFT_PADDLE, &leftPaddle));
    gWorldObjects.insert(std::make_pair<PongGameHandle, GameObject*>(PongGameHandle::RIGHT_PADDLE, &rightPaddle));
    gWorldObjects.insert(std::make_pair<PongGameHandle, GameObject*>(PongGameHandle::BALL, &ball));

    leftPaddle.DebugPrint();
    rightPaddle.DebugPrint();
    ball.DebugPrint();
    gRigidBodyMgr.DebugPrint();
    gMeshInstanceMgr.DebugPrint();

    //TODO - map keyboard input to events
    CollisionData* data = new CollisionData();
    data->obj1 = &leftPaddle;
    data->obj2 = &ball;
    Event* dummyEvt = gEventMgr.CreateEvent2();
    dummyEvt->Initialize(EventType::UP_ARROW_PRESSED, data, EventPriority::LOW, 10000000);
    eventQueue.Enqueue(dummyEvt);

    KeyboardInput ki;


    // ALL SETUP - ENTER LOOP

    bool endGameLoop = false;
    FrameTimeRunningAvg frameTimeTracker(10);
    double previousTime = 0;

    reinterpret_cast<RigidBody*>(ball.GetRigidBodyComponent())->mDirection = glm::vec4(-1, 0, 0, 0);
    reinterpret_cast<RigidBody*>(ball.GetRigidBodyComponent())->mSpeed = 6.0f; //6 x units per second

    while (!endGameLoop)
    {
        ki.Loop(window);

        // Placeholder code for detecting collision and reversing ball speed
        RigidBody* ballbody = reinterpret_cast<RigidBody*>(ball.GetRigidBodyComponent());
        if (ballbody->mPositionWorldCoord.x < -3 && ballbody->mDirection.x < 0)
        {
            ballbody->mDirection.x *= -1;
            LOG_SIMPLE("Hit! %f", gTimer.GetTime());
        }
        else if (ballbody->mPositionWorldCoord.x > 3 && ballbody->mDirection.x > 0)
        {
            ballbody->mDirection.x *= -1;
            LOG_SIMPLE("Hit! %f", gTimer.GetTime());
        }

        //rbm.DebugPrint();

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Handle Events

        while (true)
        {
            const Event* peek = eventQueue.Peek();
            if (peek && peek->GetFrameToExecute() <= gFrameCount)
            {
                LOG_SIMPLE("About to process: ");
                peek->DebugPrint();
            }
            else
            {
                break;
            }

            Event* evt = eventQueue.Dequeue();

            if (evt)
            {
                HandleEvent(*evt);
            }
        }

        eventQueue.ClearProcessedEvents();

        //Update time
        double currentTime = gTimer.GetTime();
        frameTimeTracker.AddElement(currentTime - previousTime);
        previousTime = currentTime;

        //Update subsystems
        UpdateSubsystems(frameTimeTracker.GetRunningAvg());

        Draw(ball);
        Draw(leftPaddle);
        Draw(rightPaddle);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

        //endGameLoop = true;
        ++gFrameCount;
    }

    //TODO - RELEASE STUFF

    system("pause");
    return 0;
}