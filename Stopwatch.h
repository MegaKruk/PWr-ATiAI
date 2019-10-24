#include <chrono>

class Stopwatch
{
private:
	long double diff;

public:
	std::chrono::high_resolution_clock::time_point point1;
	std::chrono::high_resolution_clock::time_point point2;

public:
	long double countTimeDiff();
	Stopwatch();
	~Stopwatch();
};

