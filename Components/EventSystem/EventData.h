#ifndef EVENT_DATA_H
#define EVENT_DATA_H

#include "EventEnums.h"

class GameObject;

class EventData
{
public:
    EventPriority priority;
    //EventData();
    virtual ~EventData(){};
};

class CollisionData : public EventData
{
   //which objects are colliding
    //ptrs? hmmm
public:
    GameObject* obj1;
    GameObject* obj2;

};


#endif