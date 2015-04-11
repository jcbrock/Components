#include "RigidBodyManager.h"
//#include <iostream>
//#include <wchar.h>
//#include <stdlib.h>
//#include <cstring>
//#include <cstdlib>


void RigidBodyManager::Initialize()
{
   /* maxSize = 10; //w/e for now
    mRigidBodyArray = new int[1024]; //4192 bytes
    mUsedArray = new int[128]; //512 possible elements

    std::memset(mUsedArray, 0, 128 * sizeof(int));

    freeHeadPtr = mRigidBodyArray;

    
    RigidBody* ptr = reinterpret_cast <RigidBody*>(mRigidBodyArray);
    //void* nextPtr;
    for (unsigned int i = 0; i < maxSize; ++i)
    {
        void* foo = (void*)ptr;
        std::memset(ptr, 0, sizeof(void*));//set ptr to next free element (next in line)        
        //std::memcpy(ptr, ptr, sizeof(void*));//set ptr to next free element (next in line)        
        std::memcpy(foo, foo, 4);//set ptr to next free element (next in line)        
        ++ptr;
    }

    */
    //how do I construct it? on stack, then memcopy?

    /*  {
          RigidBody rb;
          rb.Initialize();
          int debug = sizeof(rb);
          RigidBody* ptr = reinterpret_cast < RigidBody*>(mRigidBodyArray);
          std::memcpy(ptr, &rb, sizeof(rb));
          }

          std::cout << "";
          */
}

void* RigidBodyManager::CreateRigidBody()
{
    //static int index = 0;
    //if (index > maxSize)
    //{
        //assert - TODO
    //    return nullptr;
    //}

    //ugh, do bit math.
    //void* bPtr = mUsedArray;
    /*
    mUsedArray2[0] = true; //TODO BIT ARRAY


    //TODO - right now i'm not freeing, just adding on, there this is highest
   // highestAllocatedIndex = index; //TOOD?


    RigidBody rb;
    rb.Initialize();
    //int debug = sizeof(rb);
    RigidBody* ptr = reinterpret_cast <RigidBody*>(freeHeadPtr);

    std::memcpy(freeHeadPtr, freeHeadPtr, 4); //first 4 bytes of a block should be a pointer to next free block. TOOD - size check?
   
    std::memcpy(ptr, &rb, sizeof(rb));

    return ptr; //rather than passing around stuff as void ptrs - i should probably use poly morph (i.e. rigidbody, meshinstance, would inherit from Component class)
    */
    return nullptr;
}

void RigidBodyManager::DestroyRigidBody(unsigned int index)
{
    /*
    if (index > maxSize)
    {
        //assert - TODO
        return;
    }

    mUsedArray2[index] = false;

    //actually, I just mark the block free?
    //no, I gotta add it back to the pool...
    RigidBody* ptr = reinterpret_cast <RigidBody*>(mRigidBodyArray);
    std::memcpy(&ptr[index], 0, sizeof(RigidBody));
    std::memcpy(&ptr[index], freeHeadPtr, sizeof(void*));
    freeHeadPtr = (void*)ptr;

    //How do I add it back to the LL? oh, make head pt to it, the ptr
    //in this block is the former head
    */
}

// I guess I'm going to have to do a "if nullptr" check
// each time while iterating, since my block is going to be
// fragmented over time

// Oh wait, I'm having the seperate bit array

// Well, I'm not really doing the free block LL approach, I just check the bit array?
// GetFirstOpenBit - how expensive is that op?
// Well, the downside to the LL apporach is the extra storage of hte ptrs,
// but those are theoretically hidden within the free blocks...

// Ok, I'll try to do the LL apporach too, at least acidemically