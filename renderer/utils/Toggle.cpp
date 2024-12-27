#include "Toggle.hpp"

spry::Toggle::Toggle()
    : mTimer(500e-3)
    , mValue(true)
{
}

spry::Toggle::Toggle(double duration, bool initialValue)
    : mTimer(duration)
    , mValue(initialValue)
{
}

void spry::Toggle::update(double delta)
{
    mTimer.update(delta);
}

bool spry::Toggle::canToggle() const
{
    return mTimer.finished();
}

bool spry::Toggle::toggle()
{
    mTimer.reset();
    mValue = !mValue;
    return mValue;
}

bool spry::Toggle::getValue() const
{
    return mValue;
}