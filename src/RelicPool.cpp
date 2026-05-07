#include "RelicPool.h"
#include "StatRelic.h"
#include "UtilityRelic.h"
#include "ProbabilityRelic.h"
#include "MoveRelic.h"
#include <cstdlib>

Relic* RelicPool::getRandomRelic() {
    int rarityRoll = rand() % 100;

    if (rarityRoll < 70) {
        return getRandomCommonRelic();
    } else if (rarityRoll < 95) {
        return getRandomRareRelic();
    } else {
        return getRandomLegendaryRelic();
    }
}

Relic* RelicPool::getRandomCommonRelic() {
    int roll = rand() % 8;

    switch (roll) {
        case 0:
            return new StatRelic(
                "Heart Stone",
                "Increases max HP by 20.",
                RelicRarity::COMMON,
                StatType::HP,
                20
            );

        case 1:
            return new StatRelic(
                "Warrior Fang",
                "Increases ATK by 5.",
                RelicRarity::COMMON,
                StatType::ATK,
                5
            );

        case 2:
            return new StatRelic(
                "Stone Shield",
                "Increases DEF by 5.",
                RelicRarity::COMMON,
                StatType::DEF,
                5
            );

        case 3:
            return new StatRelic(
                "Apprentice Ring",
                "Increases SP.ATK by 5.",
                RelicRarity::COMMON,
                StatType::SPATK,
                5
            );

        case 4:
            return new StatRelic(
                "Cloth Mantle",
                "Increases SP.DEF by 5.",
                RelicRarity::COMMON,
                StatType::SPDEF,
                5
            );

        case 5:
            return new StatRelic(
                "Traveler Boots",
                "Increases SPD by 3.",
                RelicRarity::COMMON,
                StatType::SPD,
                3
            );

        case 6:
            return new UtilityRelic(
                "Potion Belt",
                "Adds 1 extra potion slot.",
                RelicRarity::COMMON,
                UtilityType::EXPAND_POTION_SLOTS,
                1
            );

        default:
            return new UtilityRelic(
                "Healing Charm",
                "Heals 15 HP when obtained.",
                RelicRarity::COMMON,
                UtilityType::HEAL_ON_FLOOR,
                15
            );
    }
}

Relic* RelicPool::getRandomRareRelic() {
    int roll = rand() % 4;

    switch (roll) {
        case 0:
            return new StatRelic(
                "Hunter Boots",
                "Increases SPD by 6.",
                RelicRarity::RARE,
                StatType::SPD,
                6
            );

        case 1:
            return new StatRelic(
                "Golden Ring",
                "Increases SP.ATK by 8.",
                RelicRarity::RARE,
                StatType::SPATK,
                8
            );

        case 2:
            return new ProbabilityRelic(
                "Sharp Eye",
                "Increases accuracy of all moves by 5.",
                RelicRarity::RARE,
                ProbabilityType::MOVE_ACCURACY,
                5
            );

        default:
            return new UtilityRelic(
                "Large Potion Belt",
                "Adds 2 extra potion slots.",
                RelicRarity::RARE,
                UtilityType::EXPAND_POTION_SLOTS,
                2
            );
    }
}

Relic* RelicPool::getRandomLegendaryRelic() {
    int roll = rand() % 2;

    switch (roll) {
        case 0:
            return new StatRelic(
                "Dragon Heart",
                "Increases max HP by 50.",
                RelicRarity::LEGENDARY,
                StatType::HP,
                50
            );

        default:
            return new StatRelic(
                "Ancient Blade",
                "Increases ATK by 15.",
                RelicRarity::LEGENDARY,
                StatType::ATK,
                15
            );
    }
}