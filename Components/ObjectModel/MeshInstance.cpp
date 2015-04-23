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
	mName = "Unknown";
}

void MeshInstance::DebugPrint()
{
	std::cout << mName << std::endl <<
		"	mVerticesHandle: " << mVertices2.mBufferHandle << std::endl <<
		"	mUVBufferHandle: " << mUVBuffer2.mBufferHandle << std::endl <<
		"	mTextureHandle: " << mTextureHandle << std::endl <<
		"	mMVPForScene: " << mMVPForScene[0].x << "," << mMVPForScene[0].y << "," <<
		mMVPForScene[0].z << "," << mMVPForScene[0].w << std::endl << std::endl;
}