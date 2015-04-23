#ifndef MESH_INSTANCE_MANAGER_H
#define MESH_INSTANCE_MANAGER_H

//#include <list>
//#include <array>
#include "ObjectModel\MeshInstance.h"

//for glm  /transofrm stuff
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class MeshInstanceManager //inherit?
{

public:
    void Initialize(unsigned int maxSize);
    void UpdateSubsystem(float timeDelta); //inherit?
	MeshInstance* CreateMeshInstance(); //returns a handle to MeshInstance
    
	void CleanupDeletions();

    void MarkMeshInstanceForDelete(int index);
    void DestroyMeshInstance(unsigned int index);
    
	//void DestroyMeshInstance(); //mark expired, next time through loop, we remove that item from linked list (so we don't have to iterate tho at this exact moment and destroy)
	void DebugPrint();
private:

    void* mMeshInstanceArray; //space for 8 RBs for now.
    void* freeHeadPtr;
    //unsigned int highestAllocatedIndex;
	unsigned int mNumberOfAllocatedBlocks;
    unsigned int mMaxSize;

	// maybe this isn't the best place, but then again, this is kinda the point
	// of having a mgr classs like this we can use all this stuff once
	glm::mat4 mProjection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	
	// Camera matrix
	glm::vec3 cameraPos = glm::vec3(0, 0, 10);
	glm::mat4 mView = glm::lookAt(
		cameraPos, // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0));  // Head is up (set to 0,-1,0 to look upside-down)



    //1024 bytes / sizeof rigidbodies - that is how many elements my bit array needs to be. w/e, i'll create it extra large now
};

#endif