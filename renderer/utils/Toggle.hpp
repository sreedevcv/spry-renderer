#pragma once

#include "Timer.hpp"
namespace spry {

class Toggle {
public:
    Toggle();
    Toggle(double duration, bool initialValue);

    void update(double delta);
    bool canToggle() const;
    bool toggle();
    bool getValue() const;

private:
    Timer mTimer;
    bool mValue;
};

}