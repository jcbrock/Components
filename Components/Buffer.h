#ifndef BUFFER_H
#define BUFFER_H

// Include GLEW
#include <GL/glew.h>

class Buffer
{
public:
    Buffer();
    ~Buffer();

    // To change buffer data, just recall this function.
    // Same buffer handle will be used.
    void Initialize(unsigned int bufferSize, GLfloat * data);
    bool IsInitialized() const { return mInitialized; }

    GLuint GetHandle() const;
    unsigned int GetSize() const;
 
private:
    Buffer(const Buffer&) = delete; //I don't want any copies atm
    Buffer(Buffer&&) = delete;

private:
    // handle to GL_ARRAY_BUFFER containing the data for this object
    GLuint mBufferHandle;

    // buffer data (could be vertices, could be uv data)
    GLfloat mBufferData[500];

    unsigned int mBufferSize;
    bool mInitialized;

};

#endif