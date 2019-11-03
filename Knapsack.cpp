//
// Created by Piotr on 27.10.2019.
//

#include "Knapsack.h"

float Knapsack::getMaxWeight() const {
    return maxWeight;
}

void Knapsack::setMaxWeight(float maxWeight) {
    Knapsack::maxWeight = maxWeight;
}

float Knapsack::getCurrWeight() const {
    return currWeight;
}

void Knapsack::setCurrWeight(float currWeight) {
    Knapsack::currWeight = currWeight;
}

float Knapsack::getRentingRatio() const {
    return rentingRatio;
}

void Knapsack::setRentingRatio(float rentingRatio) {
    Knapsack::rentingRatio = rentingRatio;
}

float Knapsack::getMinSpeed() const {
    return minSpeed;
}

void Knapsack::setMinSpeed(float minSpeed) {
    Knapsack::minSpeed = minSpeed;
}

float Knapsack::getMaxSpeed() const {
    return maxSpeed;
}

void Knapsack::setMaxSpeed(float maxSpeed) {
    Knapsack::maxSpeed = maxSpeed;
}

// ?????
float Knapsack::getCurrSpeed(float stolenItemWeight) const {
    return getMaxSpeed() - getCurrWeight() * (getMaxSpeed() - getMinSpeed()) / stolenItemWeight;
}

//to zmienilem. teraz jest dobrze
void Knapsack::setCurrSpeed() 
{
    Knapsack::currSpeed = minSpeed + (((maxWeight - currWeight) / maxWeight) * (maxSpeed - minSpeed));
}

const std::vector<int> &Knapsack::getStolenItemsList() const {
    return stolenItemsList;
}

void Knapsack::setStolenItemsList(const std::vector<int> &stolenItemsList) {
    Knapsack::stolenItemsList = stolenItemsList;
}

void Knapsack::clearKnapsack()
{
    Knapsack::currWeight = 0; 
    Knapsack::currSpeed = maxSpeed; 
    Knapsack::stolenItemsList.clear(); 
    Knapsack::stolenItemsList.resize(0); 
}

void Knapsack::increaseCurrWeight(float stolenItemWeight)
{
    Knapsack::currWeight += stolenItemWeight;
}

Knapsack::Knapsack(float maxWeight, float currWeight, float rentingRatio, float minSpeed, float maxSpeed,
                   float currSpeed) : maxWeight(maxWeight), currWeight(currWeight), rentingRatio(rentingRatio),
                                      minSpeed(minSpeed), maxSpeed(maxSpeed), currSpeed(currSpeed) { }

Knapsack::Knapsack() { }
