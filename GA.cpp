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

	for (int i = 0; i < 100 * noOfCities; i++)
	{
		int x = randNum(1, noOfCities - 1);
		int y = randNum(1, noOfCities - 1);
		std::swap(calcPath[x], calcPath[y]);
	}

	/*for (int l = 0; l < calcPath.size(); l++)
	{
		std::cout << calcPath[l] << "\t";
	}
	std::cout << std::endl;*/

	return calcPath;
	/*for (int i = 0; i < noOfCities; i++)
	{
		calcPath[i] = i;
		if (i == noOfCities - 1)
			calcPath[i + 1] = calcPath[0];
	}

	for (int j = 0; j < popSize; j++)
	{
		for (int h = 0; h < 1024 * noOfCities; h++)
		{
			int x = randNum(1, noOfCities - 1);
			int y = randNum(1, noOfCities - 1);
			std::swap(calcPath[x], calcPath[y]);
		}
	}
	return calcPath;*/
}

std::vector<int> GA::itemsInit(int noOfItems, Knapsack &knapsack, std::vector<Item> &valuableItemsMatrix)
{
	knapsack.setCurrWeight(0);
	std::vector<int> stolenItemsList;
	stolenItemsList.resize(noOfItems);
	for (int i = 0; i < noOfItems; i++)
	{
		stolenItemsList[i] = 1;
	}
	while(calculateWeight(valuableItemsMatrix, stolenItemsList) > knapsack.getMaxWeight())
	{
		//std::swap(stolenItemsList[rand() % stolenItemsList.size()], stolenItemsList.back());
		//stolenItemsList.pop_back();
		int temp = rand() % stolenItemsList.size();
		stolenItemsList[temp] = 0;
		// cout for debug
		//std::cout << "\n Dropped item[" << temp << "]\n";
	}
	return stolenItemsList;
}

int GA::popInit(int noOfCities, int noOfItems, Knapsack &knapsack, std::vector<Item> &valuableItemsMatrix)
{
	parentsPop.clear();
	parentsPop.resize(0);
	parentsPop.resize(popSize);
	for (int i = 0; i < popSize; ++i)
		parentsPop[i].resize(noOfCities + 1);

	for (int i = 0; i < popSize; i++)
	{
		std::vector<int> initPath = pathInit(noOfCities);
		std::vector<int> initItems = itemsInit(noOfItems, knapsack, valuableItemsMatrix);
		parentsPop[i] = initPath;
		parentsPop[i].insert(parentsPop[i].end(), initItems.begin(), initItems.end());
	}

	//for observing population
	/*for (int k = 0; k < parentsPop.size(); k++)
	{
		std::cout << k << ": ";
		for (int l = 0; l < parentsPop[k].size(); l++)
		{
			std::cout << parentsPop[k][l] << "\t";
		}
		std::cout << std::endl;
	}*/
	return 0;
}


float GA::calculateWeight(std::vector<Item> &valuableItemsMatrix, std::vector<int> &stolenItemsList)
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
						  std::vector<int> &popMember, int noOfCities, int noOfItems, 
						  Knapsack &knapsack)
{
	knapsack.setCurrWeight(0);
	knapsack.setCurrSpeed();
	float tmpDist = 0;
	float tmpTime = 0;
	float calcProfit = 0;
	for(int i = 0; i < noOfCities; i++)
	{
		for(int j = 0; j < noOfItems; j++)
		{
			if(popMember[i] == valuableItemsMatrix[j].getAssignedCity() && popMember[noOfCities + 1 + j] == 1)
			{
				knapsack.increaseCurrWeight(valuableItemsMatrix[j].getWeight());
				knapsack.setCurrSpeed();
				calcProfit += valuableItemsMatrix[j].getProfit();
			}
		}
		int matrixX = popMember[i];
		int matrixY = popMember[i + 1];
		tmpDist += adjacancyMatrix[matrixX][matrixY % noOfCities];
		tmpTime += knapsack.getSpeed() / tmpDist;
		calcProfit -= tmpTime * knapsack.getRentingRatio();
	}
	return calcProfit;
}

