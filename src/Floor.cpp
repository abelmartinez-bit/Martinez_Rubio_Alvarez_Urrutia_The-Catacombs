#include "../include/Floor.h"
#include "../include/RelicPool.h"
#include "../include/Potion.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

Floor::Floor(int number) : floorNumber(number) {}

int Floor::calculateCorruptionChance(float modifier) const {
    // escala la prob de corrupcion con floorNumber + relic
    return static_cast<int>(floorNumber * 8 + modifier);
}

EnemyType Floor::getRandomEnemyType() const {
    int random = rand() % 6;
    switch (random) {
        case 0: return EnemyType::SENTINEL;
        case 1: return EnemyType::SHADE;
        case 2: return EnemyType::WRAITH;
        case 3: return EnemyType::GOLEM;
        case 4: return EnemyType::SHAMAN;
        case 5: return EnemyType::SPECTER;
        default: return EnemyType::SENTINEL;
    }
}

void Floor::generateDoors(const Player& player) {
    // new doors
    doors.clear();
    
    for (int i = 1; i <= 3; i++) {
        EnemyType enemyType = getRandomEnemyType();
        DoorType doorType = DoorType::NORMAL;
        
        // Check event
        if (rand() % 100 < 10) {
            doorType = DoorType::EVENT;
        }
        // Check corruption if not event
        else if (rand() % 100 < calculateCorruptionChance(player.getCorruptDoorModifier())) {
            doorType = DoorType::CORRUPT;
        }
        // if they fail it stays normal

        doors.push_back(Door(i, doorType, enemyType));
    }
}

void Floor::scaleEnemyStats(int& hp, int& atk, int& def, int& spAtk, int& spDef, int& spd) const {
    // Scale stats based on floor number
    hp = static_cast<int>(hp * (.5 + floorNumber * 0.3));      
    atk = static_cast<int>(atk * (.5 + floorNumber * 0.3));
    def = static_cast<int>(def * (.5 + floorNumber * 0.3));
    spAtk = static_cast<int>(spAtk * (.5 + floorNumber * 0.3));
    spDef = static_cast<int>(spDef * (.5 + floorNumber * 0.3));
    spd = static_cast<int>(spd * (.5 + floorNumber * 0.15));     // Speed scales slower
    // Needs balancing
}

Entity* Floor::spawnEnemyFromDoor(const Door& door) {
    // Base stats para enemy types
    int hp, atk, def, spAtk, spDef, spd;
    std::string name;
    
    switch (door.getEnemyType()) {
        case EnemyType::SENTINEL:
            name = "Sentinel";
            hp = 60; atk = 15; def = 14; spAtk = 5; spDef = 14; spd = 12;
            break;
        case EnemyType::SHADE:
            name = "Shade";
            hp = 40; atk = 18; def = 10; spAtk = 8; spDef = 8; spd = 26;
            break;
        case EnemyType::WRAITH:
            name = "Wraith";
            hp = 50; atk = 5; def = 10; spAtk = 20; spDef = 12; spd = 16;
            break;
        case EnemyType::GOLEM:
            name = "Golem";
            hp = 70; atk = 8; def = 24; spAtk = 6; spDef = 22; spd = 8;
            break;
        case EnemyType::SHAMAN:
            name = "Shaman";
            hp = 50; atk = 12; def = 12; spAtk = 14; spDef = 16; spd = 18;
            break;
        case EnemyType::SPECTER:
            name = "Specter";
            hp = 35; atk = 15; def = 6; spAtk = 22; spDef = 8; spd = 30;
            break;
    }
    
    // Scale stats by floor
    scaleEnemyStats(hp, atk, def, spAtk, spDef, spd);
    
    // Spawn MiniBoss if corrupt, otherwise regular Enemy
    if (door.isCorrupt()) {
    name = "Corrupted " + name;
    // Boost mini-boss stats by 1.3x
    hp = static_cast<int>(hp * 1.3);
    atk = static_cast<int>(atk * 1.3);
    def = static_cast<int>(def * 1.3);
    spAtk = static_cast<int>(spAtk * 1.3);
    spDef = static_cast<int>(spDef * 1.3);
    spd = static_cast<int>(spd * 1.3);
    return new MiniBoss(name, door.getEnemyType(), hp, atk, def, spAtk, spDef, spd);
    } else {
        Enemy* enemy = new Enemy(name, door.getEnemyType(), hp, atk, def, spAtk, spDef, spd);
        enemy->initialize();
        return enemy;
    }
}

