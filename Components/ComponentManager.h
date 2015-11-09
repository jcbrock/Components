#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H


// This has to be a template thing - if I were to have some sort of polymorphic
// setup I'd have to use pointers a lot, and I don't want to store a list of pointers. I
// want to store the actual objects in a memory page.




//todo - in order to do this, I think I'd need a polymorphic 
//component class. still might have issues with sizing

// so I could just use this as a contract, not as impl.
// or could I maybe use templates?
// todo - LEFT OFF HERE

// I'm running into questions like, if I have polymomrphic component
// class that gets DebugPrint called, how does it know the right function?
// I know vtable, but how is the object stored? How does the memory work
// if I'm storing a list of components? Like what if the actual class is huge?
// Oh... that is why they need to be pointers :/  fuuuuuuu

template <typename T>
class ComponentManager
{
public:
    void Initialize(unsigned long maxSize);
    void UpdateSubsystem(double timeDelta);
    T* CreateComponent();
    void MarkComponentForDelete(T* component);


    void DebugPrint();

private:
    bool IsBlockFilled(T* block) const;
    void DestroyComponent(unsigned int index);

private:
    void* mComponentArray; // space up to page size
    void* freeHeadPtr;
    unsigned int mNumberOfAllocatedBlocks;
    unsigned int mMaxSize;
};


// Checks to see if block is populated. We check some bytes after the
// first 4 (because those bytes point to the next free block) to see
// if they are populated. Obviously this relies on empty blocks being memset
// to zero when they are initialized/freed.
template <typename T>
bool ComponentManager<T>::IsBlockFilled(T* block) const
{
    int set = 0;
    char* p = reinterpret_cast<char*>(block)+4;

    std::memcpy(&set, p, 4);

    return set != 0;
}

template <typename T>
void ComponentManager<T>::DebugPrint()
{
    // pretty dirty with all these casts
    T* data = reinterpret_cast <T*>(mComponentArray);

    for (int i = 0; i < mMaxSize; ++i)
    {
        if (IsBlockFilled(data))
        {
            (*data).DebugPrint();
        }

        ++data;
    }
}
//TODO - issue -> MeshInstance is like 4k bytes cuz it holds bufferData
// so it is like...a page per instance, how do I make that more memory friendly?
template <typename T>
void ComponentManager<T>::Initialize(unsigned long maxSize)
{
    mNumberOfAllocatedBlocks = 0;
    mMaxSize = floor(maxSize / sizeof(T));
    mComponentArray = new T[mMaxSize];
    freeHeadPtr = mComponentArray;

    // Not really needed, but nice for debugging

    std::memset(mComponentArray, 0, mMaxSize * sizeof(T));

    T* nextBlock = reinterpret_cast <T*>(mComponentArray);

    // Set ptr to next free block

    for (unsigned int i = 0; i < mMaxSize - 1; ++i)
    {
        // Copy the actual address of the next address into the block

        T* currentBlock = nextBlock;
        ++nextBlock;
        std::memcpy(currentBlock, &nextBlock, sizeof(void*)); // Assuming 4 byte pointer
    }

    std::memset(nextBlock, 0, sizeof(void*));

}

// I should return a shared_ptr I think. Though, I would need to make
// sure that when the shared ref count goes to 0, DestroyComponent
// gets called.
// Having a shared_ptr will make sure that the consumer will be able
// to use the component however they want, and when they are done,
// they don't need to worry about if they are deleting a raw pointer
// that someone else is pointing to.
// Hmm, I still feel like there needs to be another layer between
// this and the consumer. Like a layer where I can request shared/weak ptrs
// to be made properly off the constructed component (i.e. so I don't
// end up in a siuation where I have two shared pointers pointing to the
// same object, but having different control blocks)
//
// So that would still achieve the memory tightness of the objects (i.e.
// for iteration loops), yet make sure things referening those objects
// are safe and can't shoot themselves in the foot.
//
// Would need to think about how destruct component would get called. I guess 
// i'd use a custom deallocator - make_shared def not gunna work
template <typename T>
T* ComponentManager<T>::CreateComponent()
{
    //If there is no room, freeHeadPtr will be null

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
    T* ptr = reinterpret_cast <T*>(aboutToBeAllocated);

    // This...heopfully...blows away w/e was in this ptr
    // again, probably wasteful. Ideally I create the object in the block,
    // not on the stack then copy it...
    T rb;
    rb.Initialize();
    std::memcpy(ptr, &rb, sizeof(rb));

    ++mNumberOfAllocatedBlocks;

    return ptr;
}

template <typename T>
void ComponentManager<T>::MarkComponentForDelete(T* component)
{
    assert(component);

    component->MarkForDeletion();
}

template <typename T>
void ComponentManager<T>::DestroyComponent(unsigned int index)
{
    assert(mNumberOfAllocatedBlocks != 0);

    // Clear the block
    // There is no "placement delete" - have to cleanup myself
    // http://www.stroustrup.com/bs_faq2.html#placement-delete

    T* ptr = reinterpret_cast <T*>(mComponentArray); //TODO - can this be a different cast?
    ptr[index].~T();
    std::memset(&ptr[index], 0, sizeof(T));

    // Update pointer to the next free block (which is currently the head)

    std::memcpy(&ptr[index], &freeHeadPtr, sizeof(void*));

    // Update head pointer to newly freed block. This means that the most reecently
    // freed block will be the next block to be filled

    freeHeadPtr = &ptr[index];

    --mNumberOfAllocatedBlocks;
}

template <typename T>
void ComponentManager<T>::UpdateSubsystem(double timeDelta)
{
    T* data = reinterpret_cast <T*>(mComponentArray);

    for (int i = 0; i < mMaxSize; ++i)
    {
        if (IsBlockFilled(data))
        {
            if (data->IsMarkedForDeletion()) //maybe this is set by an event
            {
                LOG_SIMPLE("Deleting component!");
                DestroyComponent(i);
            }
            else
            {
                data->Update(timeDelta); //right? errr, each object have an update func?
                //LOG_SIMPLE("Updating component!");


            }
        }

        ++data;
    }
}

#endif