//
// Created by Piotr on 27.10.2019.
//

#include "GreedySearch.h"
#include "Item.h"
#include "Knapsack.h"

std::pair<std::vector<int>, std::vector<std::string>>
GreedySearch::evaluateGreedy(int startCity, std::vector<std::vector<float>> cities, std::vector<Item> allItems,
                             Knapsack knapsack) {

    std::vector<int> visitedCities;
    std::vector<int> citiesResult;
    std::vector<std::string> knapsackResult;
    for (int k = 0; k < cities.size(); ++k) {
        visitedCities.push_back(k);
    }
    int currentCity = startCity;
    while (!visitedCities.empty()) {
        citiesResult.push_back(currentCity);
        visitedCities.erase(std::remove(visitedCities.begin(), visitedCities.end(), currentCity), visitedCities.end());
        int indexPickedItem = -1;
        float bestResult = -10000;
        float bestDistance = 100000;
        std::vector<Item> items = getItemsFromCurrCity(currentCity, allItems);
        for (int toCity: visitedCities) {
            //if no items then evaluate distance
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
                        indexPickedItem = j;
                    }
                }
            }

        }
        knapsackResult.push_back(std::to_string(currentCity) + " " + std::to_string(indexPickedItem));
    }
    citiesResult.push_back(startCity);
    std::pair<std::vector<int>, std::vector<std::string>> result;
    result.first = citiesResult;
    result.second = knapsackResult;
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
