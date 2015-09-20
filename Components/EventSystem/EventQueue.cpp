#include "EventQueue.h"
#include "Event.h"
#include "EventMemoryPoolManager.h"

#include <vector>
#include <string>
#include <iostream>

extern EventMemoryPoolManager gEventMgr;

EventQueue::EventQueue()
{
    std::memset(mEventQueue, 0, sizeof(Event*)* MAX_QUEUE_SIZE);
}

void EventQueue::Enqueue(Event* evt)
{
    if (!evt)
        return;

    if (mCurrentSize >= MAX_QUEUE_SIZE)
    {
        return; //too big
    }

    mEventQueue[mCurrentSize] = evt;
    ++mCurrentSize;

    //Now insert sort - assumming the list is stored, so just sort the last element
    InsertSortQueue();
}

void EventQueue::PrintDebugLog() const
{
    for (const std::string& message : mDebugEventQueue)
    {
        std::cout << message << std::endl;
    }
}

void EventQueue::LogDebugMessage(const Event& evt)
{
    int index = static_cast<int>(evt.GetType());
    mDebugEventQueue.push_back("Event: " + std::string(EventTypeEnumStrings[index]) + " Frame to execute: " + std::to_string(evt.GetFrameToExecute()));
}


Event* EventQueue::Dequeue()
{
    Event* evt = nullptr;

    if (mCurrentSize > mNextUnprocessedEventIndex)
    {
        evt = mEventQueue[mNextUnprocessedEventIndex];
        ++mNextUnprocessedEventIndex;
        LogDebugMessage(*evt);
    }

    return evt; //TODO - make sure pointers get cleaned up....
}

const Event* EventQueue::Peek()
{
    Event* evt = nullptr;

    if (mCurrentSize > mNextUnprocessedEventIndex)
    {
        evt = mEventQueue[mNextUnprocessedEventIndex];
        LogDebugMessage(*evt);
    }

    return evt; //TODO - make sure pointers get cleaned up....
}

// 1 1 3 4 5 3 2
// 1 1 2 3 4 5 3
// 1 1 2 3 3 4 5
//

void EventQueue::InsertSortQueue()
{
    if (mCurrentSize < 2)
    {
        return;
    }

    int index = mCurrentSize - 1;
    Event* current = mEventQueue[index];

    while (index >= 0)
    {
        Event* next = (index - 1) >= 0 ? mEventQueue[(index - 1)] : nullptr;

        // Just added these variables for readability

    
        bool foundSortedSpot = !next || 
            current->GetFrameToExecute() > next->GetFrameToExecute() ||
            (current->GetFrameToExecute() == next->GetFrameToExecute() && current->GetPriority() <= next->GetPriority());
        
        if (foundSortedSpot)
        {
            bool currentEventIsAlreadySorted = (index == (mCurrentSize - 1));
            if (currentEventIsAlreadySorted)
            {
                return;
            }
            //found our spot, shift all to the right and insert
            Event* temp = current; // COPY

            //mEventQueue.push_back(nullptr); //make sure we have that extra space
            for (int i = mCurrentSize - 1; i > index; --i)
            {
                mEventQueue[i] = mEventQueue[i - 1];
            }
            mEventQueue[index] = temp;
            break;
        }

        --index;
    }
}

// NOTE: I start at the back and iterate forward since the unsorted element is at the back of the list

//TODO - I don't have a pop_front on a vector...and I don't want to shift every time I handle an event, so I gotta use the back?
//well that would mean I gotta shift every time I ADD something (if I add it to the front)
//How is perf if I add to the back, reorder, then pop_back? It will get sorted through almost all of the event queue most time I think,
// and thus cause shifting

// I mean, it is just pointers...not a big deal to shift
// The only way to NOT shift is if it a non contiguous data structure like a LL

// Actually I don't need to shift everytime I pop out, just when I'm done processing, then I do one big shift



// Overwrites processed events
void EventQueue::ClearProcessedEvents()
{
    //assumption - all processed events are at the front, there are no gaps in processed vs unprocessed events
    int shiftAmt = 0; //this is also how many elements I get rid of out of array

    for (int i = 0; i < mCurrentSize; ++i)
    {
        if (!mEventQueue[i]->GetHasBeenProcessed())
        {
            break;
        }
        gEventMgr.DestroyEvent(mEventQueue[i]);
        //delete mEventQueue[i];
        ++shiftAmt;
    }

    if (shiftAmt == 0)
    {
        return; // events have been processed, return early
    }

    for (int i = 0; i < mCurrentSize; ++i)
    {
        if (i + shiftAmt < mCurrentSize)
        {
            mEventQueue[i] = mEventQueue[i + shiftAmt];
        }
        else
        {
            // I mean, if it were a static array, I'd do this...
            mEventQueue[i] = nullptr;
        }
    }

    mCurrentSize -= shiftAmt; 
    mNextUnprocessedEventIndex = 0;
}