#pragma once
#include <ctime>

class TIMER
{
private:
	clock_t time;
	
public:
	TIMER() { time = clock(); };
	void set() { time = clock(); }
	clock_t getElapsedTime() { return clock() - time; }

};
