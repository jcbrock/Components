#define _CRT_SECURE_NO_WARNINGS

#include "GameObject.h"
#include <iostream>

GameObject::GameObject()
{
    //mName = "foobar".c_str();
   // mName = new char[12];
}

GameObject::~GameObject()
{
   // delete[] mName;
}

GameObject::GameObject(const GameObject& copy)
{
    //mName = copy.mName;
   // mName = new char[12];
   // strcpy(mName, copy.mName);

    strcpy(mName, copy.mName);
}

void GameObject::SetName(std::string name)
{
    if (name.size() > 11) //(remember null char)
    {
        //print error
        return;
    }
    
    //strcpy(mName, name.c_str());
    strcpy(mName, name.c_str());

}

std::string GameObject::GetName()
{
    return mName;
}

void GameObject::DebugPrint()
{
    std::cout << "[GameObject][" << mName << "]" << std::endl;
    std::cout << "Size: " << sizeof(GameObject) << std::endl << std::endl;
    //std::cout << "Size: " << sizeof(std::string) << std::endl << std::endl;
}

//How do I want to update components?
//  Option 1
//      Iterate over each GO, update components
//  Option 2
//      Iterate over each subsystem, update each GO -> It'd be great if I knew the component slot in the array to use so I don't have to search the array every time

void GameObject::SetComponent(void* ptr, int slot)
{
    if (slot < 0 || slot > 4)
    {
        //print error
        return;
    }

    mComponents[slot] = ptr;
}

void GameObject::SetRigidBodyComponent(void* ptr)
{
    SetComponent(ptr, 0);
}

void GameObject::SetBComponent(void* ptr)
{
    SetComponent(ptr, 1);
}

void GameObject::SetCComponent(void* ptr)
{
    SetComponent(ptr, 2);
}

//NO - this is updating on a per object basis, not ideal
//void GameObject::UpdateASubsystem()
//{
    //if (mComponents[0] != nullptr)
    //mComponents[0].update();
//}






