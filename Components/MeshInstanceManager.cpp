#include "MeshInstanceManager.h"
#include <math.h>

void MeshInstanceManager::DebugPrint()
{
    // pretty dirty with all these casts
    MeshInstance* data = reinterpret_cast <MeshInstance*>(mMeshInstanceArray);

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

void MeshInstanceManager::Initialize(unsigned long maxSize)
{
    mMaxSize = floor(maxSize / sizeof(MeshInstance));
   
    mNumberOfAllocatedBlocks = 0;
  //  mMaxSize = maxSize;

    //don't want to actually create the mesh instances yet, so just setting space
    mMeshInstanceArray = new char[maxSize * sizeof(MeshInstance)];
    freeHeadPtr = mMeshInstanceArray;

    // Not really needed, but nice for debugging

    std::memset(mMeshInstanceArray, 0, maxSize * sizeof(MeshInstance));

    MeshInstance* nextBlock = reinterpret_cast <MeshInstance*>(mMeshInstanceArray);

    // Set ptr to next free block

    for (unsigned int i = 0; i < mMaxSize - 1; ++i)
    {
        // Copy the actual address of the next address into the block

        MeshInstance* currentBlock = nextBlock;
        ++nextBlock;
        std::memcpy(currentBlock, &nextBlock, sizeof(void*)); // Assuming 4 byte pointer
    }

    std::memset(nextBlock, 0, sizeof(void*));

}


MeshInstance* MeshInstanceManager::CreateMeshInstance()
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

    // Create new MeshInstance in this block
    // Fuck, this is the part about how I don't know if this is possible without custom allocator
    MeshInstance* ptr = reinterpret_cast <MeshInstance*>(aboutToBeAllocated);

    // This...heopfully...blows away w/e was in this ptr
    // again, probably wasteful. Ideally I create the object in the block,
    // not on the stack then copy it...
    MeshInstance mi;
    mi.Initialize();
    std::memcpy(ptr, &mi, sizeof(mi));

    ++mNumberOfAllocatedBlocks;

    return ptr;
}

//TODO - how do I know the index?
void MeshInstanceManager::DestroyMeshInstance(unsigned int index)
{
    if (mNumberOfAllocatedBlocks == 0)
    {
        //assert - TODO
        return;
    }


    // Get pointer to block, free it (i.e. just memset it to 0? That is...a bad idea? Cuz I don't
    // call destructor? Then again, do I... need to?)

    // Clear the block

    MeshInstance* ptr = reinterpret_cast <MeshInstance*>(mMeshInstanceArray);
    std::memset(&ptr[index], 0, sizeof(MeshInstance));

    // Update pointer to the next free block (which is currently the head)

    std::memcpy(&ptr[index], &freeHeadPtr, sizeof(void*));

    // Update head pointer to newly freed block. This means that the most reecently
    // freed block will be the next block to be filled

    freeHeadPtr = &ptr[index];

    --mNumberOfAllocatedBlocks;
}


//OR CALL DestroyMeshInstance during the update loop thru each one
void MeshInstanceManager::CleanupDeletions()
{
    for (int i = 0; i < mMaxSize; ++i)
    {
        MeshInstance* data = reinterpret_cast <MeshInstance*>(mMeshInstanceArray);

        int set = 0;
        char* p = reinterpret_cast<char*>(data)+4;

        std::memcpy(&set, p, 4);
        if (set != 0 && data->mMarkedForDeletion)
        {
            DestroyMeshInstance(i);
        }

        ++data;
    }
}

void MeshInstanceManager::UpdateSubsystem(float timeDelta)
{
    MeshInstance* data = reinterpret_cast <MeshInstance*>(mMeshInstanceArray);

    for (int i = 0; i < mMaxSize; ++i)
    {
        int set = 0;
        char* p = reinterpret_cast<char*>(data)+4;

        std::memcpy(&set, p, 4);
        if (set != 0)
        {
            if (data->mMarkedForDeletion) //maybe this is set by an event
            {
                DestroyMeshInstance(i);
            }
        }

        ++data;
    }

}