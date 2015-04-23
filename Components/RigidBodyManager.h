#ifndef RIGID_BODY_MANAGER_H
#define RIGID_BODY_MANAGER_H

//#include <list>
//#include <array>
#include "ObjectModel\RigidBody.h"

class RigidBodyManager //inherit?
{

public:
    void Initialize(unsigned int maxSize);
    void UpdateSubsystem(size_t timeDelta); //inherit?
	RigidBody* CreateRigidBody(); //returns a handle to RigidBody
    
	void CleanupDeletions();

    void MarkRigidBodyForDelete(int index);
    void DestroyRigidBody(unsigned int index);
    
	//void DestroyRigidBody(); //mark expired, next time through loop, we remove that item from linked list (so we don't have to iterate tho at this exact moment and destroy)
	void DebugPrint();
private:

    void* mRigidBodyArray; //space for 8 RBs for now.
    void* freeHeadPtr;
    //unsigned int highestAllocatedIndex;
	unsigned int mNumberOfAllocatedBlocks;
    unsigned int mMaxSize;
    //1024 bytes / sizeof rigidbodies - that is how many elements my bit array needs to be. w/e, i'll create it extra large now
};

#endif