#include "Item.h"
#include <iostream>

Item::Item()
{}

Item::~Item()
{}

void Item::DebugPrint() const
{
    std::cout << "[Item]" << std::endl;
}