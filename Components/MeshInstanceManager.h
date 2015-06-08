#ifndef MESH_INSTANCE_MANAGER_H
#define MESH_INSTANCE_MANAGER_H

class MeshInstance;

class MeshInstanceManager //inherit?
{

public:
    void Initialize(unsigned long maxSize);
    void UpdateSubsystem(double timeDelta); //inherit?
    MeshInstance* CreateMeshInstance(); //returns a handle to MeshInstance
    
    void MarkMeshInstanceForDelete(int index);
    void DestroyMeshInstance(unsigned int index);
    
    void DebugPrint();

private:
    bool IsBlockFilled(MeshInstance* block) const;

private:

    void* mMeshInstanceArray; //space for 8 RBs for now.
    void* freeHeadPtr;
    unsigned int mNumberOfAllocatedBlocks;
    unsigned int mMaxSize;
};

#endif