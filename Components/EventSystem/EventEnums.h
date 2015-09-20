#ifndef EVENT_ENUMS_H
#define EVENT_ENUMS_H

enum class EventType : int
{
    UP_ARROW_PRESSED,
    UP_ARROW_RELEASED,
    DOWN_ARROW_PRESSED,
    DOWN_ARROW_RELEASED, //SHOULD THESE BE MORE GAME EVENTS?
    MOVE_PADDLE,
    PRINT_DEBUG,
};

// For debugging
static const char * EventTypeEnumStrings[] = { 
    "UP_ARROW_PRESSED", 
    "UP_ARROW_RELEASED", 
    "DOWN_ARROW_PRESSED",
    "DOWN_ARROW_RELEASED", 
    "MOVE_PADDLE",
    "PRINT_DEBUG",
};

enum class EventPriority : int
{
    LOW,
    MEDIUM,
    HIGH,
};

// For debugging
static const char * EventPriorityEnumStrings[] = {
    "LOW",
    "MEDIUM",
    "HIGH",
};

#endif