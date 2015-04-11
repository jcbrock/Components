#include "RigidBody.h"
#include <iostream>

RigidBody::RigidBody()
{
    std::cout << "RigidBody constructor!" << std::endl;
}

RigidBody::~RigidBody()
{
    std::cout << "RigidBody destructor!" << std::endl;
}

RigidBody::RigidBody(const RigidBody& obj)
{
    std::cout << "RigidBody copy constructor!" << std::endl;
}

RigidBody& RigidBody::operator=(const RigidBody& obj)
{
    std::cout << "RigidBody assignment operator!" << std::endl;
    return *this;
}

void RigidBody::Initialize()
{

}