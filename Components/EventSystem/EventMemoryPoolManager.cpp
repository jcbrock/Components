#include "EventMemoryPoolManager.h"
#include "Event.h"

#include <math.h>
#include <cstring>
#include <assert.h>
#include <new> // needed for placement new

//TODO - issue -> MeshInstance is like 4k bytes cuz it holds bufferData
// so it is like...a page per instance, how do I make that more memory friendly?
void EventMemoryPoolManager::Initialize(unsigned long maxSize)
{
    mMaxSize = floor((double)maxSize / sizeof(Event)); //todo - double?

    mNumberOfAllocatedBlocks = 0;

    // We don't want to actually create the mesh instances yet, so just setup the space
    mEventArray = new char[mMaxSize * sizeof(Event)];
    freeHeadPtr = mEventArray;

    // Not really needed, but nice for debugging

    std::memset(mEventArray, 0, mMaxSize * sizeof(Event));

    Event* nextBlock = reinterpret_cast <Event*>(mEventArray);

    // Set ptr to next free block

    for (unsigned int i = 0; i < mMaxSize - 1; ++i)
    {
        // Copy the actual address of the next address into the block

        Event* currentBlock = nextBlock;
        ++nextBlock;
        std::memcpy(currentBlock, &nextBlock, sizeof(void*));
    }

    std::memset(nextBlock, 0, sizeof(void*));
}

Event* EventMemoryPoolManager::CreateEvent2()
{
    // If there is no room, and we try to create instance, this will assert
    assert(freeHeadPtr);

    // Save location of current free block and update headPtr to next free block

    void* freeBlock = freeHeadPtr;
    std::memcpy(&freeHeadPtr, freeHeadPtr, sizeof(void*));

    ++mNumberOfAllocatedBlocks;

    // Construct a new mesh instance in the block and return it

    //return new Event(); //todo
    return new(freeBlock)Event();
    //return new(freeBlock) Event();
    //return new(freeBlock)MeshInstance();
}

void EventMemoryPoolManager::DestroyEvent(unsigned int index)
{
    assert(mNumberOfAllocatedBlocks != 0);

    // Clear the block
    // There is no "placement delete" - have to cleanup myself
    // http://www.stroustrup.com/bs_faq2.html#placement-delete

    Event* ptr = reinterpret_cast <Event*>(mEventArray); //TODO - can this be a different cast?
    ptr[index].~Event();
    std::memset(&ptr[index], 0, sizeof(Event));

    // Update pointer to the next free block (which is currently the head)

    std::memcpy(&ptr[index], &freeHeadPtr, sizeof(void*));

    // Update head pointer to newly freed block. This means that the most reecently
    // freed block will be the next block to be filled

    freeHeadPtr = &ptr[index];

    --mNumberOfAllocatedBlocks;
}

void EventMemoryPoolManager::DestroyEvent(Event* ptr)
{
    assert(mNumberOfAllocatedBlocks != 0);

    // Clear the block
    // There is no "placement delete" - have to cleanup myself
    // http://www.stroustrup.com/bs_faq2.html#placement-delete

    ptr->~Event();
    std::memset(ptr, 0, sizeof(Event));

    // Update pointer to the next free block (which is currently the head)

    std::memcpy(ptr, &freeHeadPtr, sizeof(void*));

    // Update head pointer to newly freed block. This means that the most reecently
    // freed block will be the next block to be filled

    freeHeadPtr = ptr;

    --mNumberOfAllocatedBlocks;
}

// Checks to see if block is populated. We check some bytes after the
// first 4 (because those bytes point to the next free block) to see
// if they are populated. Obviously this relies on empty blocks being memset
// to zero when they are initialized/freed.
bool EventMemoryPoolManager::IsBlockFilled(Event* block) const
{
    int set = 0;
    char* p = reinterpret_cast<char*>(block)+4;

    std::memcpy(&set, p, 4);

    return set != 0;
}
//
//void EventMemoryPoolManager::UpdateSubsystem(double timeDelta)
//{
//    MeshInstance* data = reinterpret_cast <MeshInstance*>(mMeshInstanceArray);
//
//    for (int i = 0; i < mMaxSize; ++i)
//    {
//        if (IsBlockFilled(data))
//        {
//            if (data->IsMarkedForDeletion()) //maybe this is set by an event
//            {
//                DestroyMeshInstance(i);
//            }
//        }
//
//        ++data;
//    }
//
//}

void EventMemoryPoolManager::DebugPrint()
{
    // pretty dirty with all these casts
    Event* data = reinterpret_cast <Event*>(mEventArray);

    for (int i = 0; i < mMaxSize; ++i)
    {
        if (IsBlockFilled(data))
        {
            (*data).DebugPrint();
        }

        ++data;
    }
}