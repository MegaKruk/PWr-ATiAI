//
// Created by Piotr on 27.10.2019.
//

#ifndef UNTITLED1_ITEM_H
#define UNTITLED1_ITEM_H


class Item {
public:

    float getProfit() const;

    void setProfit(float profit);

    float getWeight() const;

    void setWeight(float weight);

    int getAssignedCity() const;

    void setAssignedCity(int assignedCity);

private:
    float profit;
    float weight;
    int assignedCity;
public:
    Item(float profit, float weight, int assignedCity);

    virtual ~Item();
};


#endif //UNTITLED1_ITEM_H
