#ifndef LOGGING_DEFINES_H
#define LOGGING_DEFINES_H

#include <stdarg.h>

//TODO - replace std::cout with my LOG statement

const size_t kMaxLogStatementSize = 20480;

// Purely doing this macro / variable argument / template stuff to see if I could

template <const size_t size>
void FormatMessage(char(&ary)[size], const char * messageFormat, va_list args)
{
    vsnprintf_s(ary, size, messageFormat, args);
}

void LogMessage(int sev, const char * messageFormat, ...)
{
    va_list args;
    va_start(args, messageFormat);
    char formattedMessage[kMaxLogStatementSize];
    FormatMessage(formattedMessage, messageFormat, args);

    std::cout << formattedMessage << std::endl; //TODO - write to a log and console
    va_end(args);
}

#define LOG_SIMPLE_SEV(sev, messageFormat, ...){ LogMessage(sev, messageFormat, ##__VA_ARGS__);}
#define LOG_SIMPLE(messageFormat, ...){ LogMessage(0, messageFormat, ##__VA_ARGS__);}

#endif //LOGGING_DEFINES_H