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
    int currWeight = 0;
    for (int k = 0; k < cities.size(); ++k) {
        visitedCities.push_back(k);
    }
    int currentCity = startCity;
    while (!visitedCities.empty()) {
        citiesResult.push_back(currentCity);
        int actualCity = currentCity;
        visitedCities.erase(std::remove(visitedCities.begin(), visitedCities.end(), currentCity), visitedCities.end());
        int indexPickedItem = -1;
        float bestResult = -10000;
        float bestDistance = 100000;
        std::vector<Item> items = getItemsFromCurrCity(currentCity, allItems);
        for (int toCity: visitedCities) {
            if (items.size() == 0) {
                float distance = cities[currentCity][toCity];
                if (distance < bestDistance) {
                    bestDistance = distance;
                    currentCity = toCity;
                }
            } else {
                for (int j = 0; j < items.size(); ++j) {
                    float currSpeed = knapsack.getCurrSpeed(items[j].getWeight());
                    float distance = cities[currentCity][toCity];
                    float g = items[j].getProfit() - distance / currSpeed;
                    if (g > bestResult) {
                        bestResult = g;
                        currentCity = toCity;
                        indexPickedItem = items[j].getIdItem();
                        currWeight += items[j].getWeight();
                    }
                }
            }
        }
//        knapsackResult.push_back(std::to_string(currentCity) + " " + std::to_string(indexPickedItem));
        if (indexPickedItem == -1) indexPickedItem = getBestIfExist(actualCity, knapsack, allItems, currWeight);
        knapsackResult.push_back(std::make_pair(actualCity, indexPickedItem));
    }
    citiesResult.push_back(startCity);
    std::pair<std::vector<int>, std::vector<int>> result;
    result.first = citiesResult;
    std::vector<int> normal = repairKnapsack(knapsackResult, allItems.size());
    repairKnapsack(knapsackResult, allItems.size());
    result.second = normal;
    return result;
}

std::vector<Item> GreedySearch::getItemsFromCurrCity(int currCity, std::vector<Item> allItems) {
    std::vector<Item> currItems;
    for (int i = 0; i < allItems.size(); i++) {
        if (allItems[i].getAssignedCity() == currCity)
            currItems.push_back(allItems[i]);
    }
    return currItems;
}

std::vector<int> GreedySearch::repairKnapsack(std::vector<std::pair<int, int>> knapsack, int size) {
    std::vector<int> normal;
    for (int i = 0; i < size; i++) {
        bool found = false;
        for (int j = 0; j < knapsack.size() && !found; j++) {
            if (i == knapsack[j].second) {
                normal.push_back(knapsack[j].first);
                found = true;
            }
        }
        if (!found)
            normal.push_back(-1);
    }
    return normal;
}

int GreedySearch::getBestIfExist(int city, Knapsack knapsack, std::vector<Item> allItems, int currWeight) {
    std::vector<Item> items = getItemsFromCurrCity(city, allItems);
    for (int j = 0; j < items.size(); ++j) {
        if (knapsack.getMaxWeight() < currWeight + items[j].getWeight())
            return items[j].getIdItem();
    }
    return 0;
}