std::string Floor::triggerEvent(Player& player, const Door& door) {
    int eventType = rand() % 100;

    int expGained = 120 * player.getLevel();
    player.gainExp(expGained);

    std::string result = "The dungeon gods smile upon you.\n\n";

    if (eventType < 40) {
        int healAmount = static_cast<int>(player.getMaxHp() * 0.4);
        player.heal(healAmount);
        result += "Healing Shrine: You were restored for " + std::to_string(healAmount) + " HP!";
    } else if (eventType < 70) {
        Potion p = Potion::createRandomPotion();
        std::string potionName = p.getName();

        if (player.getPotions().size() < player.getPotionSlots()) {
            player.addPotion(p);
            result += "You found a " + potionName + " !";
        } else {
            result += "Found: " + p.getName() + ".\nBut your inventory is full, you left it behind.\n\n";
        }

        
    } else {
        Relic* r = RelicPool::getRandomRelic();
        std::string relicName = r->getName();
        std::string relicDesc = r->getDescription();
        player.addRelic(r);
        result += "Ancient relic found: " + relicName + "\n" + relicDesc;
    }

    result += "\n\nGained " + std::to_string(expGained) + " EXP!";
    return result;
}

std::string Floor::giveRewards(Player& player, Entity* enemy, bool isMiniBoss) {
    std::string result;

    // --- EXP ---
    Enemy* e = dynamic_cast<Enemy*>(enemy);
    int expGained = 0;
    if (e) {
        expGained = e->getExpReward();
        player.gainExp(expGained);
    }
    result += "+" + std::to_string(expGained) + " EXP gained!\n";
    result += "EXP: " + std::to_string(player.getExp()) + " / " + std::to_string(player.getExpToNextLevel()) + "\n\n";

    // --- Potion drop (30% chance) ---
    if (rand() % 100 < 30) {
        Potion p = Potion::createRandomPotion();
        if (player.getPotions().size() < (size_t)player.getPotionSlots()) {
            player.addPotion(p);
            result += "Found: " + p.getName() + "!\n\n";
        } else {
            result += "Found: " + p.getName() + ".\nBut your inventory is full, you left it behind.\n\n";
        }
    }

    // --- Relic drop ---
    int roll = rand() % 100;
    Relic* dropped = nullptr;
    std::string rarityLabel;

    if (isMiniBoss) {
        float mod = player.getMinibossDropModifier();
        int legendaryThreshold = static_cast<int>(20 + mod);
        int rareThreshold      = static_cast<int>(80 + mod / 2);

        if (roll < legendaryThreshold) {
            dropped = RelicPool::getRandomLegendaryRelic();
            rarityLabel = "[LEGENDARY] ";
        } else if (roll < rareThreshold) {
            dropped = RelicPool::getRandomRareRelic();
            rarityLabel = "[RARE] ";
        } else {
            dropped = RelicPool::getRandomCommonRelic();
            rarityLabel = "[COMMON] ";
        }
    } else {
        float mod = player.getEnemyDropModifier();
        int legendaryThreshold = static_cast<int>(5  + mod / 2);
        int rareThreshold      = static_cast<int>(30 + mod);

        if (roll < legendaryThreshold) {
            dropped = RelicPool::getRandomLegendaryRelic();
            rarityLabel = "[LEGENDARY] ";
        } else if (roll < rareThreshold) {
            dropped = RelicPool::getRandomRareRelic();
            rarityLabel = "[RARE] ";
        } else {
            dropped = RelicPool::getRandomCommonRelic();
            rarityLabel = "[COMMON] ";
        }
    }

    player.addRelic(dropped);
    result += "Relic acquired:\n" + rarityLabel + dropped->getName() + "\n" + dropped->getDescription();

    player.displayStats(); // temp for testing
    return result;
}