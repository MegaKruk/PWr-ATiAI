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
	std::vector<bool> stolenItemsList;
	std::tuple<std::vector<int>, std::vector<bool>> popMember;
	std::vector<std::tuple<std::vector<int>, std::vector<bool>>> childrenPop;
	std::vector<std::tuple<std::vector<int>, std::vector<bool>>> parentsPop;
	
	int popSize;
	int noOfGenerations;
	float crossRatio;
	float mutRatio;

public:
	GA();
	~GA();

	std::vector<int> pathInit(int noOfCities);
	std::vector<bool> itemsInit(int noOfItems, Knapsack& knapsack, std::vector<Item> &valuableItemsMatrix);

	float calculateWeight(std::vector<Item> &valuableItemsMatrix, std::vector<bool> &stolenItemsList, int noOfItems);
	float calculateDist(std::vector<std::vector<float>> &adjacancyMatrix, std::vector<int> &calcPath, int noOfCities);
	float calculateProfit(std::vector<std::vector<float>> &adjacancyMatrix, std::vector<Item> &valuableItemsMatrix, 
						  std::vector<int> &calcPath, std::vector<bool> &stolenItemsList, int noOfCities, int noOfItems, 
						  Knapsack& knapsack);  

	int solverGA(std::vector<std::vector<float>> &adjacancyMatrix, std::vector<Item> &valuableItemsMatrix,  
				 std::vector<int> &calcPath, std::vector<bool> &stolenItemsList, int noOfCities, int noOfItems, 
				 Knapsack& knapsack); 

	int randInt(int l, int r);
	double randFraction();

	std::vector<Item> getItemsFromCurrCity(int currCity, std::vector<Item> items);
	std::vector<int> getCalcPath();
	std::vector<bool> getStolenItemsList();
	std::vector<std::tuple<std::vector<int>, std::vector<bool>>> getParentsPop();
	std::vector<std::tuple<std::vector<int>, std::vector<bool>>> getChildrenPop();
	std::tuple<std::vector<int>, std::vector<bool>> getPopMember();
	int getPopSize();
	int getNoOfGenerations();
	float getCrossRatio();
	float getMutRatio();

	void setMutationRatio(float val);
	void setCrossoverRatio(float val);
	void setPopSize(int val);
	void setNoOfGenerations(int val);
};

