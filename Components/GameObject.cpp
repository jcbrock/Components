#define _CRT_SECURE_NO_WARNINGS

#include "GameObject.h"
#include <iostream>

GameObject::GameObject()
{
}

GameObject::GameObject(const std::string& name)
{
    if (name.size() > 11)
    {
        //print error
        return;
    }

    strcpy(mName, name.c_str());
}

GameObject::~GameObject()
{
}

GameObject::GameObject(const GameObject& copy)
{
    strcpy(mName, copy.mName);
}

void GameObject::SetName(const std::string& name)
{
    if (name.size() > 11)
    {
        //print error
        return;
    }
    
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
}

void GameObject::SetComponent(void* ptr, unsigned int slot)
{
    if (slot < 0 || slot > ComponentIndex::MaxSize)
    {
        //print error
        return;
    }

    mComponents[slot] = ptr;
}

void GameObject::SetRigidBodyComponent(void* ptr)
{
    SetComponent(ptr, ComponentIndex::RigidBody);
}

void GameObject::SetMeshInstanceComponent(void* ptr)
{
    SetComponent(ptr, ComponentIndex::MeshInstance);
}

//void GameObject::SetCComponent(void* ptr)
//{
//    SetComponent(ptr, 2);
//}

//hmm, it is nice to not have this class dependant on other crap, like RigidBody
//well, if not here, then where? 
void* GameObject::GetRigidBodyComponent() const
{
    return mComponents[ComponentIndex::RigidBody];
}

void* GameObject::GetMeshInstanceComponent() const
{
    return mComponents[ComponentIndex::MeshInstance];
}