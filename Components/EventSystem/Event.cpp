#include "Event.h"
#include "EventData.h"

#include <iostream>

Event::~Event()
{
    if (mData)
    {
        delete mData;
    }
}

void Event::Initialize(EventType type, EventData* data, EventPriority priority, uint32_t frameToExecute)
{
    mType = type;
    mData = data;
    mPriority = priority;
    mFrameToExecute = frameToExecute;
}

void Event::DebugPrint() const
{
    std::cout <<
        "   EventType: " << std::string(EventTypeEnumStrings[static_cast<int>(mType)]).c_str() << std::endl <<
        "	EventPriority: " << std::string(EventPriorityEnumStrings[static_cast<int>(mPriority)]).c_str() << std::endl <<
        "	frameToExecute: " << mFrameToExecute << std::endl <<
        "	HasBeenProcessed: " << mHasBeenProcessed << std::endl << 
        std::endl;
}