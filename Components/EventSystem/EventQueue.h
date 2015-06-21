#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include "EventEnums.h"
#include <vector>
#include <string>

class Event;

class EventQueue
{
public:
    EventQueue();
    //~EventQueue(){};

    // Will add and sort into the right spot
    void Enqueue(Event* evt); // gotta duplicate, or can I pillage?
    Event* Dequeue();

    bool IsEmpty() const { mCurrentSize == 0; };
    void ClearProcessedEvents();
    //Event* GetEvent(int index) const { return mEventQueue[index]; };
    
   
private:
    void InsertSortQueue();
private:
    int mNextUnprocessedEventIndex = 0;
    int mCurrentSize = 0;
    Event* mEventQueue[10];
    std::vector<std::string> mDebugEventQueue;
};

#endif