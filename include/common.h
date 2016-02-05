#ifndef VACPP_COMMON_H
#define VACPP_COMMON_H

#include <atomic>
#include <string>
#include <utility>

typedef std::pair<std::string, double> PVValuePair;

extern std::atomic<bool> stop_flag;


#endif /* VACPP_COMMON_H */
