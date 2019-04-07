#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <string>

class Timer {
    static std::chrono::time_point<std::chrono::steady_clock> startPoint;
public:
    static void start();
    static void stop(std::string msg);    
};

#endif