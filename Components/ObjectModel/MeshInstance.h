#ifndef MESH_INSTANCE_H
#define MESH_INSTANCE_H

#include "../Buffer.h"
#include "glm\glm.hpp"

#include <string>

// I'd say this class is responsible for handling the OpenGL representation of obj
class MeshInstance
{
public:
    static unsigned int ID;

    MeshInstance();
    ~MeshInstance();
    MeshInstance(const MeshInstance&);
    //todo - move constructors
    MeshInstance& operator=(const MeshInstance&); //this right?

//private:
    void Initialize();
    void DebugPrint();

    // Getters / Setters
    void SetName(const std::string& name);

	bool mMarkedForDeletion = false;
    GLuint mTextureHandle;

	Buffer mVertices;
    Buffer mUVBuffer;

private:
    char mName[15]; //stack 14 bytes
};

/*
// I'd say this class is responsible for handling the OpenGL representation of obj
class MeshInstance
{
public:
static unsigned int ID;

MeshInstance();
~MeshInstance();
MeshInstance(const MeshInstance&);
//todo - move constructors
MeshInstance& operator=(const MeshInstance&); //this right?

//private:
void Initialize();
void DebugPrint();

// Getters / Setters
void SetName(const std::string& name);

bool mMarkedForDeletion = false;
Handle mTextureHandle;

Buffer2 mVertices2;
Buffer2 mUVBuffer2;

// Temp - just keeping around for TestOpenGL.cpp
//std::unique_ptr<Buffer> mVertices;
//std::unique_ptr<Buffer> mUVBuffer;

private:
char mName[15]; //stack 14 bytes
};

*/


#endif