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
    mName = "Unknown";
    mSpeed = 1.0;
    mWidth = 1.0;
    mHeight = 3.0;
    mIsCollidable = true;
}

void RigidBody::DebugPrint()
{
    std::cout << mName << std::endl <<
        "	mSpeed: " << mSpeed << std::endl <<
        "	mWidth: " << mWidth << std::endl <<
        "	mHeight: " << mHeight << std::endl <<
        "	mPositionWorldCoord: " << mPositionWorldCoord.x << "," << mPositionWorldCoord.y << std::endl <<
        "	mDirection: " << mDirection.x << "," << mDirection.y << std::endl <<
        "	mIsCollidable: " << mIsCollidable << std::endl <<
        "	mMVPForScene: " << mMVPForScene[0].x << "," << mMVPForScene[0].y << "," <<
        mMVPForScene[0].z << "," << mMVPForScene[0].w << std::endl << std::endl;
}