#pragma once

namespace spry {

class Timer {
public:
    Timer();
    Timer(double duration);

    void setDuration(double duration);
    bool update(double delta);
    void reset();
    bool finished() const;

private:
    double mDuration;
    double mCurrent;
};

}