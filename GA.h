#include <stdlib.h>
#include <string>
#include <limits.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <limits>
#include "TTP.h"
#include "Knapsack.h"
#include "Item.h"

class GA
{
private:
	std::vector<int> calcPath;
	std::vector<int> stolenItemsList;
	std::vector<std::vector<int>> childrenPop;
	std::vector<std::vector<int>> parentsPop;
	std::vector<int> contestantA;
	std::vector<int> contestantB;
	std::vector<int> parentA;
	std::vector<int> parentB;
	std::vector<int> childA;
	std::vector<int> childB;

	int popSize = 100;
	int noOfGenerations = 1000;
	float crossRatio = 60;
	float mutRatio = 3;

public:
	GA();
	~GA();

	std::vector<int> pathInit(int noOfCities);
	std::vector<int> itemsInit(int noOfItems, Knapsack &knapsack, std::vector<Item> &valuableItemsMatrix);
	int popInit(int noOfCities, int noOfItems, Knapsack &knapsack, std::vector<Item> &valuableItemsMatrix);

	float calculateWeight(std::vector<Item> &valuableItemsMatrix, std::vector<int> &stolenItemsList);
	float calculateDist(std::vector<std::vector<float>> &adjacancyMatrix, std::vector<int> &calcPath, int noOfCities);
	float calculateProfit(std::vector<std::vector<float>> &adjacancyMatrix, std::vector<Item> &valuableItemsMatrix, 
						  std::vector<int> &popMember, int noOfCities, int noOfItems, 
						  Knapsack &knapsack); 

	int OPOOX(std::vector<int> &childA, std::vector<int> &childB, std::vector<int> &parentA, std::vector<int> &parentB, 
			  int noOfCities, int noOfItems, Knapsack &knapsack, std::vector<Item> &valuableItemsMatrix);
	int mutation(std::vector<int> &childA, std::vector<int> &childB, int noOfCities, int noOfItems, Knapsack &knapsack, 
				 std::vector<Item> &valuableItemsMatrix);
	int tournament(std::vector<std::vector<float>> &adjacancyMatrix, std::vector<Item> &valuableItemsMatrix,  
				   int noOfCities, int noOfItems, Knapsack &knapsack);

	float solverGA(std::vector<std::vector<float>> &adjacancyMatrix, std::vector<Item> &valuableItemsMatrix,  
				   int noOfCities, int noOfItems, Knapsack &knapsack); 

	int randNum(int l, int r);
	double randFraction();

	std::vector<Item> getItemsFromCurrCity(int currCity, std::vector<Item> items);
	std::vector<int> getCalcPath();
	std::vector<int> getStolenItemsList();
	std::vector<std::vector<int>> getParentsPop();
	std::vector<std::vector<int>> getChildrenPop();
	int getPopSize();
	int getNoOfGenerations();
	float getCrossRatio();
	float getMutRatio();

	void setMutationRatio(float val);
	void setCrossoverRatio(float val);
	void setPopSize(int val);
	void setNoOfGenerations(int val);
};
