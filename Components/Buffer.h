#ifndef BUFFER_H
#define BUFFER_H

// Include GLEW
#include <GL/glew.h>
// Include standard headers
#include <cstring>


#include <iostream>

class Buffer
{
public:
    //Buffer(){}
    Buffer(unsigned int bufferSize, GLfloat * data) : mBufferSize(bufferSize)
    {
        std::cout << "Buffer made" << std::endl;
        //todo - does size == 0 break?
        mBufferData = new GLfloat[mBufferSize];
        std::memset(mBufferData, 0, mBufferSize);
        std::memcpy(mBufferData, data, mBufferSize);

        //GLuint mBufferHandle;
        glGenBuffers(1, &mBufferHandle); //creates buffer object for vertexbuffer varialbe
    }
    ~Buffer()
    {
        std::cout << "Buffer destroyed" << std::endl;
        delete[] mBufferData;
    }

    // handle to GL_ARRAY_BUFFER containing the data for this object
    GLuint mBufferHandle;

    // buffer data (could be vertices, could be uv data)
    GLfloat* mBufferData;

    unsigned int mBufferSize;

private:
    //Buffer(const Buffer&) = delete; //I don't want any copies atm
};

#endif