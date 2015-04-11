#include "GameObject2.h"
#include <iostream>

GameObject2::GameObject2()
{
    std::cout << "Game object constructor!" << std::endl;
}

GameObject2::~GameObject2()
{
    std::cout << "Game object destructor!" << std::endl;
}

GameObject2::GameObject2(const GameObject2& obj)
{
    std::cout << "Game object copy constructor!" << std::endl;
}

GameObject2& GameObject2::operator=(const GameObject2& obj)
{
    std::cout << "Game object assignment operator!" << std::endl;
    return *this;
}

void GameObject2::InitializeMeshInstance(
    unsigned int vertBufferSize, GLfloat * vertData,
    unsigned int uvBufferSize, GLfloat * uvData,
    GLuint textureId)
{
    mMeshInstance = std::make_unique<MeshInstance>();
    mMeshInstance->mVertices = std::make_unique<Buffer>(vertBufferSize, vertData);
    mMeshInstance->mUVBuffer = std::make_unique<Buffer>(uvBufferSize, uvData);
    mMeshInstance->mTextureHandle = textureId;
}

void GameObject2::InitializeRigidBodyInstance(
    float width, float height,
    glm::vec4 position, glm::vec4 direction,
    float speed)
{
    mRigidBodyInstance = std::make_unique<RigidBody>();
    mRigidBodyInstance->mWidth = width;
    mRigidBodyInstance->mHeight = height;
    mRigidBodyInstance->mPositionWorldCoord = position;
    mRigidBodyInstance->mDirection = direction;
    mRigidBodyInstance->mSpeed = speed;
}
void GameObject2::SetSpeed(float newSpeed)
{
    mRigidBodyInstance->mSpeed = newSpeed;
}

void GameObject2::SetDirection(glm::vec4 newDirection)
{
    mRigidBodyInstance->mDirection = newDirection;
}