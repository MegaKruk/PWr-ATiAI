#include "Stopwatch.h"
#include "TTP.h"

TTP::TTP()
{

}

TTP::~TTP()
{

}

int TTP::initTTP()
{
	Stopwatch *timer = new Stopwatch();
	timer->point1 = std::chrono::high_resolution_clock::now();
	for(int i = 0; i < 10; i++)
	{
		std::cout << "yo wassup " << i << std::endl;
	}
	std::cout << std::endl << timer->countTimeDiff() << " nanosecs to complete this action\n";
	return 0;
}