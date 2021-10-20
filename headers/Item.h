#pragma once

#include <string>

enum class Rarity { Rare, Uncommon, Common };

struct Item {
    std::string name;
    Rarity rarity;
    int value;
};
