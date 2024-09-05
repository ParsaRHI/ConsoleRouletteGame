#pragma once
#include <iostream>
#include "color.hpp"

enum ItemRarity{
    UNCOMMON,
    COMMON,
    RARE,
    EPIC,
    LEGENDARY
};
int IDNum;
class Item{

    std::string name;
    int ID=0;

    ItemRarity rarity;
public:
    enum ItemType{
        HEALING,
        SEE,REFILL
    };
    ItemType type;
    float value;
    explicit Item(ItemType type_val, float value_val,std::string_view name_val,ItemRarity rarity_val):
            name(name_val),type(type_val),rarity(rarity_val), value(value_val)
    {
        ID=IDNum;
        ID+=1;
        IDNum=ID;
    }
    [[nodiscard]] int getID() const { return ID; }
    [[nodiscard]] std::string_view getName() const { return name; }
    [[nodiscard]] ItemRarity getRarity() const { return rarity; }
    [[nodiscard]] ItemType getType() const { return type; }
    [[nodiscard]] float getValue() const { return value; }
    void getItemColor()const{
        switch (this->getRarity()) {

            case COMMON:
                std::cout << dye::white(this->getName());
                break;
            case UNCOMMON:
                std::cout << dye::green(this->getName());
                break;
            case RARE:
                std::cout << dye::aqua(this->getName());
                break;
            case EPIC:
                std::cout << dye::purple(this->getName());
                break;
            case LEGENDARY:
                std::cout << dye::yellow(this->getName());
                break;

        }
    }

};