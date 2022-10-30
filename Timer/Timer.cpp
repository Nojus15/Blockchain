#include "Timer.h"

void Timer::Start()
{
    Start_point = hrClock::now();
}
double Timer::Stop()
{
    return durationDouble(hrClock::now() - Start_point).count();
}