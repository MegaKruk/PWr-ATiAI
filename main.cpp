/*************************************/
/******Traveling Thief Problem********/
/*************************************/
/**********Greedy Algorithm***********/
/*********Simulated Annealing*********/
/**********Genetic Algorithm**********/
/*************************************/
/***********Implemented by************/
/*Filip Mazur*Dawid Ryl*Piotr Neumann*/
/*************************************/

#include "TTP.h"
#include <iostream>

int main()
{
	srand(time(0));
	TTP *testUnit = new TTP();
	testUnit->start();
	delete(testUnit);
	std::cin.ignore(1);
	return 0;
}
