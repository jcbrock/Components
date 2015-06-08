#ifndef EVENT_H
#define EVENT_H

#include "EventSystem\EventEnums.h"

//class EventType;
class EventData;

class Event// : public GameObject
{
public:
   // Item();
   // ~Item() override;

   // void DebugPrint() const;
    EventType type;
    EventData* data;
};


#endif