#include "Stopwatch.h"
#include <chrono>

Stopwatch::Stopwatch()
{
	point1 = std::chrono::high_resolution_clock::now();
}

long double Stopwatch::countTimeDiff()
{
	std::chrono::high_resolution_clock::time_point point2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<long double, std::nano> diff = point2 - point1;
	return diff.count();
}

Stopwatch::~Stopwatch()
{
	delete &point1;
}
