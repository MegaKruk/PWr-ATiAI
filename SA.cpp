#include "Stopwatch.h"
#include "SA.h"
#include "TTP.h"
#include "Knapsack.h"
#include "Item.h"

int SA::pathInit(std::vector<int> &calcPath, int noOfCities)
{
	calcPath.clear();
	calcPath.resize(noOfCities + 1);
	for (int i = 0; i < noOfCities; i++)
	{
		calcPath[i] = i;
		if (i == noOfCities - 1)
			calcPath[i + 1] = calcPath[0];
	}
	return 0;
}

int SA::calculateCost(std::vector<std::vector<int>> &adjacancyMatrix, std::vector<int> &calcPath, int noOfCities)
{
	int tmpCost = 0;
	for (int i = 0; i < noOfCities; i++)
	{
		//std::cout << endl << seq[i] << endl;
		int a = calcPath[i];
		//std::cout << endl << "a= "<< a;
		int b = calcPath[i + 1];
		//std::cout << endl << "b= " << b;
		tmpCost += adjacancyMatrix[a][b % (noOfCities)];

	}
	return tmpCost;
}

int SA::itemsInit()
{
	return 0;
}

int SA::calculateProfit()
{
	return 0;
}

int SA::randInt(int l, int r)
{
	return rand() % (r - l + 1) + l;
}

double SA::randFraction(void)
{
	return randInt(1, 10000) / 10000;
}

std::vector<Item> SA::getItemsFromCurrCity(int currCity, std::vector<Item> allItems) 
{
	std::vector<Item> currItems;
	for (int i = 0; i < allItems.size(); i++) 
	{
		if (allItems[i].getAssignedCity() == currCity)
			currItems.push_back(allItems[i]);
	}
	return currItems;
}

SA::SA()
{

}

SA::~SA()
{

}

