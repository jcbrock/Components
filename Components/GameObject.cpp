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

void GameObject::SetMeshInstanceComponent(void* ptr)
{
    SetComponent(ptr, 1);
}

void GameObject::SetCComponent(void* ptr)
{
    SetComponent(ptr, 2);
}

void* GameObject::GetRigidBodyComponent() const
{
    return mComponents[0];
}

void* GameObject::GetMeshInstanceComponent() const
{
    return mComponents[1];
}