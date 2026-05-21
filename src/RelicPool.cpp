#include "RelicPool.h"
#include "StatRelic.h"
#include "UtilityRelic.h"
#include "ProbabilityRelic.h"
#include "MoveRelic.h"
#include "Move.h"
#include <cstdlib>

Relic* RelicPool::getRandomRelic() {
    int rarityRoll = rand() % 100;

    if (rarityRoll < 70) {
        return getRandomCommonRelic();      // 70%
    } else if (rarityRoll < 95) {
        return getRandomRareRelic();        // 25%
    } else {
        return getRandomLegendaryRelic();   // 5%
    }
}

Relic* RelicPool::getRandomCommonRelic() {
    int roll = rand() % 16;

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
                "Increases ATK by 10.",
                RelicRarity::COMMON,
                StatType::ATK,
                10
            );

        case 2:
            return new StatRelic(
                "Stone Shield",
                "Increases DEF by 10.",
                RelicRarity::COMMON,
                StatType::DEF,
                10
            );

        case 3:
            return new StatRelic(
                "Apprentice Ring",
                "Increases SP.ATK by 10.",
                RelicRarity::COMMON,
                StatType::SPATK,
                10
            );

        case 4:
            return new StatRelic(
                "Cloth Mantle",
                "Increases SP.DEF by 10.",
                RelicRarity::COMMON,
                StatType::SPDEF,
                10
            );

        case 5:
            return new StatRelic(
                "Traveler Boots",
                "Increases SPD by 6.",
                RelicRarity::COMMON,
                StatType::SPD,
                6
            );

        case 6:
            return new UtilityRelic(
                "Potion Belt",
                "Adds 1 extra potion slot.",
                RelicRarity::COMMON,
                UtilityType::EXPAND_POTION_SLOTS,
                1
            );

        case 7:
            return new UtilityRelic(
                "Healing Charm",
                "Heals 10% HP when going up a floor.",
                RelicRarity::COMMON,
                UtilityType::HEAL_ON_FLOOR,
                10
            );

        case 8:
            return new StatRelic(
                "Iron Helmet",
                "Increases DEF by 6.",
                RelicRarity::COMMON,
                StatType::DEF,
                3
            );

        case 9:
            return new StatRelic(
                "Small Ruby",
                "Increases ATK by 6.",
                RelicRarity::COMMON,
                StatType::ATK,
                6
            );

        case 10:
            return new StatRelic(
                "Blue Crystal",
                "Increases SP.ATK by 6.",
                RelicRarity::COMMON,
                StatType::SPATK,
                6
            );

        case 11:
            return new StatRelic(
                "Silver Thread",
                "Increases SP.DEF by 6.",
                RelicRarity::COMMON,
                StatType::SPDEF,
                6
            );

        case 12:
            return new StatRelic(
                "Light Sandals",
                "Increases SPD by 4.",
                RelicRarity::COMMON,
                StatType::SPD,
                4
            );

        case 13:
            return new StatRelic(
                "Old Medal",
                "Increases max HP by 10.",
                RelicRarity::COMMON,
                StatType::HP,
                10
            );

        case 14:
            return new ProbabilityRelic(
                "Steady Hand",
                "Increases accuracy of all moves by 5.",
                RelicRarity::COMMON,
                ProbabilityType::MOVE_ACCURACY,
                5
            );

        default:
            return new UtilityRelic(
                "Minor Scholar Coin",
                "Increases EXP gained by 10%.",
                RelicRarity::COMMON,
                UtilityType::EXP_BOOST,
                10
            );
    }
}

Relic* RelicPool::getRandomRareRelic() {
    int roll = rand() % 8;

    switch (roll) {
        case 0:
            return new StatRelic(
                "Hunter Boots",
                "Increases SPD by 12.",
                RelicRarity::RARE,
                StatType::SPD,
                12
            );

        case 1:
            return new StatRelic(
                "Golden Ring",
                "Increases SP.ATK by 16.",
                RelicRarity::RARE,
                StatType::SPATK,
                16
            );

        case 2:
            return new ProbabilityRelic(
                "Sharp Eye",
                "Increases accuracy of all moves by 10.",
                RelicRarity::RARE,
                ProbabilityType::MOVE_ACCURACY,
                10
            );

        case 3:
            return new UtilityRelic(
                "Scholar Coin",
                "Increases EXP gained by 20%.",
                RelicRarity::RARE,
                UtilityType::EXP_BOOST,
                20
            );

        case 4:
            return new StatRelic(
                "Knight Armor",
                "Increases DEF by 20.",
                RelicRarity::RARE,
                StatType::DEF,
                20
            );

        case 5:
            return new StatRelic(
                "Mystic Cape",
                "Increases SP.DEF by 20.",
                RelicRarity::RARE,
                StatType::SPDEF,
                20
            );

        case 6:
            return new UtilityRelic(
                "Large Potion Belt",
                "Adds 2 extra potion slots.",
                RelicRarity::RARE,
                UtilityType::EXPAND_POTION_SLOTS,
                2
            );

        default:
            return new MoveRelic(
                "Flame Scroll",
                "Adds Flame Burst or boosts it if already owned.",
                RelicRarity::RARE,
                Move(
                    "Flame Burst",
                    MoveType::SPECIAL,
                    110,
                    90,
                    MoveEffect::NONE,
                    MoveTarget::ENEMY
                ),
                50
            );
    }
}

Relic* RelicPool::getRandomLegendaryRelic() {
    int roll = rand() % 4;

    switch (roll) {
        case 0:
            return new StatRelic(
                "Dragon Heart",
                "Increases max HP by 50.",
                RelicRarity::LEGENDARY,
                StatType::HP,
                50
            );

        case 1:
            return new UtilityRelic(
                "Phoenix Feather",
                "Restores 30% extra HP when leveling up.",
                RelicRarity::LEGENDARY,
                UtilityType::RESTORE_ON_LEVEL_UP,
                30
            );

        case 2:
            return new StatRelic(
                "Ancient Blade",
                "Increases ATK by 30.",
                RelicRarity::LEGENDARY,
                StatType::ATK,
                30
            );

        default:
            return new MoveRelic(
                "Thunder Tome",
                "Adds Thunder Strike or boosts it if already owned.",
                RelicRarity::LEGENDARY,
                Move(
                    "Thunder Strike",
                    MoveType::SPECIAL,
                    100,
                    100,
                    MoveEffect::NONE,
                    MoveTarget::ENEMY
                ),
                50
            );
    }
}