#include <stdlib.h>
#include <string>
#include <limits.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <vector>
#include "TTP.h"
#include "Knapsack.h"
#include "Item.h"

class SA
{
private:
	std::vector<int> calcPath;
	std::vector<int> stolenItemsList;

public:
	SA();
	~SA();
	int pathInit(std::vector<int> &calcPath, int noOfCities);
	int calculateCost(std::vector<std::vector<int>> &adjacancyMatrix, std::vector<int> &calcPath, int noOfCities);

	int calculateProfit();  
	
	int randInt(int l, int r);
	double randFraction(void);
	std::vector<Item> getItemsFromCurrCity(int currCity, std::vector<Item> items);

	std::vector<int> getCalcPath();
	std::vector<int> getStolenItemsList();

	int solverSA(std::vector<std::vector<int>> &adjacancyMatrix, std::vector<Item> &valuableItemsMatrix, std::vector<int> &calcPath, 
				 std::vector<int> &stolenItemsList, int noOfCities, int noOfItems); 
};

