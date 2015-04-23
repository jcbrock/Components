#ifndef MESH_INSTANCE_H
#define MESH_INSTANCE_H

#include "../Buffer.h"
#include "glm\glm.hpp"

#include "../misc.h"

#include <memory>
#include <string>


class Buffer;

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
    
    std::string mName; //TODO - can only be a certain size, don't want a pointer to somewhere else
	
	bool mMarkedForDeletion = false;
	Handle mTextureHandle;

	//this does change throughout the game, not positive
	//this is the right place for it
	glm::mat4 mMVPForScene;

	Buffer2 mVertices2;
    Buffer2 mUVBuffer2;
    
	// Temp - just keeping around for TestOpenGL.cpp
	std::unique_ptr<Buffer> mVertices;
    std::unique_ptr<Buffer> mUVBuffer; 
};



#endif