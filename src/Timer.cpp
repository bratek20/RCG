#include "Timer.h"

#include <iostream>

using namespace std;
using namespace chrono;

time_point<steady_clock> Timer::startPoint;
string Timer::measureName;

void Timer::start(string name) {
	startPoint = chrono::steady_clock::now();
    measureName = name;
    cout << "Time measure for " << name << endl;
}

void Timer::stop() {
    auto end = chrono::steady_clock::now();

	std::chrono::duration<double> elapsedSeconds = end-startPoint;
    cout << measureName << " took " << elapsedSeconds.count() << " seconds" << endl; 
}