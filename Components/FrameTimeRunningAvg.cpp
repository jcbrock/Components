#include <iostream>
#include "FrameTimeRunningAvg.h"

FrameTimeRunningAvg::FrameTimeRunningAvg(uint8_t maxNumOfElements) : mMaNumberOfElements(maxNumOfElements)
{
    mData = new double[mMaNumberOfElements];
    std::memset(mData, 0, sizeof(double)* mMaNumberOfElements);
}

FrameTimeRunningAvg::~FrameTimeRunningAvg()
{
    delete[] mData;
}

void FrameTimeRunningAvg::AddElement(double element)
{
    // Since this is a running average of frame time ms, we're assuming very small
    // numbers (i.e. under 5000ms for sure). If it is over 5000ms, I probably put
    // a breakpoint and am debugging - so just discard.

    // element should never be less than 0, otherwise my currentime - previoustime is really f'd up
    if (element < 0 || element > 5000)
    {
        return;
    }

    //so increasing 1 bit means I double the space of my previous
    // thus the first bit increase, I can store 2 of my previous max number
    // the next bit increase, I can store 4 of the original max number
    // etc..
    // 1 byte increase means I can store 2^8 (256) times of the original max
    // 2 byte increase means I can store 2^16 times of the original max
    // so int32 can store 2^16 of MAX_INT16
    // so if I store int16 in the array, and only have like 10 elements,
    // then I need like 3 extra bits for my sum total to never overflow


    // Again, since I'm getting rid of high numbers, I know the sum isn't going to overflow.

    if (mCurrentNumOfElements == mMaNumberOfElements)
    {
        mCurrentSum -= mData[currentIndex];
        mData[currentIndex] = (double)element;
        mCurrentSum += mData[currentIndex];
    }
    else
    {
        mData[currentIndex] = (double)element;
        mCurrentSum += mData[currentIndex];
        ++mCurrentNumOfElements;
    }

    mRunningAvg = mCurrentSum / mCurrentNumOfElements;

    if (currentIndex + 1 == mMaNumberOfElements)
        currentIndex = 0;
    else
        ++currentIndex;

    if (element < 0.000001)
        ++zeroFrames;
    else if (element < 5)
        ++framesUnder5;
    else if (element >= 5 && element < 15)
        ++framesUnder15;
    else if (element >= 15 && element < 20)
        ++frames15to20;
    else
        ++framesOver20;

    static unsigned int framesUntilPrint = 100;
    --framesUntilPrint;
    if (framesUntilPrint == 0)
    {
        std::cout << "zeroframes: " << zeroFrames << std::endl;
        std::cout << "framesUnder5: " << framesUnder5 << std::endl;
        std::cout << "frames5to15:  " << framesUnder15 << std::endl;
        std::cout << "frames15to20:  " << frames15to20 << std::endl;
        std::cout << "framesOver20:  " << framesOver20 << std::endl;
        std::cout << "Average frame time (ms): " << mRunningAvg << std::endl;
        std::cout << "FPS: " << 1000 / mRunningAvg << std::endl;
        framesUntilPrint = 10000;
    }
}

double FrameTimeRunningAvg::GetRunningAvg() const
{
    return mRunningAvg;
}
