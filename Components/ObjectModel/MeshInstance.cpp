#include "MeshInstance.h"
#include <iostream>
#include <assert.h>

MeshInstance::MeshInstance()
{
    std::cout << "MeshInstance constructor!" << std::endl;
}

MeshInstance::~MeshInstance()
{
    std::cout << "MeshInstance destructor!" << std::endl;
}

MeshInstance::MeshInstance(const MeshInstance& obj)
{
    std::cout << "MeshInstance copy constructor!" << std::endl;
}

MeshInstance& MeshInstance::operator=(const MeshInstance& obj)
{
    std::cout << "MeshInstance assignment operator!" << std::endl;
    return *this;
}

MeshInstance::MeshInstance(MeshInstance&& obj)
{
    std::cout << "MeshInstance move constructor!" << std::endl;
    //return *this;
}

void MeshInstance::Initialize()
{

    strcpy(mName, "Unknown");
}

void MeshInstance::SetName(const std::string& name)
{
    assert(name.length() < 15);

    strcpy(mName, name.c_str());
}

std::string MeshInstance::GetName() const
{
    return std::string(mName);
}

GLuint MeshInstance::GetVertBufferHandle() const
{
    return mVertices.GetHandle();
}

GLuint MeshInstance::GetUVBufferHandle() const
{
    return mUVBuffer.GetHandle();
}

GLuint MeshInstance::GetTextureHandle() const
{
    return mTextureHandle;
}

bool MeshInstance::IsMarkedForDeletion() const
{
    return mMarkedForDeletion;
}
void MeshInstance::DebugPrint()
{
    std::cout << mName << std::endl <<
        "	mVerticesHandle: " << mVertices.GetHandle() << std::endl <<
        "	mUVBufferHandle: " << mUVBuffer.GetHandle() << std::endl <<
        "	mTextureHandle: " << mTextureHandle << std::endl << std::endl;
}

void MeshInstance::SetTexture(unsigned int uvBufferSize, GLfloat* data, GLuint textureHandle)
{
    mUVBuffer.Initialize(uvBufferSize, data);
    mTextureHandle = textureHandle;
}

void MeshInstance::SetVertexData(unsigned int vertBufferSize, GLfloat* data)
{
    mVertices.Initialize(vertBufferSize, data);
}