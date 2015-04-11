#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

//#include <list>
//#include <array>
//#include "ObjectModel\RigidBody.h"

class RigidBodyManager //inherit?
{

public:
    void Initialize();
    void UpdateSubsystem(); //inherit?
    void* CreateRigidBody(); //returns a handle to RigidBody
    
    void MarkRigidBodyForDelete(int index);
    void DestroyRigidBody(unsigned int index);
    //marking expired should be on RB
    //void DestroyRigidBody(); //mark expired, next time through loop, we remove that item from linked list (so we don't have to iterate tho at this exact moment and destroy)

private:
    //void MarkRigidBodyForDelete(int index);
    //void DestroyRigidBody(int index);

    //LL of rigidbodies? vector? vector = better memory layout. Lots of inserts and deletes tho...
    //std::list<RigidBody> mRigidBodies;

    //How to dynamically create Rigidbodies in a row without my own allocator? Do I gotta do it up front? I guess I'll do that for now...
    //std::array<RigidBody, 10> mRigidBodies;



    void* mRigidBodyArray; //space for 8 RBs for now.
    void* freeHeadPtr;
    unsigned int highestAllocatedIndex;
    unsigned int maxSize;
    //1024 bytes / sizeof rigidbodies - that is how many elements my bit array needs to be. w/e, i'll create it extra large now
    void* mUsedArray;
    bool mUsedArray2[512];






};

#endif