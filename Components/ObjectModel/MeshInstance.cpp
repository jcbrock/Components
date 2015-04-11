#include "MeshInstance.h"
#include <iostream>

MeshInstance::MeshInstance()
{
    std::cout << "MeshInstance constructor!" << std::endl;
}

MeshInstance::~MeshInstance()
{
    std::cout << "MeshInstance destructor!" << std::endl;
}

MeshInstance::MeshInstance(const MeshInstance& obj)
{
    std::cout << "MeshInstance copy constructor!" << std::endl;
}

MeshInstance& MeshInstance::operator=(const MeshInstance& obj)
{
    std::cout << "MeshInstance assignment operator!" << std::endl;
    return *this;
}