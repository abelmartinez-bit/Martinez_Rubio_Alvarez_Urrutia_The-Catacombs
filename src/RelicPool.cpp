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
    int roll = rand() % 20;

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
                6
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
        
        case  15:
            return new StatRelic(
                "Iron Knuckle",
                "Increases ATK by 8.",
                RelicRarity::COMMON,
                StatType::ATK,
                8
            );
    
        case 16:
              return new StatRelic(
                "Stone Guard",
                "Increases DEF by 8.",
                RelicRarity::COMMON,
                StatType::DEF,
                8
             );
        case 17:
            return new StatRelic(
                "Sage's Amulet",
                "Increases SP.ATK by 8.",
                RelicRarity::COMMON,
                StatType::SPATK,
                8
            );

        case 18:
            return new UtilityRelic(
                "Healing Talisman",
                "Heals 15% HP when going up a floor.",
                RelicRarity::COMMON,
                UtilityType::HEAL_ON_FLOOR,
                15
            );

        case 19:
            return new UtilityRelic(
                "Apprentice's Coin",
                "Increases EXP gained by 15%.",
                RelicRarity::COMMON,
                UtilityType::EXP_BOOST,
                15
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
    int roll = rand() % 12;

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

        case 7:
            return new MoveRelic(
                "Ice Scroll",
                "Adds Ice Shard or boosts it if already owned.",
                RelicRarity::RARE,
                Move(
                    "Ice Shard",
                    MoveType::PHYSICAL,
                    90,
                    95,
                    MoveEffect::NONE,
                    MoveTarget::ENEMY
                ),
                50
            );
        case 8:
            return new StatRelic(
                "Berserker's Axe",
                "Increases ATK by 20.",
                RelicRarity::RARE,
                StatType::ATK,
                20
            );
        
        case 9:
            return new StatRelic(
                "Guardian Shield",
                "Increases DEF by 20.",
                RelicRarity::RARE,
                StatType::DEF,
                20
            );

        case 10:
            return new UtilityRelic(
                "Healing Pendant",
                "Heals 20% HP when going up a floor.",
                RelicRarity::RARE,
                UtilityType::HEAL_ON_FLOOR,
                20
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
    int roll = rand() % 12;

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
        
        case 3:
            return new StatRelic(
                "Eternal Shield",
                "Increases DEF by 30.",
                RelicRarity::LEGENDARY,
                StatType::DEF,
                30
            );
        
        case 4:
            return new StatRelic(
                "Sage's Robe",
                "Increases SP.ATK by 30.",
                RelicRarity::LEGENDARY,
                StatType::SPATK,
                30
            );
        case 5: 
            return new UtilityRelic(
                "Timekeeper's Hourglass",
                "Increases EXP gained by 30%.",
                RelicRarity::LEGENDARY,
                UtilityType::EXP_BOOST,
                30
            );
        case 6:
            return new MoveRelic(
                "Earth Tome",
                "Adds Earthquake or boosts it if already owned.",
                RelicRarity::LEGENDARY,
                Move(
                    "Earthquake",
                    MoveType::PHYSICAL,
                    120,
                    85,
                    MoveEffect::NONE,
                    MoveTarget::ENEMY
                ),
                50
            );
        case 10:
            return new MoveRelic(
                "Wind Scroll",
                "Adds Wind Slash or boosts it if already owned.",
                RelicRarity::LEGENDARY,
                Move(
                    "Wind Slash",
                    MoveType::PHYSICAL,
                    100,
                    95,
                    MoveEffect::NONE,
                    MoveTarget::ENEMY
                ),
                50
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