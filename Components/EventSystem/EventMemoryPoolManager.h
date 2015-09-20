#ifndef EVENT_MEMORY_POOL_MANAGER_H
#define EVENT_MEMORY_POOL_MANAGER_H

class Event;

class EventMemoryPoolManager //inherit?
{

public:
    void Initialize(unsigned long maxSize);
    //void UpdateSubsystem(double timeDelta); //inherit?
    Event* CreateEvent2(); //returns a handle to MeshInstance
    
    //void MarkMeshInstanceForDelete(int index);
    void DestroyEvent(unsigned int index);
    void DestroyEvent(Event* evt);
    
    void DebugPrint();

private:
    bool IsBlockFilled(Event* block) const;

private:

    void* mEventArray; //space for 8 RBs for now.
    void* freeHeadPtr;
    unsigned int mNumberOfAllocatedBlocks;
    unsigned int mMaxSize;
};

#endif