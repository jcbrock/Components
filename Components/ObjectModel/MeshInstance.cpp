#include "MeshInstance.h"
#include <iostream>

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

void MeshInstance::Initialize()
{
 
    strcpy(mName, "Unknown");
}

void MeshInstance::SetName(const std::string& name)
{
    if (name.length() > 13)
    {
        //todo - print error
        return;
    }

    strcpy(mName, name.c_str());
}

void MeshInstance::DebugPrint()
{
    std::cout << mName << std::endl <<
        "	mVerticesHandle: " << mVertices.mBufferHandle << std::endl <<
        "	mUVBufferHandle: " << mUVBuffer.mBufferHandle << std::endl <<
        "	mTextureHandle: " << mTextureHandle << std::endl << std::endl;
}