// one point orden one crossover
int GA::OPOOX(std::vector<int> &childA, std::vector<int> &childB, std::vector<int> &parentA, std::vector<int> &parentB, 
			  int noOfCities, int noOfItems, Knapsack &knapsack, std::vector<Item> &valuableItemsMatrix)
{
	// 1-point crossover
	float diceroll = randNum(1, 10000);
	diceroll = diceroll / 10000.0;
	if (diceroll < crossRatio / 100.0)
	{
		//std::cout << "\nCROSSOVER!";
		// choose random place to cut parent
		int randCut = randNum(3, noOfCities - 2);
		std::vector<int> firstHalfA;
		firstHalfA.resize(randCut);
		for (int i = 0; i < randCut; i++)
			firstHalfA[i] = parentA[i];
		std::vector<int> firstHalfB;
		firstHalfB.resize(randCut);
		for (int i = 0; i < randCut; i++)
			firstHalfB[i] = parentB[i];
		// Copy elements from first parent up to cut point
		for (int i = 0; i < randCut; i++)
			childA[i] = firstHalfA[i];
		// Add remaining elements from second parent to child while preserving order
		int remaining = noOfCities - randCut;	
		int count = 0;
		for (int i = 0; i < noOfCities; i++)	
		{
			bool found = false;
			for (int j = 0; j <= randCut; j++) 
			{
				// If the city is in the child, exit this loop
				if (childA[j] == parentB[i])
				{
					found = true;
					break;
				}
			}
			// If the city was not found in the child, add it to the child
			if (!found)
			{
				childA[randCut + count] = parentB[i];
				count++;
			}
			// Stop once all of the cities have been added
			if (count == remaining)
				break;
		}
		// Copy elements from second parent up to cut point
		for (int i = 0; i < randCut; i++)
			childB[i] = firstHalfB[i];
		// Add remaining elements from first parent to child while preserving order
		int remainingB = noOfCities - randCut;
		int countB = 0;
		for (int i = 0; i < noOfCities; i++)
		{
			bool foundB = false;
			for (int j = 0; j <= randCut; j++)
			{
				// If the city is in the child, exit this loop
				if (childB[j] == parentA[i])
				{
					foundB = true;
					break;
				}
			}
			// If the city was not found in the child, add it to the child
			if (!foundB)
			{
				
				childB[randCut + countB] = parentA[i];
				countB++;
			}
			// Stop once all of the cities have been added
			if (countB == remainingB)
				break;
		}
	}
	else
	{
		//std::cout << "\nCOPY!";
		for (int i = 0; i < noOfCities + 1; i++)
			childA[i] = parentA[i];
		for (int i = 0; i < noOfCities + 1; i++)
			childB[i] = parentB[i];
	}
	return 0;
}

int GA::mutation(std::vector<int> &childA, std::vector<int> &childB, int noOfCities, int noOfItems, Knapsack &knapsack, 
				 std::vector<Item> &valuableItemsMatrix)
{
	//std::cout << "\nMUTATION!";
	double diceroll2 = randNum(1, 10000);
	diceroll2 = diceroll2 / 10000.0;
	if (diceroll2 < (mutRatio / 100.0))
	{
		int x = randNum(1, noOfCities - 1);
		int y = randNum(1, noOfCities - 1);
		std::swap(childA[x], childA[y]);
		int k = randNum(1, noOfCities - 1);
		int l = randNum(1, noOfCities - 1);
		std::swap(childB[k], childB[l]);
		
		int randA = randNum(noOfCities + 1, noOfCities + 1 + noOfItems);
		while(childA[randA] == 1)
			randA = randNum(noOfCities + 1, noOfCities + 1 + noOfItems);
		childA[randA] = 1;
		std::vector<int> tmpItems = std::vector(childA.begin() + noOfCities + 1, childA.end());
		while(calculateWeight(valuableItemsMatrix, tmpItems) > knapsack.getMaxWeight())
		{
			int temp = rand() % tmpItems.size();
			tmpItems[temp] = 0;
		}
		for(int i = 0; i < noOfItems; i++)
			childA[noOfCities + 1 + i] = tmpItems[i];
		
		int randB = randNum(noOfCities + 1, noOfCities + 1 + noOfItems);
		while(childB[randB] == 1)
			randB = randNum(noOfCities + 1, noOfCities + 1 + noOfItems);
		childB[randB] = 1;
		tmpItems = std::vector(childB.begin() + noOfCities + 1, childB.end());
		while(calculateWeight(valuableItemsMatrix, tmpItems) > knapsack.getMaxWeight())
		{
			int temp = rand() % tmpItems.size();
			tmpItems[temp] = 0;
		}
		for(int i = 0; i < noOfItems; i++)
			childB[noOfCities + 1 + i] = tmpItems[i];
	}
	return 0;
}

