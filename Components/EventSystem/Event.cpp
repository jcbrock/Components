#include "Event.h"

#include <iostream>

Event::~Event()
{
    if (data)
    {
        delete data;
    }
}

void Event::DebugPrint() const
{
    std::cout << 
        "   EventType: " << std::string(EventTypeEnumStrings[static_cast<int>(type)]).c_str() << std::endl <<
        "	EventPriority: " << std::string(EventPriorityEnumStrings[static_cast<int>(priority)]).c_str() << std::endl <<
        "	frameToExecute: " << frameToExecute << std::endl <<
        "	HasBeenProcessed: " << HasBeenProcessed << std::endl << std::endl;
}