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
		float profit;
		float weight;
		int assignedCity;
	};

	struct thiefKnapsack
	{
		float maxWeight;
		float currWeight;
		float rentingRatio;
		float minSpeed;
		float maxSpeed;
		float currSpeed;
		std::vector<int> stolenItemsList;
	} myKnapsack;

	int noOfCities;
	int noOfItems;
	std::vector<std::vector<float>> adjacancyMatrix;
	std::vector<valuableItem> valuableItemsMatrix;

public:
	int start();
	int initTTP();
	
	int getNoOfCities();
	int getNoOfItems();
	float getItemProfit(int);
	float getItemWeight(int);
	int getItemCity(int);
	float getMaxWeight();
	float getCurrWeight();
	float getCurrRentingRatio();
	float getMinSpeed();
	float getMaxSpeed();
	float getCurrSpeed();
	std::vector<int> getStolenItemsList();
	std::vector<std::vector<float>> getAdjacancyMatrix();

	void increaseCurrWeight(float);
	void setCurrSpeed();

	TTP();
	~TTP();
};