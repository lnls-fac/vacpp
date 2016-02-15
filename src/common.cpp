#include "common.h"


void _wait_interval_from(const std::chrono::milliseconds& interval,
    const TimePoint& start_time)
{
    const auto delta = Clock::now() - start_time;
    if (delta < interval)
        std::this_thread::sleep_for(interval-delta);
}
