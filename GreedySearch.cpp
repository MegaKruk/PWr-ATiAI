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
    float currProfit = 0;
    while (currWeight + allItems[i].getWeight() < knapsack.getMaxWeight()) {
        currWeight += allItems[i].getWeight();
        pickingVector.push_back(allItems[i].getIdItem());
        currProfit += allItems[i].getProfit();
        i++;
    }
    weight = currWeight;
    profit = (int) currProfit;
    return pickingVector;
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
