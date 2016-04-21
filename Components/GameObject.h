#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include <array>

class GameObject
{
    //todo - right place for this?
    enum ComponentIndex// : unsigned char
    {
        RigidBody = 0,
        MeshInstance,
        MaxSize // MUST BE LAST ELEMENT
    };
public:
    GameObject();
    GameObject(const std::string& name);
    virtual ~GameObject();

    GameObject(const GameObject&);

    void SetName(const std::string& name);
    std::string GetName();


    void DebugPrint();
    
    void SetComponent(void* ptr, unsigned int slot);
    void SetRigidBodyComponent(void* ptr);
    void SetMeshInstanceComponent(void* ptr);
    //void SetCComponent(void* ptr);

    void* GetRigidBodyComponent() const;
    void* GetMeshInstanceComponent() const;

private:

public:

private:
    char mName[12]; //stack 12 bytes

    // The question is - do I have an array of components with slots always being the same type of components?
    // - Why even have an array then? I don't think I'll ever iterate through them...
    // Or do I have a dynamic list of components? Then each time I'm looking for a RB, I need to lookup the type
    // - Saves on size, but lookup time is crappier
    // Getting compile time lookups would be nice tho...
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