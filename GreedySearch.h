//
// Created by Piotr on 27.10.2019.
//

#ifndef UNTITLED1_GREEDYSEARCH_H
#define UNTITLED1_GREEDYSEARCH_H

#include <vector>
#include "TTP.h"
#include "Knapsack.h"
#include "Item.h"

class GreedySearch
{
private:

public:

    std::vector<Item> getItemsFromCurrCity(int currCity, std::vector<Item> items);

    std::pair<std::vector<int>, std::vector<int>>
    evaluateGreedy(int startCity, std::vector<std::vector<float>> cities, std::vector<Item> allItems,
                   Knapsack knapsack);

    std::vector<int> repairKnapsack(std::vector<std::pair<int, int>> knapsack, int size);
};
#endif //UNTITLED1_GREEDYSEARCH_H
