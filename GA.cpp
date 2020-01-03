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

int GA::paramsInit()
{
	while(1)
	{
		int option;
		std::cout << "Current parameters are:" << "\nPopulation size:\t" << popSize << "\nCrossover ratio:\t" << crossRatio << "%\nMutation ratio:\t\t" << mutRatio << "%\nTime limit [s]:\t\t" << timeLimitSec;
		std::cout << "\nDo you wish to change parametres?\n1 - Yes\n2 - No, proceed\n";
		std::cin >> option;
		switch (option)
		{
			case 1:
			{
				std::cout << "Input new population size\n";
				float newPopSize;
				std::cin >> newPopSize;
				setPopSize(newPopSize);

				std::cout << "Input new crossover ratio [%]\n";
				float newCrossRatio;
				std::cin >> newCrossRatio;
				setCrossoverRatio(newCrossRatio);

				std::cout << "Input new mutation ratio [%]\n";
				float newMutRatio;
				std::cin >> newMutRatio;
				setMutationRatio(newMutRatio);

				std::cout << "Input new time limit [s]\n";
				float newtimeLimitSec;
				std::cin >> newtimeLimitSec;
				setTimeLimitSec(newtimeLimitSec);
				break;
			}
			case 2:
			{
				return 0;
			}
			default:
			{
				std::cout << "Wrong input";
				break;
			}
		}
	}
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
		int randCutPath = randNum(3, noOfCities - 2);
		int randCutItems = randNum(3, noOfItems - 2);
		std::vector<int> firstHalfPathA;
		std::vector<int> firstHalfItemsA;
		firstHalfPathA.resize(randCutPath);
		firstHalfItemsA.resize(randCutItems);
		for (int i = 0; i < randCutPath; i++)
			firstHalfPathA[i] = parentA[i];
		for (int i = 0; i < randCutItems; i++)
			firstHalfItemsA[i] = parentA[i + noOfCities + 1];
		std::vector<int> firstHalfPathB;
		std::vector<int> firstHalfItemsB;
		firstHalfPathB.resize(randCutPath);
		firstHalfItemsB.resize(randCutItems);
		for (int i = 0; i < randCutPath; i++)
			firstHalfPathB[i] = parentB[i];
		for (int i = 0; i < randCutItems; i++)
			firstHalfItemsB[i] = parentB[i + noOfCities + 1];
		// Copy elements from first parent up to cut point
		for (int i = 0; i < randCutPath; i++)
			childA[i] = firstHalfPathA[i];
		for (int i = 0; i < randCutItems; i++)
			childA[i + noOfCities + 1] = firstHalfItemsA[i];
		for (int i = 0; i < noOfItems - randCutItems; i++)
			childA[i + noOfCities + 1 + randCutItems] = parentB[i];
		// drop items if child went over max weight
		std::vector<int> tmpItems = std::vector(childA.begin() + noOfCities + 1, childA.end());
		while(calculateWeight(valuableItemsMatrix, tmpItems) > knapsack.getMaxWeight())
		{
			int temp = rand() % tmpItems.size();
			tmpItems[temp] = 0;
		}
		for(int i = 0; i < noOfItems; i++)
			childA[noOfCities + 1 + i] = tmpItems[i];
		// Add what's left of elements from second parent to child while preserving order (not necessary for items)
		int remainingA = noOfCities - randCutPath;	
		int count = 0;
		for (int i = 0; i < noOfCities; i++)	
		{
			bool found = false;
			for (int j = 0; j <= randCutPath; j++) 
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
				childA[randCutPath + count] = parentB[i];
				count++;
			}
			// Stop once all of the cities have been added
			if (count == remainingA)
				break;
		}
		// Copy elements from second parent up to cut point
		for (int i = 0; i < randCutPath; i++)
			childB[i] = firstHalfPathB[i];
		for (int i = 0; i < randCutItems; i++)
			childB[i + noOfCities + 1] = firstHalfItemsB[i];
		for (int i = 0; i < noOfItems - randCutItems; i++)
			childB[i + noOfCities + 1 + randCutItems] = parentA[i];
		// drop items if child went over max weight
		tmpItems = std::vector(childB.begin() + noOfCities + 1, childB.end());
		while(calculateWeight(valuableItemsMatrix, tmpItems) > knapsack.getMaxWeight())
		{
			int temp = rand() % tmpItems.size();
			tmpItems[temp] = 0;
		}
		for(int i = 0; i < noOfItems; i++)
			childB[noOfCities + 1 + i] = tmpItems[i];
		// Add what's left of elements from first parent to child while preserving order (not necessary for items)
		int remainingB = noOfCities - randCutPath;
		int countB = 0;
		for (int i = 0; i < noOfCities; i++)
		{
			bool foundB = false;
			for (int j = 0; j <= randCutPath; j++)
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
				
				childB[randCutPath + countB] = parentA[i];
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

// two point orden one crossover
int GA::TPOOX(std::vector<int> &childA, std::vector<int> &childB, std::vector<int> &parentA, std::vector<int> &parentB, 
			  int noOfCities, int noOfItems, Knapsack &knapsack, std::vector<Item> &valuableItemsMatrix)
{
	// 2-point crossover
	float diceroll = randNum(1, 10000);
	diceroll = diceroll / 10000.0;
	if (diceroll < crossRatio / 100.0)
	{
		//std::cout << "\nCROSSOVER!";
		// choose random 2 places to cut parent
		int randCutPath1 = randNum(3, noOfCities - 2);
		int randCutPath2 = randNum(3, noOfCities - 2);
		while(randCutPath1 == randCutPath2)
			randCutPath2 = randNum(3, noOfCities - 2);
		int sizeOfPathCut = abs(randCutPath2 - randCutPath1) + 1;
		int lowerPathCut = std::min(randCutPath1, randCutPath2);
		int higherPathCut = std::max(randCutPath1, randCutPath2);
		//std::cout << "\nsize path " << sizeOfPathCut << "\nlower path " << lowerPathCut << "\nhigher path " << higherPathCut;
		int randCutItems1 = randNum(3, noOfItems - 2);
		int randCutItems2 = randNum(3, noOfItems - 2);
		while(randCutItems1 == randCutItems2)
			randCutItems2 = randNum(3, noOfItems - 2);
		int sizeOfItemsCut = abs(randCutItems2 - randCutItems1) + 1;
		int lowerItemsCut = std::min(randCutItems1, randCutItems2);
		int higherItemsCut = std::max(randCutItems1, randCutItems2);
		//std::cout << "\nsize items " << sizeOfItemsCut<< "\nlower items " << lowerItemsCut << "\nhigher items " << higherItemsCut;
		std::vector<int> firstHalfPathA;
		std::vector<int> firstHalfItemsA;
		firstHalfPathA.resize(sizeOfPathCut);
		firstHalfItemsA.resize(sizeOfItemsCut);
		for (int i = 0; i < sizeOfPathCut; i++)
			firstHalfPathA[i] = parentA[i + lowerPathCut];
		for (int i = 0; i < sizeOfItemsCut; i++)
			firstHalfItemsA[i] = parentA[i + noOfCities + 1 + lowerItemsCut];
		std::vector<int> firstHalfPathB;
		std::vector<int> firstHalfItemsB;
		firstHalfPathB.resize(sizeOfPathCut);
		firstHalfItemsB.resize(sizeOfItemsCut);
		for (int i = 0; i < sizeOfPathCut; i++)
			firstHalfPathB[i] = parentB[i + lowerPathCut];
		for (int i = 0; i < sizeOfItemsCut; i++)
			firstHalfItemsB[i] = parentB[i + noOfCities + 1 + noOfCities + 1 + lowerItemsCut];
		// Copy elements from first parent up to cut point
		for (int i = 0; i < sizeOfPathCut; i++)
			childA[i] = firstHalfPathA[i];
		for (int i = 0; i < sizeOfItemsCut; i++)
			childA[i + noOfCities + 1] = firstHalfItemsA[i];
		for (int i = 0; i < noOfItems - sizeOfItemsCut; i++)
			childA[i + noOfCities + 1 + sizeOfItemsCut] = parentB[i];
		// drop items if child went over max weight
		std::vector<int> tmpItems = std::vector(childA.begin() + noOfCities + 1, childA.end());
		while(calculateWeight(valuableItemsMatrix, tmpItems) > knapsack.getMaxWeight())
		{
			int temp = rand() % tmpItems.size();
			tmpItems[temp] = 0;
		}
		for(int i = 0; i < noOfItems; i++)
			childA[noOfCities + 1 + i] = tmpItems[i];
		// Add what's left of elements from second parent to child while preserving order (not necessary for items)
		int remainingA = noOfCities - sizeOfPathCut;	
		int count = 0;
		for (int i = 0; i < noOfCities; i++)	
		{
			bool found = false;
			for (int j = 0; j <= sizeOfPathCut; j++) 
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
				childA[count + sizeOfPathCut] = parentB[i];
				count++;
			}
			// Stop once all of the cities have been added
			if (count == remainingA)
				break;
		}

		// Copy elements from second parent up to cut point
		for (int i = 0; i < sizeOfPathCut; i++)
			childB[i] = firstHalfPathB[i];
		for (int i = 0; i < sizeOfItemsCut; i++)
			childB[i + noOfCities + 1] = firstHalfItemsB[i];
		for (int i = 0; i < noOfItems - sizeOfItemsCut; i++)
			childB[i + noOfCities + 1 + sizeOfItemsCut] = parentA[i];
		// drop items if child went over max weight
		tmpItems = std::vector(childB.begin() + noOfCities + 1, childB.end());
		while(calculateWeight(valuableItemsMatrix, tmpItems) > knapsack.getMaxWeight())
		{
			int temp = rand() % tmpItems.size();
			tmpItems[temp] = 0;
		}
		for(int i = 0; i < noOfItems; i++)
			childB[noOfCities + 1 + i] = tmpItems[i];
		// Add what's left of elements from second parent to child while preserving order (not necessary for items)
		int remainingB = noOfCities - sizeOfPathCut;	
		count = 0;
		for (int i = 0; i < noOfCities; i++)	
		{
			bool found = false;
			for (int j = 0; j <= sizeOfPathCut; j++) 
			{
				// If the city is in the child, exit this loop
				if (childB[j] == parentA[i])
				{
					found = true;
					break;
				}
			}
			// If the city was not found in the child, add it to the child
			if (!found)
			{
				childB[count + sizeOfPathCut] = parentA[i];
				count++;
			}
			// Stop once all of the cities have been added
			if (count == remainingB)
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
		TPOOX(childA, childB, parentA, parentB, noOfCities, noOfItems, knapsack, valuableItemsMatrix);
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
	//int iterations = 0;
	
	Stopwatch *timer = new Stopwatch();
	timer->point1 = std::chrono::high_resolution_clock::now();
	
	while (timer->countTimeDiff() < timeLimitSec * 1E9)
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
		//iterations++;
		noOfGenerations++;
	} 

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
	std::cout << std::endl << "Number of generations:\t" << noOfGenerations;
	bestFound.clear();
	bestFound.resize(0);
	childrenPop.clear();
	childrenPop.resize(0);
	parentsPop.clear();
	parentsPop.resize(0);
	noOfGenerations = 0;
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

void GA::setTimeLimitSec(int newTimeLimitSec)
{
	timeLimitSec = newTimeLimitSec;
}

GA::GA()
{

}

GA::~GA()
{

}
