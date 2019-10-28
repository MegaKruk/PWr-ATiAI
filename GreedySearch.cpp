//
// Created by Piotr on 27.10.2019.
//

#include "GreedySearch.h"
#include "Item.h"
#include "Knapsack.h"

std::pair<std::vector<int>, std::vector<int>>
GreedySearch::evaluateGreedy(int startCity, std::vector<std::vector<float>> cities, std::vector<Item> allItems,
                             Knapsack knapsack) {


    std::vector<int> vCities;
    std::vector<int> citiesResult;
    std::vector<std::string> knapsackResult;


    for (int k = 0; k < cities.size(); ++k) {
        vCities.push_back(k);
    }
    vCities.erase(std::remove(vCities.begin(), vCities.end(), startCity), vCities.end());

    int currentCity = startCity;
    for (int z = 0; z < vCities.size(); ++z) {
        citiesResult.push_back(currentCity);
        int indexPickedItem = -1;
        float bestResult = -1000;
        std::vector<Item> items = getItemsFromCurrCity(currentCity, allItems);
        for (int i = 0; i < vCities.size(); ++i) {
            for (int j = 0; j < items.size(); ++j) {
                float currSpeed = knapsack.getCurrSpeed(items[j].getWeight());
                float distance = cities[currentCity][vCities[i]];
                float g = items[j].getProfit() - distance / currSpeed;
                if (g > bestResult) {
                    bestResult = g;
                    currentCity = vCities[i];
                    indexPickedItem = j;
                }
            }
        }
        knapsackResult.push_back(std::to_string(currentCity) + " " + std::to_string(indexPickedItem));
        vCities.erase(std::remove(vCities.begin(), vCities.end(), currentCity), vCities.end());

    }

    for (int i = 0; i < cities.size(); ++i) {
        for (int j = 0; j < cities[i].size(); ++j) {
            std::cout << cities[i][j];
        }
    }


    return std::pair<std::vector<int>, std::vector<int>>();
}

std::vector<Item> GreedySearch::getItemsFromCurrCity(int currCity, std::vector<Item> allItems) {
    std::vector<Item> currItems;
    for (int i = 0; i < allItems.size(); i++) {
        if (allItems[i].getAssignedCity() == currCity)
            currItems.push_back(allItems[i]);
    }
    return currItems;
}
