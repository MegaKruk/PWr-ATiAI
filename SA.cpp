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

std::vector<int> SA::itemsInit(int noOfItems, Knapsack& knapsack, std::vector<Item> &valuableItemsMatrix)
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

int SA::paramsInit()
{
	while(1)
	{
		int option;
		std::cout << "Current parameters are:" << "\nTmax:\t\t" << Tmax << "\nTmin:\t\t" << Tmin << "\nTcoefficient:\t" << Tcoeff << "\nTime limit [s]:\t" << timeLimitSec;
		std::cout << "\nDo you wish to change parametres?\n1 - Yes\n2 - No, proceed\n";
		std::cin >> option;
		switch (option)
		{
			case 1:
			{
				std::cout << "Input new Tmax\n";
				double newTmax;
				std::cin >> newTmax;
				setTmax(newTmax);

				std::cout << "Input new Tmin\n";
				double newTmin;
				std::cin >> newTmin;
				setTmin(newTmin);

				std::cout << "Input new Tcoefficient\n";
				double newTcoeff;
				std::cin >> newTcoeff;
				setTcoeff(newTcoeff);

				std::cout << "Input new time limit [s]\n";
				double newtimeLimitSec;
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

float SA::calculateDist(std::vector<std::vector<float>> &adjacancyMatrix, std::vector<int> &calcPath, int noOfCities)
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

float SA::calculateProfit(std::vector<std::vector<float>> &adjacancyMatrix, std::vector<Item> &valuableItemsMatrix, 
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

float SA::solverSA(std::vector<std::vector<float>> &adjacancyMatrix, std::vector<Item> &valuableItemsMatrix, std::vector<int> &calcPath, 
				   std::vector<int> &stolenItemsList, int noOfCities, int noOfItems, Knapsack& knapsack)
{
	// 1. travel from city[0] to city[1] | T = v/s | P -= R * T
	// 2. if there is item from steal-list pick it up
	// 3. set new currWeight and currSpeed
	// 4. repeat 1-3 until all cities are visited
	// 5. come back to city[0] - REMEMBER TO CHANGE PROFIT!
	// 6. return final profit
	/*std::cout << std::endl;
	for(int i = 0; i < calcPath.size(); i++)
		std::cout << calcPath[i];
	std::cout << std::endl;
	for(int i = 0; i < stolenItemsList.size(); i++)
		std::cout << stolenItemsList[i];
	std::cout << std::endl;*/

	float bestProfit = std::numeric_limits<float>::min();
	float bestWeight = -1;
	std::vector<int> bestPath;
	std::vector<int> bestItems;

	// s - initial candidate
	float currProfit = calculateProfit(adjacancyMatrix, valuableItemsMatrix, calcPath, stolenItemsList, 
									   noOfCities, noOfItems, knapsack);
	//std::cout << std::endl << currProfit;
	// best = s
	bestProfit = currProfit;

	bestPath.clear();
	bestPath.resize(calcPath.size());
	
	bestItems.clear();
	bestItems.resize(stolenItemsList.size());

	for (int i = 0; i < noOfCities + 1; i++)
	{
		bestPath[i] = calcPath[i];
	}

	for (int i = 0; i < stolenItemsList.size(); i++)
	{
		bestItems[i] = stolenItemsList[i];
		//std::cout << std::endl << bestItems[i];
	}
	
	// check if we went over limit
	while(calculateWeight(valuableItemsMatrix, bestItems, bestItems.size()) > knapsack.getMaxWeight())
	{
		// couts for debug
		//std::cout << std::endl << "curr weight is " << calculateWeight(valuableItemsMatrix, bestItems, bestItems.size()) << "\tmax is " << knapsack.getMaxWeight();
		std::swap(bestItems[rand() % bestItems.size()], bestItems.back());
		//std::cout << std::endl << "popping " << bestItems.back();
		bestItems.pop_back();
	}
	//std::cout << std::endl << "curr weight is " << calculateWeight(valuableItemsMatrix, bestItems, bestItems.size()) << "\tmax is " << knapsack.getMaxWeight();

	// old main loop
	//for (double T = Tmax; T >= Tmin; T *= Tcoeff)	
	Stopwatch *timer = new Stopwatch();
	timer->point1 = std::chrono::high_resolution_clock::now();
	// new main loop
	double T = Tmax;
	while(timer->countTimeDiff() < timeLimitSec * 1E9)
	{
		//std::cout << timer->countTimeDiff() << " ns elapsed\n";

		// r - tweak attempt for path
		int i = randNum(1, noOfCities - 1);
		int j = randNum(1, noOfCities - 1);
		std::swap(calcPath[i], calcPath[j]);

		// tweaking attempt for steal-list
		std::vector<int> tmpItems;
		tmpItems.resize(noOfItems);
		for(int i = 0; i < noOfItems; i++)
			tmpItems[i] = i;
		while(calculateWeight(valuableItemsMatrix, tmpItems, tmpItems.size()) > knapsack.getMaxWeight())
		{
			std::swap(tmpItems[rand() % tmpItems.size()], tmpItems.back());
			tmpItems.pop_back();
		}

		float newProfit = calculateProfit(adjacancyMatrix, valuableItemsMatrix, calcPath, tmpItems, 
										  noOfCities, noOfItems, knapsack);

		if (newProfit > currProfit || randFraction() < exp((currProfit - newProfit) / T))
		{
			// s = r
			currProfit = newProfit;

			// best = s
			if (currProfit > bestProfit)
			{
				bestProfit = currProfit;
				bestWeight = knapsack.getCurrWeight();
				bestItems.clear();
				bestItems.resize(tmpItems.size());
				for (int i = 0; i < calcPath.size(); i++)
				{
					bestPath[i] = calcPath[i];
				}
				for(int i = 0; i < tmpItems.size(); i++)
				{
					bestItems[i] = tmpItems[i];
				}
			}
		}
		else
		{
			std::swap(calcPath[i], calcPath[j]);
		}
		if(T > Tmin)
			T *= Tcoeff;
	}
	
	std::cout << "Path:\t";
	for (int i = 0; i < bestPath.size(); i++)
	{
		std::cout << bestPath[i] << "\t";
	}

	std::sort (bestItems.begin(), bestItems.end());
	std::cout << std::endl << "Items:\t";
	for (int i = 0; i < bestItems.size(); i++)
	{
		std::cout << bestItems[i] << "\t";
	}
	bestPath.clear();
	bestItems.clear();
	std::cout << "\nWeight:\t" << bestWeight << " / " << knapsack.getMaxWeight();
	std::cout << "\nProfit:\t" << bestProfit << std::endl;	
	return bestProfit;
}

int SA::randNum(int l, int r)
{
	return rand() % (r - l + 1) + l;
}

double SA::randFraction(void)
{
	return randNum(1, 10000) / 10000;
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

double SA::getTmax()
{
	return Tmax;
}

double SA::getTmin()
{
	return Tmin;
}

double SA::getTcoeff()
{
	return Tcoeff;
}

int SA::getTimeLimitSec()
{
	return timeLimitSec;
}

void SA::setTmax(double newTmax)
{
	Tmax = newTmax;
}

void SA::setTmin(double newTmin)
{
	Tmin = newTmin;
}

void SA::setTcoeff(double newTcoeff)
{
	Tcoeff = newTcoeff;
}

void SA::setTimeLimitSec(int newTimeLimitSec)
{
	timeLimitSec = newTimeLimitSec;
}

SA::SA()
{

}

SA::~SA()
{

}

