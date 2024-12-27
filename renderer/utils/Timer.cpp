#include "Timer.hpp"

spry::Timer::Timer()
    : mDuration(0.0f)
{
}

spry::Timer::Timer(double duration)
    : mDuration(duration)
{
}

void spry::Timer::setDuration(double duration)
{
    mDuration = duration;
}

bool spry::Timer::update(double delta)
{
    mCurrent += delta;
    return mCurrent >= mDuration;
}

void spry::Timer::reset()
{
    mCurrent = 0;
}

bool spry::Timer::finished() const
{
    return mCurrent >= mDuration;
}
