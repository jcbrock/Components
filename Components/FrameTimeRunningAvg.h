#ifndef FRAME_TIME_RUNNING_AVG
#define FRAME_TIME_RUNNING_AVG

#include <stdint.h>

class FrameTimeRunningAvg
{
public:
    FrameTimeRunningAvg(uint8_t maxNumOfElements);
    ~FrameTimeRunningAvg();

    //other constructors?
    FrameTimeRunningAvg() = delete;
    FrameTimeRunningAvg(const FrameTimeRunningAvg&) = delete;
    // FrameTimeRunningAvg&(FrameTimeRunningAvg&&) = delete;

    void AddElement(double element);
    double GetRunningAvg() const;

private:
    double mRunningAvg = 0;
    double mCurrentSum = 0; //shit, how much error are these fp's going to have
    double* mData;
    uint8_t mCurrentNumOfElements = 0;
    uint8_t mMaNumberOfElements = 0;
    uint8_t currentIndex = 0;

    uint32_t zeroFrames = 0;
    uint32_t framesUnder5 = 0;
    uint32_t framesUnder15 = 0;
    uint32_t frames15to20 = 0;
    uint32_t framesOver20 = 0;
};


#endif