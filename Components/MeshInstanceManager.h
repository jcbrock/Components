#ifndef MESH_INSTANCE_MANAGER_H
#define MESH_INSTANCE_MANAGER_H

class MeshInstance;

class MeshInstanceManager //inherit?
{

public:
    void Initialize(unsigned long maxSize);
    void UpdateSubsystem(float timeDelta); //inherit?
    MeshInstance* CreateMeshInstance(); //returns a handle to MeshInstance
    
    void CleanupDeletions();

    void MarkMeshInstanceForDelete(int index);
    void DestroyMeshInstance(unsigned int index);
    
    void DebugPrint();
private:

    void* mMeshInstanceArray; //space for 8 RBs for now.
    void* freeHeadPtr;
    unsigned int mNumberOfAllocatedBlocks;
    unsigned int mMaxSize;
};

#endif