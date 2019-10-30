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

    std::pair<std::vector<int>, std::vector<std::string>>
    evaluateGreedy(int startCity, std::vector<std::vector<float>> vector, std::vector<Item> vector1, Knapsack knapsack);

    std::vector<Item> getItemsFromCurrCity(int currCity, std::vector<Item> items);
};
#endif //UNTITLED1_GREEDYSEARCH_H
