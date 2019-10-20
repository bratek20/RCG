#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <string>

class Timer {
    static std::chrono::time_point<std::chrono::steady_clock> startPoint;
    static std::string measureName;
    
public:
    static void start(std::string name);
    static void stop();    
};

#endif