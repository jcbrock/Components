#ifndef EVENT_H
#define EVENT_H

#include "EventSystem\EventEnums.h"

//class EventType;
class EventData;

class Event// : public GameObject
{
public:
    Event(){};
    ~Event();

    void DebugPrint() const;
    EventType type;
    EventData* data;
    EventPriority priority;
    unsigned __int64 frameToExecute = 0;
    bool HasBeenProcessed = false;
};


#endif