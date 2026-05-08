#include "../include/UtilityRelic.h"
#include <iostream>

UtilityRelic::UtilityRelic(std::string name, std::string description, RelicRarity rarity,
                            UtilityType utilityType, int value)
            : Relic(name, description, rarity), utilityType(utilityType), value(value) {}


void UtilityRelic::applyEffect(Player& player) {
    switch (utilityType) {
        case UtilityType::EXPAND_POTION_SLOTS:
            player.expandPotionSlots(value);
            std::cout << "Potion slots expanded by " << value << "!\n";
            break;
        case UtilityType::HEAL_ON_FLOOR:
            // trabajado por el Floor system
            player.heal(value);
            std::cout << "Healed " << value << " HP on entering floor!\n";
            break;
        // funciones ya aplicadas en Player para los efectos de las reliquias, solo se llaman desde applyEffect segun el tipo de reliquia
        case UtilityType::EXP_BOOST:
            player.addExpBoost(value);
            std::cout << "EXP gain increased by " << value << "%!\n";
            break;

        case UtilityType::RESTORE_ON_LEVEL_UP:
            player.addRestoreOnLevelUp(value);
            std::cout << "HP restore on level up increased by " << value << "%!\n";
            break;
    }
}