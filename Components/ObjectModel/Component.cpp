#include "Component.h"
#include <iostream>
#include <assert.h>

//#include "common\LoggingDefines.h"

Component::Component()
{
    std::cout << "Component constructor!" << std::endl;
}

Component::~Component()
{
    std::cout << "Component destructor!" << std::endl;
}

Component::Component(const Component& obj)
{
    std::cout << "Component copy constructor!" << std::endl;
}

Component& Component::operator=(const Component& obj)
{
    std::cout << "Component assignment operator!" << std::endl;
    return *this;
}

Component::Component(Component&& obj)
{
    std::cout << "Component move constructor!" << std::endl;
    //return *this;
}

void Component::Initialize()
{
    strcpy(mName, "Unknown");
}

void Component::SetName(const std::string& name)
{
    assert(name.length() < 15);

    strcpy(mName, name.c_str());
}

std::string Component::GetName() const
{
    return std::string(mName);
}


bool Component::IsMarkedForDeletion() const
{
    return mMarkedForDeletion;
}