int GA::tournament(std::vector<std::vector<float>> &adjacancyMatrix, std::vector<Item> &valuableItemsMatrix,  
				   int noOfCities, int noOfItems, Knapsack &knapsack)
{
	for (int k = 0; k < popSize / 2; k++)
	{
		int bestParent = 0;
		int currParent;
		int secondParent = 0;

		parentA.clear();
		parentA.resize(noOfCities + 1 + noOfItems);
		parentB.clear();
		parentB.resize(noOfCities + 1 + noOfItems);
		contestantA.clear();
		contestantA.resize(noOfCities + 1 + noOfItems);
		contestantB.clear();
		contestantB.resize(noOfCities + 1 + noOfItems);

		int randA = randNum(0, popSize - 1);
		//for (int i = 0; i < noOfCities + 1; i++)
		contestantA = parentsPop[randA];

		int randB = randNum(0, popSize - 1);
		while (randA == randB)
			randB = randNum(0, popSize - 1);
		//for (int i = 0; i < noOfCities + 1; i++)
		contestantB = parentsPop[randB];

		float profA = calculateProfit(adjacancyMatrix, valuableItemsMatrix, contestantA, 
									  noOfCities, noOfItems, knapsack);
		float profB = calculateProfit(adjacancyMatrix, valuableItemsMatrix, contestantB, 
									  noOfCities, noOfItems, knapsack);

		if (profA > profB)
		{
			for (int j = 0; j < noOfCities + 1 + noOfItems; j++)
				parentA[j] = contestantA[j];
		}
		else
		{
			for (int j = 0; j < noOfCities + 1 + noOfItems; j++)
				parentA[j] = contestantB[j];
		}

		randA = randNum(0, popSize - 1);
		//for (int i = 0; i < noOfCities + 1; i++)
		contestantA = parentsPop[randA];

		randB = randNum(0, popSize - 1);
		while (randA == randB)
			randB = randNum(0, popSize - 1);
		//for (int i = 0; i < noOfCities + 1; i++)
		contestantA = parentsPop[randB];

		profA = calculateProfit(adjacancyMatrix, valuableItemsMatrix, contestantA, 
								noOfCities, noOfItems, knapsack);
		profB = calculateProfit(adjacancyMatrix, valuableItemsMatrix, contestantB, 
								noOfCities, noOfItems, knapsack);

		if (profA > profB)
		{
			for (int j = 0; j < noOfCities + 1 + noOfItems; j++)
				parentB[j] = contestantA[j];
		}
		else
		{
			for (int j = 0; j < noOfCities + 1 + noOfItems; j++)
				parentB[j] = contestantB[j];
		}
		// breed with chance to cross and to mutate
		childA.clear();
		childA.resize(noOfCities + 1 + noOfItems);
		childB.clear();
		childB.resize(noOfCities + 1 + noOfItems);
		//crossover and mutation functions
		OPOOX(childA, childB, parentA, parentB, noOfCities, noOfItems, knapsack, valuableItemsMatrix);
		mutation(childA, childB, noOfCities, noOfItems, knapsack, valuableItemsMatrix);
		// Q <- Qa, Qb
		childrenPop.push_back(childA);
		childrenPop.push_back(childB);
	}
	return 0;
}

