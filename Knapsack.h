//
// Created by Piotr on 27.10.2019.
//

#ifndef UNTITLED1_KNAPSACK_H
#define UNTITLED1_KNAPSACK_H


#include <vector>

class Knapsack {

private:
    float maxWeight;
    float currWeight;
    float rentingRatio;
    float minSpeed;
    float maxSpeed;
    float currSpeed;
    std::vector<int> stolenItemsList;
public:
    Knapsack();

    Knapsack(float maxWeight, float currWeight, float rentingRatio, float minSpeed, float maxSpeed, float currSpeed);

    float getMaxWeight() const;

    void setMaxWeight(float maxWeight);

    float getCurrWeight() const;

    void setCurrWeight(float currWeight);

    float getRentingRatio() const;

    void setRentingRatio(float rentingRatio);

    float getMinSpeed() const;

    void setMinSpeed(float minSpeed);

    float getMaxSpeed() const;

    void setMaxSpeed(float maxSpeed);

    float getCurrSpeed() const;

    void setCurrSpeed();

    const std::vector<int> &getStolenItemsList() const;

    void setStolenItemsList(const std::vector<int> &stolenItemsList);

    float getCurrSpeed(float stolenItemWeight) const;

    void clearKnapsack();

    void increaseCurrWeight(float);
};


#endif //UNTITLED1_KNAPSACK_H
