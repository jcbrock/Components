#include "Buffer.h"
#include <iostream>
#include <cstring> //for std::memset
#include <assert.h>

Buffer::Buffer() : mBufferSize(0), mBufferHandle(0), mInitialized(false)
{
}

Buffer::~Buffer()
{
    if (mInitialized)
    {
        std::cout << "Buffer destroyed" << std::endl;
        delete[] mBufferData;
    }
}

void Buffer::Initialize(unsigned int bufferSize, GLfloat * data)
{
    assert(bufferSize < 500);

    // Convienent for debugging
    std::memset(mBufferData, 0, 500);

    if (!mInitialized)
    {
        glGenBuffers(1, &mBufferHandle); //creates buffer object for vertexbuffer varialbe
        std::cout << "Buffer made for the first time! Handle: " << mBufferHandle << std::endl;

        mInitialized = true;
    }

    mBufferSize = bufferSize;
    std::memcpy(mBufferData, data, bufferSize);

    glBindBuffer(GL_ARRAY_BUFFER, mBufferHandle); //binds buffer handle to GL_ARRAY_BUFFER. Future opengl calls will operate on this buffer
    glBufferData(GL_ARRAY_BUFFER, mBufferSize, mBufferData, GL_STATIC_DRAW); //do shit to bound buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0); //clear the binding, so we don't accendently make modifcations to the bound buffer
}

GLuint Buffer::GetHandle() const
{
    assert(mInitialized);

    return mBufferHandle;
}

unsigned int Buffer::GetSize() const
{
    assert(mInitialized);

    return mBufferSize;
}