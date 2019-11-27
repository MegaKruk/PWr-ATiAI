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

std::vector<bool> GA::itemsInit(int noOfItems, Knapsack& knapsack, std::vector<Item> &valuableItemsMatrix)
{
	knapsack.setCurrWeight(0);
	std::vector<bool> stolenItemsList;
	stolenItemsList.resize(noOfItems);
	for (int i = 0; i < noOfItems; i++)
	{
		stolenItemsList[i] = true;
	}
	while(calculateWeight(valuableItemsMatrix, stolenItemsList, stolenItemsList.size()) > knapsack.getMaxWeight())
	{
		//std::swap(stolenItemsList[rand() % stolenItemsList.size()], stolenItemsList.back());
		//stolenItemsList.pop_back();
		int temp = rand() % stolenItemsList.size();
		stolenItemsList[temp] = false;
		// cout for debug
		//std::cout << "\n Dropped item[" << temp << "]\n";
	}
	return stolenItemsList;
}

float GA::calculateWeight(std::vector<Item> &valuableItemsMatrix, std::vector<bool> &stolenItemsList, int noOfItems)
{
	float calcWeight = 0;
	for (int i = 0; i < stolenItemsList.size(); i++)
	{
		if (stolenItemsList[i] == true)
		{
			calcWeight += valuableItemsMatrix[i].getWeight();
			// couts for debug
			//std::cout << "\nWeight of " << i << " is\t" << valuableItemsMatrix[i].getWeight() << std::endl;
		}
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
						  std::vector<int> &calcPath, std::vector<bool> &stolenItemsList, int noOfCities, int noOfItems, 
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
			if(calcPath[i] == valuableItemsMatrix[j].getAssignedCity() && stolenItemsList[j] == true)
			{
				knapsack.increaseCurrWeight(valuableItemsMatrix[j].getWeight());
				knapsack.setCurrSpeed();
				calcProfit += valuableItemsMatrix[j].getProfit();
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
				 std::vector<bool> &stolenItemsList, int noOfCities, int noOfItems, Knapsack& knapsack)
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

// getters
std::vector<Item> GA::getItemsFromCurrCity(int currCity, std::vector<Item> allItems) 
{
	std::vector<Item> currItems;
	for (int i = 0; i < allItems.size(); i++) 
	{
		if (allItems[i].getAssignedCity() == currCity)
			currItems.push_back(allItems[i]);
	}
	return currItems;
}

std::vector<int> GA::getCalcPath()
{
	return std::vector<int>(calcPath);
}

std::vector<bool> GA::getStolenItemsList()
{
	return std::vector<bool>(stolenItemsList);
}

std::vector<std::tuple<std::vector<int>, std::vector<bool>>> GA::getParentsPop(void)
{
	return std::vector<std::tuple<std::vector<int>, std::vector<bool>>>(parentsPop);
}

std::vector<std::tuple<std::vector<int>, std::vector<bool>>> GA::getChildrenPop(void)
{
	return std::vector<std::tuple<std::vector<int>, std::vector<bool>>>(childrenPop);
}

std::tuple<std::vector<int>, std::vector<bool>> GA::getPopMember(void)
{
	return std::tuple<std::vector<int>, std::vector<bool>>(popMember);
}

int GA::getPopSize()
{
	return popSize;
}

int GA::getNoOfGenerations()
{
	return noOfGenerations;
}

float GA::getCrossRatio()
{
	return crossRatio;
}

float GA::getMutRatio()
{
	return mutRatio;
}

// setters
void GA::setMutationRatio(float val)
{
	mutRatio = val;
}

void GA::setCrossoverRatio(float val)
{
	crossRatio = val;
}

void GA::setPopSize(int val)
{
	popSize = val;
}

void GA::setNoOfGenerations(int val)
{
	noOfGenerations = val;
}

GA::GA()
{

}

GA::~GA()
{

}

