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
			int x = randInt(1, noOfCities - 1);
			int y = randInt(1, noOfCities - 1);
			std::swap(calcPath[x], calcPath[y]);
		}
	}
	return calcPath;*/
}

std::vector<int> GA::itemsInit(int noOfItems, Knapsack& knapsack, std::vector<Item> &valuableItemsMatrix)
{
	knapsack.setCurrWeight(0);
	std::vector<int> stolenItemsList;
	stolenItemsList.resize(noOfItems);
	for (int i = 0; i < noOfItems; i++)
	{
		stolenItemsList[i] = 1;
	}
	while(calculateWeight(valuableItemsMatrix, stolenItemsList, stolenItemsList.size()) > knapsack.getMaxWeight())
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

int GA::popInit(int noOfCities)
{
	parentsPop.clear();
	parentsPop.resize(0);
	parentsPop.resize(popSize);
	for (int i = 0; i < popSize; ++i)
		parentsPop[i].resize(noOfCities + 1);

	popMember = pathInit(noOfCities);

	for (int j = 0; j < popSize; j++)
	{
		for (int h = 0; h < 1000 * noOfCities; h++)
		{
			int x = randInt(1, noOfCities - 1);
			int y = randInt(1, noOfCities - 1);
			std::swap(popMember[x], popMember[y]);
		}
		parentsPop[j] = popMember;
	}
	return 0;
}


float GA::calculateWeight(std::vector<Item> &valuableItemsMatrix, std::vector<int> &stolenItemsList, int noOfItems)
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

float GA::solverGA(std::vector<std::vector<float>> &adjacancyMatrix, std::vector<Item> &valuableItemsMatrix,  
				   int noOfCities, int noOfItems, Knapsack& knapsack)
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
	
	// best <- null
	float bestProfit = std::numeric_limits<float>::max();
	std::vector<int> bestPath;
	bestPath.clear();
	bestPath.resize(noOfCities + 1);
	int iterations = 0;

	do
	{
		//for observing population
		/*for (int k = 0; k < parentsPop.size(); k++)
		{
			for (int l = 0; l < parentsPop[k].size(); l++)
			{
				std::cout << parentsPop[k][l] << "\t";
			}
			std::cout << std::endl;
		}*/

		// Assess fitness of every P(i)
		for (int i = 0; i < popSize; i++)
		{
			int currProfit = calculateDist(adjacancyMatrix, parentsPop[i], noOfCities);
			if (currProfit < bestProfit)
			{
				bestProfit = currProfit;
				//for (int j = 0; j < noOfCities + 1; j++)
				bestPath = parentsPop[i];
			}
		}
		childrenPop.clear();
		childrenPop.resize(0);

		for (int k = 0; k < popSize / 2; k++)
		{
			// tournament
			int bestParent = 0;
			int currParent;
			int secondParent = 0;
			std::vector<int> parentA;
			parentA.resize(noOfCities + 1);
			std::vector<int> parentB;
			parentB.resize(noOfCities + 1);
			std::vector<int> contestantA;
			contestantA.resize(noOfCities + 1);
			std::vector<int> contestantB;
			contestantB.resize(noOfCities + 1);

			int randA = randInt(0, popSize - 1);
			//for (int i = 0; i < noOfCities + 1; i++)
			contestantA = parentsPop[randA];

			int randB = randInt(0, popSize - 1);
			while (randA == randB)
				randB = randInt(0, popSize - 1);
			//for (int i = 0; i < noOfCities + 1; i++)
			contestantB = parentsPop[randB];

			int distA = 0;
			int distB = 0;
			for (int i = 0; i < noOfCities; i++)
			{
				int a = contestantA[i];
				int b = contestantA[i + 1];
				distA += adjacancyMatrix[a][b % (noOfCities)];
			}
			for (int i = 0; i < noOfCities; i++)
			{
				int a = contestantB[i];
				int b = contestantB[i + 1];
				distB += adjacancyMatrix[a][b % (noOfCities)];
			}
			if (distA < distB)
			{
				for (int j = 0; j < noOfCities + 1; j++)
					parentA[j] = contestantA[j];
			}
			else
			{
				for (int j = 0; j < noOfCities + 1; j++)
					parentA[j] = contestantB[j];
			}

			randA = randInt(0, popSize - 1);
			//for (int i = 0; i < noOfCities + 1; i++)
			contestantA = parentsPop[randA];

			randB = randInt(0, popSize - 1);
			while (randA == randB)
				randB = randInt(0, popSize - 1);
			//for (int i = 0; i < noOfCities + 1; i++)
			contestantA = parentsPop[randB];

			distA = 0;
			distB = 0;
			for (int i = 0; i < noOfCities; i++)
			{
				int a = contestantA[i];
				int b = contestantA[i + 1];
				distA += adjacancyMatrix[a][b % (noOfCities)];
			}
			for (int i = 0; i < noOfCities; i++)
			{
				int a = contestantB[i];
				int b = contestantB[i + 1];
				distB += adjacancyMatrix[a][b % (noOfCities)];
			}
			if (distA < distB)
			{
				for (int j = 0; j < noOfCities + 1; j++)
					parentB[j] = contestantA[j];
			}
			else
			{
				for (int j = 0; j < noOfCities + 1; j++)
					parentB[j] = contestantB[j];
			}

			// choose random place to cut parent
			int randCut = randInt(3, noOfCities - 2);

			std::vector<int> firstHalfA;
			firstHalfA.resize(randCut);
			for (int i = 0; i < randCut; i++)
				firstHalfA[i] = parentA[i];

			std::vector<int> firstHalfB;
			firstHalfB.resize(randCut);
			for (int i = 0; i < randCut; i++)
				firstHalfB[i] = parentB[i];

			// breed with chance to cross and to mutate
			std::vector<int> childA;
			std::vector<int> childB;
			childA.resize(noOfCities + 1);
			childB.resize(noOfCities + 1);

			float diceroll = randInt(1, 10000);
			diceroll = diceroll / 10000;
			
			// crossover
			if (diceroll < crossRatio / 100)
			{
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
				for (int i = 0; i < noOfCities + 1; i++)
					childA[i] = parentA[i];
				for (int i = 0; i < noOfCities + 1; i++)
					childB[i] = parentB[i];
			}

			// mutation
			double diceroll2 = randInt(1, 10000);
			diceroll2 = diceroll2 / 10000;
			if (diceroll2 < (mutRatio / 100))
			{
				int x = randInt(1, noOfCities - 1);
				int y = randInt(1, noOfCities - 1);
				std::swap(childA[x], childA[y]);

				int z = randInt(1, noOfCities - 1);
				int t = randInt(1, noOfCities - 1);
				std::swap(childB[z], childB[t]);
			}

			// Q <- Qa, Qb
			childrenPop.push_back(childA);
			childrenPop.push_back(childB);

			parentA.clear();
			parentA.resize(0);
			parentB.clear();
			parentB.resize(0);
			firstHalfA.clear();
			firstHalfA.resize(0);
			firstHalfB.clear();
			firstHalfB.resize(0);
			childA.clear();
			childA.resize(0);
			childB.clear();
			childB.resize(0);
		}

		// P <- Q
		parentsPop.clear();
		parentsPop.resize(0);
		parentsPop.resize(popSize);
		for (int v = 0; v < popSize; ++v)
			parentsPop[v].resize(noOfCities + 1);

		for (int i = 0; i < popSize; i++)
			parentsPop[i] = childrenPop[i];
		iterations++;
		//sqrt(noOfCities) * 4000
	} while (iterations < noOfGenerations);

	std::cout << std::endl << "Profit:\t" << bestProfit << std::endl;
	//std::cout << "Weight:\t" << bestWeight << " / " << knapsack.getMaxWeight() << std::endl;
	std::cout << "Path:\t";
	for (int i = 0; i < noOfCities + 1; i++)
	{
		std::cout << bestPath[i] << "\t";
	}
	bestPath.clear();
	bestPath.resize(0);
	childrenPop.clear();
	childrenPop.resize(0);
	parentsPop.clear();
	parentsPop.resize(0);
	return bestProfit;
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

std::vector<int> GA::getPopMember(void)
{
	return std::vector<int>(popMember);
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

