#pragma once
#include "Relic.h"
#include "Player.h"

enum class ProbabilityType {
    CORRUPT_DOOR,    // cambia chance de aparecer una puerta corrupta
    MINIBOSS_DROP,   // cambia chance de drop de rare/legendary relic de miniboss
    ENEMY_DROP,      // cambia chance de drop de rare relic de enemy normal 
    MOVE_ACCURACY    // cambia accuracy de tus moves
};

//relic que afecta probabilidades en el juego
class ProbabilityRelic : public Relic {
private:
    ProbabilityType probabilityType;
    float modifier; // positivo = mas chance, negativo = menos

public:
    ProbabilityRelic(std::string name, std::string description, RelicRarity rarity, 
        ProbabilityType probabilityType, float modifier);

    //getters
    ProbabilityType getProbabilityType() const;
    float getModifier() const;

    void applyEffect(Player& player) override;
};