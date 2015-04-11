#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include <array>

// Name (for ease of use)
// Components
// Make virtual?
// GO
//  Item
//  Character
//  

class GameObject
{
public:
    GameObject();
    virtual ~GameObject();

    GameObject(const GameObject&);

    void SetName(std::string name);
    std::string GetName();


    void DebugPrint();
    
    void SetComponent(void* ptr, int slot);
    void SetRigidBodyComponent(void* ptr);
    void SetBComponent(void* ptr);
    void SetCComponent(void* ptr);

private:

public:

private:
    //std::string mName;
    //char* mName; //heap
    char mName[12]; //stack 12 bytes

    std::array<void*, 5> mComponents; //20 bytes
    //make these constants
    //slot 0 - RigidBody
    //slot 1 - 
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