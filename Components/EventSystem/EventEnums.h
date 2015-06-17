#ifndef EVENT_ENUMS_H
#define EVENT_ENUMS_H

enum class EventType : int
{
    UP_ARROW_PRESSED,
    UP_ARROW_RELEASED,
    DOWN_ARROW_PRESSED,
    DOWN_ARROW_RELEASED, //SHOULD THESE BE MORE GAME EVENTS?
    MOVE_PADDLE,
};

enum class EventPriority : int
{
    IMMEDIATE,
    HIGH,
    MEDIUM,
    LOW
};

#endif