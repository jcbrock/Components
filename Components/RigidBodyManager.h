#ifndef RIGID_BODY_MANAGER_H
#define RIGID_BODY_MANAGER_H

//for glm  /transofrm stuff (camera stuff)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class RigidBody;

class RigidBodyManager //inherit?
{

public:
    void Initialize(unsigned long maxSize);
    void UpdateSubsystem(size_t timeDelta); //inherit?
    RigidBody* CreateRigidBody(); //returns a handle to RigidBody
    
    void CleanupDeletions();

    void MarkRigidBodyForDelete(int index);
    void DestroyRigidBody(unsigned int index);
    
    void DebugPrint();
private:

    void* mRigidBodyArray; //space for 8 RBs for now.
    void* freeHeadPtr;
 
    unsigned int mNumberOfAllocatedBlocks;
    unsigned int mMaxSize;

    // maybe this isn't the best place, but then again, this is kinda the point
    // of having a mgr classs like this we can use all this stuff once
    //FOV, height, aspect, near, far
    glm::mat4 mProjection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    // Camera matrix
    glm::vec3 cameraPos = glm::vec3(0, 0, 20);
    glm::mat4 mView = glm::lookAt(
        cameraPos, // Camera is at (4,3,3), in World Space
        glm::vec3(0, 0, 0), // and looks at the origin
        glm::vec3(0, 1, 0));  // Head is up (set to 0,-1,0 to look upside-down)
};

#endif