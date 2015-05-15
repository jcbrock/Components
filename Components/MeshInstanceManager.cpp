#include "MeshInstanceManager.h"
#include "ObjectModel\MeshInstance.h"

#include <math.h>
#include <cstring>
#include <assert.h>

//TODO - issue -> MeshInstance is like 4k bytes cuz it holds bufferData
// so it is like...a page per instance, how do I make that more memory friendly?
void MeshInstanceManager::Initialize(unsigned long maxSize)
{
    mMaxSize = floor(maxSize / sizeof(MeshInstance));

    mNumberOfAllocatedBlocks = 0;

    // We don't want to actually create the mesh instances yet, so just setup the space
    mMeshInstanceArray = new char[mMaxSize * sizeof(MeshInstance)];
    freeHeadPtr = mMeshInstanceArray;

    // Not really needed, but nice for debugging

    std::memset(mMeshInstanceArray, 0, mMaxSize * sizeof(MeshInstance));

    MeshInstance* nextBlock = reinterpret_cast <MeshInstance*>(mMeshInstanceArray);

    // Set ptr to next free block

    for (unsigned int i = 0; i < mMaxSize - 1; ++i)
    {
        // Copy the actual address of the next address into the block

        MeshInstance* currentBlock = nextBlock;
        ++nextBlock;
        std::memcpy(currentBlock, &nextBlock, sizeof(void*));
    }

    std::memset(nextBlock, 0, sizeof(void*));
}

MeshInstance* MeshInstanceManager::CreateMeshInstance()
{
    // If there is no room, and we try to create instance, this will assert
    assert(freeHeadPtr);

    // Save location of current free block and update headPtr to next free block

    void* freeBlock = freeHeadPtr;
    std::memcpy(&freeHeadPtr, freeHeadPtr, sizeof(void*));

    ++mNumberOfAllocatedBlocks;

    // Construct a new mesh instance in the block and return it

    return new(freeBlock)MeshInstance();
}

void MeshInstanceManager::DestroyMeshInstance(unsigned int index)
{
    assert(mNumberOfAllocatedBlocks != 0);

    // Clear the block
    // There is no "placement delete" - have to cleanup myself
    // http://www.stroustrup.com/bs_faq2.html#placement-delete

    MeshInstance* ptr = reinterpret_cast <MeshInstance*>(mMeshInstanceArray); //TODO - can this be a different cast?
    ptr[index].~MeshInstance();
    std::memset(&ptr[index], 0, sizeof(MeshInstance));

    // Update pointer to the next free block (which is currently the head)

    std::memcpy(&ptr[index], &freeHeadPtr, sizeof(void*));

    // Update head pointer to newly freed block. This means that the most reecently
    // freed block will be the next block to be filled

    freeHeadPtr = &ptr[index];

    --mNumberOfAllocatedBlocks;
}

// Checks to see if block is populated. We check some bytes after the
// first 4 (because those bytes point to the next free block) to see
// if they are populated. Obviously this relies on empty blocks being memset
// to zero when they are initialized/freed.
bool MeshInstanceManager::IsBlockFilled(MeshInstance* block) const
{
    int set = 0;
    char* p = reinterpret_cast<char*>(block)+4;

    std::memcpy(&set, p, 4);

    return set != 0;
}

void MeshInstanceManager::UpdateSubsystem(double timeDelta)
{
    MeshInstance* data = reinterpret_cast <MeshInstance*>(mMeshInstanceArray);

    for (int i = 0; i < mMaxSize; ++i)
    {
        if (IsBlockFilled(data))
        {
            if (data->IsMarkedForDeletion()) //maybe this is set by an event
            {
                DestroyMeshInstance(i);
            }
        }

        ++data;
    }

}

void MeshInstanceManager::DebugPrint()
{
    // pretty dirty with all these casts
    MeshInstance* data = reinterpret_cast <MeshInstance*>(mMeshInstanceArray);

    for (int i = 0; i < mMaxSize; ++i)
    {
        if (IsBlockFilled(data))
        {
            (*data).DebugPrint();
        }

        ++data;
    }
}