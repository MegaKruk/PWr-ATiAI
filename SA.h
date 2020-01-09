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

class SA
{
private:
	std::vector<int> calcPath;
	std::vector<int> stolenItemsList;
	double Tmax = 1.0;
	double Tmin = 0.0000000001;
	double Tcoeff = 0.9;
	int timeLimitSec = 10;

public:
	SA();
	~SA();
	
	std::vector<int> pathInit(int noOfCities);
	std::vector<int> itemsInit(int noOfItems, Knapsack& knapsack, std::vector<Item> &valuableItemsMatrix);
	int paramsInit();

	float calculateWeight(std::vector<Item> &valuableItemsMatrix, std::vector<int> &stolenItemsList, int noOfItems);
	float calculateDist(std::vector<std::vector<float>> &adjacancyMatrix, std::vector<int> &calcPath, int noOfCities);
	float calculateProfit(std::vector<std::vector<float>> &adjacancyMatrix, std::vector<Item> &valuableItemsMatrix, 
						  std::vector<int> &calcPath, std::vector<int> &stolenItemsList, int noOfCities, int noOfItems, 
						  Knapsack& knapsack);  

	float solverSA(std::vector<std::vector<float>> &adjacancyMatrix, std::vector<Item> &valuableItemsMatrix,  
				   std::vector<int> &calcPath, std::vector<int> &stolenItemsList, int noOfCities, int noOfItems, 
				   Knapsack& knapsack); 

	int randNum(int l, int r);
	double randFraction(void);

	std::vector<Item> getItemsFromCurrCity(int currCity, std::vector<Item> items);
	std::vector<int> getCalcPath();
	std::vector<int> getStolenItemsList();
	double getTmax();
	double getTmin();
	double getTcoeff();
	int getTimeLimitSec();

	void setTmax(double newTmax);
	void setTmin(double newTmin);
	void setTcoeff(double newTcoeff);
	void setTimeLimitSec(int newtimeLimitSec);
};

