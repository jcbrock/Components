#ifndef TIMER_H
#define TIMER_H

// Include GLEW
#include <GL/glew.h>

class Timer
{
public:
    Timer();
    ~Timer();

    void Initialize();

    double GetTime() const;
 
private:
    Timer(const Timer&) = delete; //I don't want any copies atm
    Timer(Timer&&) = delete;

private:
  
    //unsigned int mBufferSize;
    bool mInitialized;
    double mCPUFreq = 0;
    __int64 mTimerStart = 0;

};

#endif