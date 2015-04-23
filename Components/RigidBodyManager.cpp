#include "RigidBodyManager.h"
//#include <iostream>
//#include <wchar.h>
//#include <stdlib.h>
//#include <cstring>
//#include <cstdlib>
//#include "ObjectModel\RigidBody.h"

void RigidBodyManager::DebugPrint()
{
    // pretty dirty with all these casts
    RigidBody* data = reinterpret_cast <RigidBody*>(mRigidBodyArray);

    for (int i = 0; i < mMaxSize; ++i)
    {
        int set = 0;
        char* p = reinterpret_cast<char*>(data)+4;
        
        std::memcpy(&set, p, 4);
        if (set != 0)
        {
            (*data).DebugPrint();
        }

        ++data;
    }
}

void RigidBodyManager::Initialize(unsigned int maxSize)
{
    mNumberOfAllocatedBlocks = 0;
    mMaxSize = maxSize;
    mRigidBodyArray = new RigidBody[maxSize];
    freeHeadPtr = mRigidBodyArray;
   
    // Not really needed, but nice for debugging

    std::memset(mRigidBodyArray, 0, maxSize * sizeof(RigidBody));
    
    RigidBody* nextBlock = reinterpret_cast <RigidBody*>(mRigidBodyArray);

    // Set ptr to next free block
    
    for (unsigned int i = 0; i < mMaxSize - 1; ++i)
    {
        // Copy the actual address of the next address into the block

        RigidBody* currentBlock = nextBlock;
        ++nextBlock; 		
        std::memcpy(currentBlock, &nextBlock, sizeof(void*)); // Assuming 4 byte pointer
    }

    std::memset(nextBlock, 0, sizeof(void*));

}


RigidBody* RigidBodyManager::CreateRigidBody()
{
    // If there is no room, freeHeadPtr will be null

    if (!freeHeadPtr)
    {
        //assert - TODO
        return nullptr;
    }


    // Save pointer to current block and jump head to next free block
    // Note: Make sure to update freeHeadPtr itself, not what it points to
    //		If this is the last block, it will be nullptr
    void* aboutToBeAllocated = freeHeadPtr;
    std::memcpy(&freeHeadPtr, freeHeadPtr, 4);
    
    // Create new rigidbody in this block
    // Fuck, this is the part about how I don't know if this is possible without custom allocator
    RigidBody* ptr = reinterpret_cast <RigidBody*>(aboutToBeAllocated);
    
    // This...heopfully...blows away w/e was in this ptr
    // again, probably wasteful. Ideally I create the object in the block,
    // not on the stack then copy it...
    RigidBody rb;
    rb.Initialize();
    std::memcpy(ptr, &rb, sizeof(rb));

    ++mNumberOfAllocatedBlocks;

    return ptr;
}



//TODO - left off here
// add .gitigore for the files I Don't want tracked... duh, lol
 
//TODO - how do I know the index?
void RigidBodyManager::DestroyRigidBody(unsigned int index)
{
    if (mNumberOfAllocatedBlocks == 0)
    {
        //assert - TODO
        return;
    }


    // Get pointer to block, free it (i.e. just memset it to 0? That is...a bad idea? Cuz I don't
    // call destructor? Then again, do I... need to?)

    // Clear the block

    RigidBody* ptr = reinterpret_cast <RigidBody*>(mRigidBodyArray);
    std::memset(&ptr[index], 0, sizeof(RigidBody));

    // Update pointer to the next free block (which is currently the head)

    std::memcpy(&ptr[index], &freeHeadPtr, sizeof(void*));

    // Update head pointer to newly freed block. This means that the most reecently
    // freed block will be the next block to be filled

    freeHeadPtr = &ptr[index];

    --mNumberOfAllocatedBlocks;
}


//OR CALL DestroyRigidBody during the update loop thru each one
void RigidBodyManager::CleanupDeletions()
{
    for (int i = 0; i < mMaxSize; ++i)
    {
        RigidBody* data = reinterpret_cast <RigidBody*>(mRigidBodyArray);
        
        int set = 0;
        char* p = reinterpret_cast<char*>(data)+4;

        std::memcpy(&set, p, 4);
        if (set != 0 && data->mMarkedForDeletion)
        {
            DestroyRigidBody(i);
        }

        ++data;
    }
}

void RigidBodyManager::UpdateSubsystem(size_t timeDelta)
{
    RigidBody* data = reinterpret_cast <RigidBody*>(mRigidBodyArray);

    for (int i = 0; i < mMaxSize; ++i)
    {
        int set = 0;
        char* p = reinterpret_cast<char*>(data)+4;

        std::memcpy(&set, p, 4);
        if (set != 0)
        {
            if (data->mMarkedForDeletion) //maybe this is set by an event
            {
                DestroyRigidBody(i);
            }
            else
            {
                data->mPositionWorldCoord.x += data->mDirection.x * data->mSpeed * timeDelta;
                data->mPositionWorldCoord.y += data->mDirection.y * data->mSpeed * timeDelta;
            }
        }

        ++data;
    }

}