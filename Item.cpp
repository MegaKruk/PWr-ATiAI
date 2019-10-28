//
// Created by Piotr on 27.10.2019.
//

#include "Item.h"
float Item::getProfit() const {
    return profit;
}

void Item::setProfit(float profit) {
    Item::profit = profit;
}

float Item::getWeight() const {
    return weight;
}

void Item::setWeight(float weight) {
    Item::weight = weight;
}

int Item::getAssignedCity() const {
    return assignedCity;
}

void Item::setAssignedCity(int assignedCity) {
    Item::assignedCity = assignedCity;
}

Item::Item(float profit, float weight, int assignedCity) : profit(profit), weight(weight), assignedCity(assignedCity) {}

Item::~Item() {

}
