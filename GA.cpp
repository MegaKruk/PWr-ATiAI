#include "Stopwatch.h"
#include "GA.h"
#include "TTP.h"
#include "Knapsack.h"
#include "Item.h"

std::vector<int> GA::pathInit(int noOfCities)
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

std::vector<int> GA::itemsInit(int noOfItems, Knapsack& knapsack, std::vector<Item> &valuableItemsMatrix)
{
	knapsack.setCurrWeight(0);
	std::vector<int> stolenItemsList;
	stolenItemsList.resize(noOfItems);
	for (int i = 0; i < noOfItems; i++)
	{
		stolenItemsList[i] = i;
	}
	while(calculateWeight(valuableItemsMatrix, stolenItemsList, stolenItemsList.size()) > knapsack.getMaxWeight())
	{
		std::swap(stolenItemsList[rand() % stolenItemsList.size()], stolenItemsList.back());
		stolenItemsList.pop_back();
	}
	return stolenItemsList;
}

float GA::calculateWeight(std::vector<Item> &valuableItemsMatrix, std::vector<int> &stolenItemsList, int noOfItems)
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

float GA::calculateDist(std::vector<std::vector<float>> &adjacancyMatrix, std::vector<int> &calcPath, int noOfCities)
{
	float tmpDist = 0;
	for (int i = 0; i < noOfCities; i++)
	{
		//std::cout << endl << seq[i] << endl;
		int a = calcPath[i];
		//std::cout << endl << "a= "<< a;
		int b = calcPath[i + 1];
		//std::cout << endl << "b= " << b;
		tmpDist += adjacancyMatrix[a][b % (noOfCities)];

	}
	return tmpDist;
}

float GA::calculateProfit(std::vector<std::vector<float>> &adjacancyMatrix, std::vector<Item> &valuableItemsMatrix, 
						  std::vector<int> &calcPath, std::vector<int> &stolenItemsList, int noOfCities, int noOfItems, 
						  Knapsack& knapsack)
{
	knapsack.setCurrWeight(0);
	knapsack.setCurrSpeed();
	float tmpDist = 0;
	float tmpTime = 0;
	float calcProfit = 0;
	for(int i = 0; i < noOfCities; i++)
	{
		for(int j = 0; j < stolenItemsList.size(); j++)
		{
			if(calcPath[i] == valuableItemsMatrix[stolenItemsList[j]].getAssignedCity())
			{
				knapsack.increaseCurrWeight(valuableItemsMatrix[stolenItemsList[j]].getWeight());
				knapsack.setCurrSpeed();
				calcProfit += valuableItemsMatrix[stolenItemsList[j]].getProfit();
			}
		}
		int matrixX = calcPath[i];
		int matrixY = calcPath[i + 1];
		tmpDist += adjacancyMatrix[matrixX][matrixY % noOfCities];
		tmpTime += knapsack.getSpeed() / tmpDist;
		calcProfit -= tmpTime * knapsack.getRentingRatio();
	}
	return calcProfit;
}

int GA::solverGA(std::vector<std::vector<float>> &adjacancyMatrix, std::vector<Item> &valuableItemsMatrix, std::vector<int> &calcPath, 
				 std::vector<int> &stolenItemsList, int noOfCities, int noOfItems, Knapsack& knapsack)
{
	// 1. Initialize population[popSize] where member is set of items and a path
	// 2. Assign them a fitness (only profit matters, it describes everything). 
	// 3. Save the best member as current best solution if it's better than current best
	// 4. Make the fight (roulette[size] or tournament[size]). The better the fitness the bigger the chance of winning
	// 5. Two winners mate 
	// 6. Chance for crossover to happen on a child (usually >60%) (1 point crossover, 2 point, 3 point...)
	// 7. Chance for mutation to happen on a child (usually <1%) 
	// (swap 2 random cities, drop 1 item, take 1 item [drop more if we went over max weight])
	// 8. Repeat until new population[popSize] is filled with children
	// 9. Repeat 2-8 until we reach max number of generations (or until bored).
	return 0;
}

int GA::randInt(int l, int r)
{
	return rand() % (r - l + 1) + l;
}

double GA::randFraction(void)
{
	return randInt(1, 10000) / 10000;
}

std::vector<int> GA::getCalcPath(void)
{
	return std::vector<int>(calcPath);
}

std::vector<int> GA::getStolenItemsList(void)
{
	return std::vector<int>(stolenItemsList);
}

GA::GA()
{

}

GA::~GA()
{

}

