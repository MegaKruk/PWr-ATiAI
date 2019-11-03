#include "Stopwatch.h"
#include "SA.h"
#include "TTP.h"
#include "Knapsack.h"
#include "Item.h"

std::vector<int> SA::pathInit(int noOfCities)
{
	calcPath.clear();
	calcPath.resize(noOfCities + 1);
	for (int i = 0; i < noOfCities; i++)
	{
		calcPath[i] = i;
		if (i == noOfCities - 1)
			calcPath[i + 1] = calcPath[0];
	}
	return calcPath;
}

std::vector<int> SA::itemsInit(int noOfItems)
{
	stolenItemsList.clear();
	stolenItemsList.resize(noOfItems + 1);
	for (int i = 0; i < noOfItems; i++)
	{
		stolenItemsList[i] = i;
	}
	return stolenItemsList;
}

float SA::calculateWeight(std::vector<Item> &valuableItemsMatrix, std::vector<int> &stolenItemsList, int noOfItems)
{
	float calcWeight = 0;
	for (int i = 0; i < stolenItemsList.size(); i++)
	{
		calcWeight += valuableItemsMatrix[stolenItemsList[i]].getWeight();
		// couts for debug
		//std::cout << "\nWeight of " <<  i << " is\t" << valuableItemsMatrix[stolenItemsList[i]].getWeight() << std::endl;
	}
	//std::cout << calcWeight;
	return calcWeight;
}

float SA::calculateCost(std::vector<std::vector<float>> &adjacancyMatrix, std::vector<int> &calcPath, int noOfCities)
{
	float tmpCost = 0;
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

float SA::calculateProfit(std::vector<std::vector<float>> &adjacancyMatrix, std::vector<Item> &valuableItemsMatrix, 
						  std::vector<int> &calcPath, std::vector<int> &stolenItemsList, int noOfCities, int noOfItems)
{
	return 0;
}

int SA::solverSA(std::vector<std::vector<float>> &adjacancyMatrix, std::vector<Item> &valuableItemsMatrix, std::vector<int> &calcPath, 
				 std::vector<int> &stolenItemsList, int noOfCities, int noOfItems)
{
	// s - initial candidate
	int currProfit = calculateCost(adjacancyMatrix, calcPath, noOfCities);

	// best = s
	int bestProfit = currProfit;

	std::vector<int> bestPath;
	bestPath.clear();
	bestPath.resize(noOfCities + 1);
	
	std::vector<int> bestItems;
	bestItems.clear();
	bestItems.resize(noOfItems);

	for (int i = 0; i < noOfCities + 1; i++)
	{
		bestPath[i] = calcPath[i];
	}

	for (int i = 0; i < noOfItems + 1; i++)
	{
		bestItems[i] = stolenItemsList[i];
	}

	calculateWeight(valuableItemsMatrix, stolenItemsList, noOfItems);

	// glowna petla
	for (double T = 1; T >= 1E-4; T *= 0.9)
		for (int n = 0; n <= 100 * noOfCities; n++)
		{
			int i = randInt(1, noOfCities - 1);
			int j = randInt(1, noOfCities - 1);
			std::swap(calcPath[i], calcPath[j]);

			// r - tweak attempt
			int newProfit = calculateCost(adjacancyMatrix, calcPath, noOfCities);

			if (newProfit < currProfit || randFraction() < exp((currProfit - newProfit) / T))
			{
				// s = r
				currProfit = newProfit;

				// best = s
				if (currProfit < bestProfit)
				{
					bestProfit = currProfit;
					for (int i = 0; i < noOfCities + 1; i++)
					{
						bestPath[i] = calcPath[i];
					}
				}
			}
			else
				std::swap(calcPath[i], calcPath[j]);
		}

	std::cout << "\nProfit:\t" << bestProfit << std::endl;
	std::cout << "Path:\t";
	for (int i = 0; i < noOfCities + 1; i++)
	{
		std::cout << bestPath[i] << "\t";
	}
	bestPath.clear();
	return bestProfit;
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

std::vector<int> SA::getCalcPath(void)
{
	return std::vector<int>(calcPath);
}

std::vector<int> SA::getStolenItemsList(void)
{
	return std::vector<int>(stolenItemsList);
}

SA::SA()
{

}

SA::~SA()
{

}

