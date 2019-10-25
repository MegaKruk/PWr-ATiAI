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
	int getItemProfit(int);
	int getItemWeight(int);
	int getItemCity(int);
	int getMaxWeight();
	int getCurrWeight();
	float getCurrRentingRatio();
	float getMinSpeed();
	float getMaxSpeed();
	float getCurrSpeed();
	std::vector<int> getStolenItemsList();
	std::vector<std::vector<float>> getAdjacancyMatrix();

	void setCurrWeight(int);
	void setCurrSpeed();

	TTP();
	~TTP();
};