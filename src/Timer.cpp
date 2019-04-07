#include "Timer.h"

#include <iostream>

using namespace std;
using namespace chrono;

time_point<steady_clock> Timer::startPoint;

void Timer::start() {
	startPoint = chrono::steady_clock::now();
}

void Timer::stop(string msg) {
    auto end = chrono::steady_clock::now();

	std::chrono::duration<double> elapsedSeconds = end-startPoint;
    cout << msg << " took " << elapsedSeconds.count() << " seconds" << endl; 
}