float GA::solverGA(std::vector<std::vector<float>> &adjacancyMatrix, std::vector<Item> &valuableItemsMatrix,  
				   int noOfCities, int noOfItems, Knapsack &knapsack)
{
	// 1. Initialize population[popSize] where member is set of items and a path
	// 2. Assign them a fitness (only profit matters, it describes everything). 
	// 3. Save the best member as current best solution if it's better than current best
	// 4. Make the fight (roulette[size] or tournament[size]). The better the fitness the bigger the chance of winning
	// 5. Two winners mate 
	// 6. Chance for crossover to happen on a child (usually >60%) (1 point crossover, 2 point, 3 point...)
	// 7. Chance for mutation to happen on a child (usually <1%) (swap 2 random cities, drop 1 item, take 1 item [drop more if we went over max weight])
	// 8. Repeat until new population[popSize] is filled with children
	// 9. Repeat 2-8 until we reach max number of generations (or until bored).
	
	// best <- float min
	float bestProfit = std::numeric_limits<float>::min();
	int bestWeight = 0;
	std::vector<int> bestFound;
	bestFound.resize(noOfCities + 1 + noOfItems);
	int iterations = 0;

	do
	{
		//for observing population
		/*for (int k = 0; k < parentsPop.size(); k++)
		{
			std::cout << k << ": ";
			for (int l = 0; l < parentsPop[k].size(); l++)
			{
				std::cout << parentsPop[k][l] << "\t";
			}
			std::cout << std::endl;
		}*/

		// Assess fitness of every P(i)
		for (int i = 0; i < popSize; i++)
		{
			float currProfit = calculateProfit(adjacancyMatrix, valuableItemsMatrix, parentsPop[i], 
											 noOfCities, noOfItems, knapsack);
			if (currProfit > bestProfit)
			{
				bestProfit = currProfit;
				//for (int j = 0; j < noOfCities + 1; j++)
				bestFound = parentsPop[i];
				bestWeight = knapsack.getCurrWeight();
			}
		}
		childrenPop.clear();
		childrenPop.resize(0);

		tournament(adjacancyMatrix, valuableItemsMatrix, noOfCities, noOfItems, knapsack);

		// P <- Q
		parentsPop.clear();
		parentsPop.resize(popSize);
		for (int v = 0; v < popSize; ++v)
			parentsPop[v].resize(noOfCities + 1);

		for (int i = 0; i < popSize; i++)
			parentsPop[i] = childrenPop[i];
		iterations++;
		//sqrt(noOfCities) * 4000
	} while (iterations < noOfGenerations);

	std::cout << std::endl << "Profit:\t" << bestProfit << std::endl;
	std::cout << "Weight:\t" << bestWeight << " / " << knapsack.getMaxWeight() << std::endl;
	std::cout << "Path:\t";
	for (int i = 0; i < noOfCities + 1; i++)
	{
		std::cout << bestFound[i] << "\t";
	}
	std::cout << std::endl << "Items:\t";
	for (int i = 0; i < noOfItems; i++)
	{
		if(bestFound[noOfCities + 1 + i] == 1)
			std::cout << i << "\t";
	}
	bestFound.clear();
	bestFound.resize(0);
	childrenPop.clear();
	childrenPop.resize(0);
	parentsPop.clear();
	parentsPop.resize(0);
	return bestProfit;
}

int GA::randNum(int l, int r)
{
	return rand() % (r - l + 1) + l;
}

double GA::randFraction(void)
{
	return randNum(1, 10000) / 10000;
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

std::vector<int> GA::getStolenItemsList()
{	
	return std::vector<int>(stolenItemsList);
}

std::vector<std::vector<int>> GA::getParentsPop(void)
{
	return std::vector<std::vector<int>>(parentsPop);
}

std::vector<std::vector<int>> GA::getChildrenPop(void)
{
	return std::vector<std::vector<int>>(childrenPop);
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
