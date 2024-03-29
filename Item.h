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
    int id_item;
    float profit;
    float weight;
    int assignedCity;
public:
    bool operator<(const Item &rhs) const;

public:
    int getIdItem() const;

    void setIdItem(int idItem);

public:
    Item();

    Item(float profit, float weight, int assignedCity);

    virtual ~Item();
};


#endif //UNTITLED1_ITEM_H
