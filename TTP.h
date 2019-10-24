#include <stdlib.h>
#include <string>
#include <limits.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <vector>

class TTP
{
private:
	struct valuableItem
	{
		int profit;
		int weight;
		int assignedCity;
	};

	struct thiefKnapsack
	{
		int maxWeight;
		int currWeight;
		int rentingRatio;
		float maxSpeed;
		float minSpeed;
		float currSpeed;
		std::vector<valuableItem> stolenItems;

	};

	std::vector<std::vector<int>> adjacancyMatrix;
	std::vector<std::vector<valuableItem>> valuableItemsMatrix;
	int noOfCities;
	int noOfItems;

public:
	TTP();
	~TTP();
	int initTTP();
};