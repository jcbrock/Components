#include "Event.h"


Event::~Event()
{
    if (data)
    {
        delete data;
    }
}