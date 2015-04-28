#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

class ComponentManager
{
public:
    virtual void Initialize(unsigned long maxSize) = 0;
    virtual void UpdateSubsystem(size_t timeDelta); //inherit?
    virtual void* CreateComponent(); //returns a handle to RigidBody

    virtual void CleanupDeletions();

    virtual void MarkComponentForDelete(int index);
    virtual void DestroyComponent(unsigned int index);

    virtual void DebugPrint() = 0;

private:
    void* mComponentArray; // space up to page size
    void* freeHeadPtr;
    unsigned int mNumberOfAllocatedBlocks;
    unsigned int mMaxSize;
};

#endif