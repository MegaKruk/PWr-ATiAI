
#ifndef TTP1
#define TTP1

#include <stdlib.h>
#include <string>
#include <limits.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <vector>
#include "Item.h"
#include "Knapsack.h"

class TTP
{
private:
	int noOfCities;
	int noOfItems;
	std::vector<std::vector<float>> adjacancyMatrix;
	std::vector<Item> valuableItemsMatrix;

public:
	int start();
	int initTTP();
	
	int getNoOfCities();
	int getNoOfItems();
	/*float getItemProfit(int);
	float getItemWeight(int);
	int getItemCity(int);
	float getMaxWeight();
	float getCurrWeight();
	float getCurrRentingRatio();
	float getMinSpeed();
	float getMaxSpeed();
	float getCurrSpeed();*/
	std::vector<int> getStolenItemsList();
	std::vector<std::vector<float>> getAdjacancyMatrix();

	//void increaseCurrWeight(float);
	//void setCurrSpeed();

	TTP();
	~TTP();
};

#endif //UNTITLED1_TTP