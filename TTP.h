
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
#include "Knapsack.h"
#include "Item.h"

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

	std::vector<int> getStolenItemsList();
	std::vector<std::vector<float>> getAdjacancyMatrix();

	TTP();
	~TTP();
};

#endif //UNTITLED1_TTP