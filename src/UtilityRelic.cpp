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
        case UtilityType::EXP_BOOST:
            // Trabajo del combatEngine o futuro PlayerManager para dar mas exp al ganar
            std::cout << "EXP gain increased by " << value << "%!\n";
            break;
        case UtilityType::RESTORE_ON_LEVEL_UP:
            // aun no se donde se trabajara pero probablemente en el levelUp del Player, aumentando el restore base de 5% del maxHp
            std::cout << "HP restore on level up increased by " << value << "%!\n";
            break;
    }
}