#include "Timer.h"
#include <iostream>
#include <cstring> //for std::memset
#include <assert.h>

#include <Windows.h>

Timer::Timer() : mInitialized(false)
{
}

Timer::~Timer()
{
    if (mInitialized)
    {
        std::cout << "Timer destroyed" << std::endl;
      //  glDeleteTimers(1, &mTimerHandle);

        // No need to call delete - was holding data on the stack
        //delete[] mTimerData;
    }
}

void Timer::Initialize()
{
    LARGE_INTEGER li;

    assert(QueryPerformanceFrequency(&li));

    mCPUFreq = double(li.QuadPart) / 1000.0;

    QueryPerformanceCounter(&li);
    mTimerStart = li.QuadPart;
    
}

double Timer::GetTime() const
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return double(li.QuadPart - mTimerStart) / mCPUFreq;
}