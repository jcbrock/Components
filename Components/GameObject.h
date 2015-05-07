#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include <array>

class GameObject
{
public:
    GameObject();
    GameObject(const std::string& name);
    virtual ~GameObject();

    GameObject(const GameObject&);

    void SetName(const std::string& name);
    std::string GetName();


    void DebugPrint();
    
    void SetComponent(void* ptr, int slot);
    void SetRigidBodyComponent(void* ptr);
    void SetMeshInstanceComponent(void* ptr);
    void SetCComponent(void* ptr);

    void* GetRigidBodyComponent();
    void* GetMeshInstanceComponent();

private:

public:

private:
    char mName[12]; //stack 12 bytes

    std::array<void*, 5> mComponents; //20 bytes
    //make these constants
    //slot 0 - RigidBody
    //slot 1 - MeshInstance
    //slot 2 - 
    //slot 3 - 
    //slot 4 - 

    //possible subsystems: Graphics, Rigidbody, Audio, 
    //RigidBody (position, size, collidable, etc)
    //Items -> no rigid body, but probably have some subsystems
    //Characters -> hvae rigid body


    //I need to read up on the game update loop. Obviously I want an event driven system, but how does that mess with the game update loop?
    //Do I queue up events/tasks that then get processed each time through the loop?

    //Each game object has a queue of events to handle? Subsystem holds onto the <object, event> pair? Read up on this :/
};




#endif