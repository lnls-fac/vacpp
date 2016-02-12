#ifndef VACPP_COMMON_H
#define VACPP_COMMON_H

#include <atomic>
#include <chrono>
#include <string>
#include <utility>


typedef std::atomic<bool> Flag;
typedef std::pair<std::string, double> PVValuePair;
typedef std::chrono::steady_clock Clock;
typedef std::chrono::steady_clock::time_point TimePoint;

const int SUCCESS = 0;
const int ERROR_DRIVER_ALREADY_STARTED = 1;
const int ERROR_DRIVER_NOT_STARTED = 2;

#endif /* VACPP_COMMON_H */
