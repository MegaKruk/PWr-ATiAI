//
// Created by Piotr on 27.10.2019.
//

#include "GreedySearch.h"
#include "Item.h"
#include "Knapsack.h"

std::pair<std::vector<int>, std::vector<int>>
GreedySearch::evaluateGreedy(int startCity, std::vector<std::vector<float>> cities, std::vector<Item> allItems,
                             Knapsack knapsack) {

    std::vector<int> visitedCities;
    std::vector<int> citiesResult;
    std::vector<std::pair<int, int>> knapsackResult;
    std::vector<int> intknapsack;
    for (int k = 0; k < cities.size(); ++k) {
        visitedCities.push_back(k);
    }
    int currentCity = startCity;
    while (!visitedCities.empty()) {
        citiesResult.push_back(currentCity);
        visitedCities.erase(std::remove(visitedCities.begin(), visitedCities.end(), currentCity), visitedCities.end());
        float bestDistance = 100000;
        for (int toCity: visitedCities) {
            float distance = cities[currentCity][toCity];
            if (distance < bestDistance) {
                bestDistance = distance;
                currentCity = toCity;

            }
        }
    }

    citiesResult.push_back(startCity);
    std::pair<std::vector<int>, std::vector<int>> result;
    result.first = citiesResult;
    result.second = pickingPlan(allItems, knapsack);
    return result;
}

std::vector<int> GreedySearch::pickingPlan(std::vector<Item> allItems, Knapsack knapsack) {
    std::vector<int> pickingVector;
    std::sort(allItems.begin(), allItems.end());
    int i = 0;
    int currWeight = 0;
    //float currProfit = 0;
    while (currWeight + allItems[i].getWeight() < knapsack.getMaxWeight()) {
        currWeight += allItems[i].getWeight();
        pickingVector.push_back(allItems[i].getIdItem());
        //currProfit += allItems[i].getProfit();
        i++;
    }
    weight = currWeight;
    //profit = currProfit;
    return pickingVector;
}

float GreedySearch::calculateProfit(std::vector<std::vector<float>> &adjacancyMatrix, std::vector<Item> &valuableItemsMatrix, 
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

float GreedySearch::solverGreedy(std::vector<std::vector<float>> &adjacancyMatrix, std::vector<Item> &valuableItemsMatrix,  
                                 int noOfCities, int noOfItems, Knapsack& knapsack)
{
    std::pair<std::vector<int>, std::vector<int>> result = evaluateGreedy(0, adjacancyMatrix, valuableItemsMatrix, knapsack);
    profit = calculateProfit(adjacancyMatrix, valuableItemsMatrix, result.first, result.second, 
                             noOfCities, noOfItems, knapsack);

    std::cout << "Path:\t";
    for (auto i = result.first.begin(); i != result.first.end(); ++i)
        std::cout << *i << "\t";
    std::cout << std::endl;
    std::cout << "Items:\t";
    for (auto i = result.second.begin(); i != result.second.end(); ++i)
        std::cout << *i << "\t";
    std::cout << std::endl;
    std::cout << "Weight:\t" << weight << " / " << knapsack.getMaxWeight();
    std::cout << std::endl;
    std::cout << "Profit:\t" << profit << std::endl;
    return profit;
}

int GreedySearch::getWeight() const {
    return weight;
}

void GreedySearch::setWeight(int weight) {
    GreedySearch::weight = weight;
}

float GreedySearch::getProfit() const {
    return profit;
}

void GreedySearch::setProfit(float profit) {
    GreedySearch::profit = profit;
}
