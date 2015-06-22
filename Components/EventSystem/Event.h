#ifndef EVENT_H
#define EVENT_H

#include "EventEnums.h"

#include <stdint.h>

class EventData;

class Event
{
public:
    Event(){};
    ~Event();

    void Initialize(EventType type, EventData* data, EventPriority priority, uint32_t frameToExecute);
    void DebugPrint() const;

    EventType       GetType() const { return mType; };
    EventData*      GetData() const { return mData; };
    EventPriority   GetPriority() const { return mPriority; };
    uint32_t        GetFrameToExecute() const{ return mFrameToExecute; };
    bool            GetHasBeenProcessed() const{ return mHasBeenProcessed; };

    void SetHasBeenProcessed(bool hasBeenProcessed) { mHasBeenProcessed = hasBeenProcessed; };

private:
    EventType mType;
    EventData* mData;
    EventPriority mPriority;
    uint32_t mFrameToExecute = 0;
    bool mHasBeenProcessed = false;
};


#endif