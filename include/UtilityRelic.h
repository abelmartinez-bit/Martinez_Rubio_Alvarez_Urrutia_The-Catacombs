#pragma once
#include "Relic.h"
#include "Player.h"

//tipos de mejoras de utlidad
enum class UtilityType {
    EXPAND_POTION_SLOTS,  
    HEAL_ON_FLOOR,        
    EXP_BOOST,            
    RESTORE_ON_LEVEL_UP   // mas hp al subir de nivel, los demas se explican solos
};

//clase para relics com efectos de utilidad general
class UtilityRelic : public Relic { 
private:
    UtilityType utilityType;
    int value;

public:
    UtilityRelic(std::string name, std::string description, RelicRarity rarity,
                 UtilityType utilityType, int value);

    UtilityType getUtilityType() const { return utilityType; }
    int getValue() const { return value; }

    void applyEffect(Player& player) override;
};