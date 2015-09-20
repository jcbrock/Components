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

    // Adds event to the queue and sorts it to the right position based on FrameToExcute and Priority
    void Enqueue(Event* evt);

    // Gets the next event to be processed
    Event* Dequeue();

    // Gets the next event to be processed, but does not assume it has been pulled out of the array for processing
    const Event* Peek();

    // Iterates through the queue and cleans up processed events
    void ClearProcessedEvents();

    bool IsEmpty() const { return mCurrentSize == 0; };
   
private:
    void InsertSortQueue();
    void LogDebugMessage(const Event& evt);
    void PrintDebugLog() const;
private:
    static const unsigned short MAX_QUEUE_SIZE = 10;
    int mNextUnprocessedEventIndex = 0;
    int mCurrentSize = 0;

    // Creation of these pointers is not the responsibility of the EventQueue 
    // (someday they'll come from a memory pool for events), but deletion happens in ClearProcessedEvents
    Event* mEventQueue[MAX_QUEUE_SIZE];

    std::vector<std::string> mDebugEventQueue;
};

#endif