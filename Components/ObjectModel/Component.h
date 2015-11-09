#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>
class Component
{

public:
    Component();
    ~Component();
    Component(const Component&);
    Component& operator=(const Component&);
    Component(Component&&);

    void MarkForDeletion() { mMarkedForDeletion = true; }

    virtual void Initialize();
    virtual void DebugPrint() = 0;
    virtual void Update(double timeDelta) = 0;

    // Getters / Setters

    void SetName(const std::string& name);

    std::string GetName() const;
    bool        IsMarkedForDeletion() const;

private:
    bool mMarkedForDeletion = false;
    char mName[16]; //stack 15 bytes
};

#endif