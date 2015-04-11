#include <iostream>
#include "GameObject.h"
#include "Item.h"

//#include "RigidBodyManager.h"


void UpdateSubsystems()
{

}

int foo2();
bool OpenGLInit();

bool InitializeSubsystems()
{
    bool ok = true;

    ok = ok && OpenGLInit();

    return ok;
}

int main()
{
    void* ptr = nullptr;
    std::memcpy(ptr, 0, 4);

    GameObject go;
    go.DebugPrint();
    go.SetName("Jeff");
    go.DebugPrint();
    {
        GameObject go2(go);
        go2.DebugPrint();
    }

    Item i;
    i.DebugPrint();
    dynamic_cast<GameObject*>(&i)->DebugPrint();


    bool endGameLoop = false;
    while (!endGameLoop)
    {
        //Update subsystems
        UpdateSubsystems();
        endGameLoop = true;
    }

    //foo2();4


    // Initialize Subsystems (OpenGL, any singletons)
    InitializeSubsystems();

    // More init? When done, enter game loop
    /*RigidBodyManager rbm;
    rbm.Initialize();
    go.SetRigidBodyComponent(rbm.CreateRigidBody());
    go.SetRigidBodyComponent(rbm.CreateRigidBody());
    rbm.DestroyRigidBody(0);
    */

    system("pause");
    return 0;
}

//Ok, loop wise
//  - we update the subsystems at the same time, NOT per object
//  - how do I do the timing estimates? Well if I have a budgets... naw, that is pointless now
//  - what do I want the loop to look like? 
//  - 

// ok, I guess graphics would be an obvious start. fuuuuuuck, do I port pong?
// yeah, I think that is a good apporach.

