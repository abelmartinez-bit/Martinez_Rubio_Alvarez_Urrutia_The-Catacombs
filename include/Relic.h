#pragma once
#include <string>

class Player; // Declaracion para no tener un include circular

enum class RelicRarity {
    COMMON,
    RARE,
    LEGENDARY
};

//clase abstracta para objetos que el jugador puede encontrar y equipar, con efectos únicos
class Relic {
//para que las hereden las subclases de los tipos de relic
protected:
    std::string name;
    std::string description;
    RelicRarity rarity;

public:
    Relic(std::string name, std::string description, RelicRarity rarity);

    std::string getName() const { return name; }
    std::string getDescription() const { return description; }
    RelicRarity getRarity() const { return rarity; }

    // virtual = 0 para que no existan objetos normales Relic, solo de sus hijos que implementen applyEffect
    // con los que usaremos con pointers
    virtual void applyEffect(Player& player) = 0;
    virtual ~Relic() = default;
};