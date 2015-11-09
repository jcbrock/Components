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
    this->SetName("Unknown");
    mSpeed = 1.0;
    mWidth = 1.0;
    mHeight = 3.0;
    mIsCollidable = true;
}

void RigidBody::DebugPrint()
{
    std::cout << GetName() << std::endl <<
        "	mSpeed: " << mSpeed << std::endl <<
        "	mWidth: " << mWidth << std::endl <<
        "	mHeight: " << mHeight << std::endl <<
        "	mPositionWorldCoord: " << mPositionWorldCoord.x << "," << mPositionWorldCoord.y << std::endl <<
        "	mDirection: " << mDirection.x << "," << mDirection.y << std::endl <<
        "	mIsCollidable: " << mIsCollidable << std::endl <<
        "	mMVPForScene: " << mMVPForScene[0].x << "," << mMVPForScene[0].y << "," <<
        mMVPForScene[0].z << "," << mMVPForScene[0].w << std::endl << std::endl;
}

void RigidBody::Update(double timeDelta)
{
    //TEMP - RigidBody specific stuff here:
    //ok, how to handle collisions...
    //AABB
    //meh, might as well cheat and only check ball for collision
    //then again, this function is agnostic to what rigidbody it is updating.
    //maybe if I detect a collision, I generate an event for those objects
    //to handle later?

    //collisions result  a change in direction (at least for now)
    //how do I not detect ina collision twice tho?

    //speed in x units per sec
    //timeDelta in ms
    //distance to move is speed / 1000 * timedelta


    mPositionWorldCoord.x += mDirection.x * (mSpeed / 1000) * timeDelta;
    mPositionWorldCoord.y += mDirection.y * (mSpeed / 1000) * timeDelta;

    glm::mat4 leftPaddleTranslationMatrix = glm::mat4(1.0f);
    leftPaddleTranslationMatrix = glm::translate(leftPaddleTranslationMatrix, glm::vec3(mPositionWorldCoord));

    mMVPForScene = mProjection * mView * leftPaddleTranslationMatrix;
}