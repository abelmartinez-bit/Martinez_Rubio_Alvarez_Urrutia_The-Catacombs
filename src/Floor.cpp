#include "../include/Floor.h"
#include "../include/RelicPool.h"
#include "../include/Potion.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

Floor::Floor(int number) : floorNumber(number) {}

int Floor::calculateCorruptionChance(float modifier) const {
    // escala la prob de corrupcion con floorNumber + relic
    return static_cast<int>(floorNumber * 10 + modifier);
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
            hp = 60; atk = 15; def = 14; spAtk = 5; spDef = 14; spd = 6;
            break;
        case EnemyType::SHADE:
            name = "Shade";
            hp = 40; atk = 20; def = 6; spAtk = 5; spDef = 6; spd = 20;
            break;
        case EnemyType::WRAITH:
            name = "Wraith";
            hp = 50; atk = 5; def = 10; spAtk = 20; spDef = 12; spd = 12;
            break;
        case EnemyType::GOLEM:
            name = "Golem";
            hp = 70; atk = 8; def = 22; spAtk = 5; spDef = 22; spd = 4;
            break;
        case EnemyType::SHAMAN:
            name = "Shaman";
            hp = 50; atk = 12; def = 12; spAtk = 14; spDef = 16; spd = 14;
            break;
        case EnemyType::SPECTER:
            name = "Specter";
            hp = 35; atk = 15; def = 6; spAtk = 22; spDef = 8; spd = 24;
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

void Floor::triggerEvent(Player& player, const Door& door) {
    int eventType = rand() % 100;
    
    std::cout << "You triggered an event behind the door...\n";
    std::cout << "You were blessed by the dungeon gods!\n";
    player.gainExp(120 * player.getLevel()); // subir de nivel al trigger un evento

    if (eventType < 40) {
        // Heal event (40% chance)
        int healAmount = player.getMaxHp() * 0.4;
        player.heal(healAmount);
        std::cout << "You found a healing shrine! Restored " << healAmount << " HP.\n";
    } else if (eventType < 70) {
        // Potion event (30% chance)
        player.addPotion(Potion::createRandomPotion());
        std::cout << "You found a potion!\n";
    } else {
        // Relic event (30% chance por default)
        player.addRelic(RelicPool::getRandomRelic());
        std::cout << "You found a relic!\n";
    }
}
void Floor::giveRewards(Player& player, Entity* enemy, bool isMiniBoss) {
    // --- EXP --
    // dynamic_cast para acceder a getExpReward(), que solo existe en Enemy
    Enemy* e = dynamic_cast<Enemy*>(enemy);
    if (e) {
        player.gainExp(e->getExpReward());
    }

    // Potion drop
    // 30% chance de drop
    if (rand() % 100 < 30) {
        player.addPotion(Potion::createRandomPotion());
        std::cout << "You now have " << player.getPotions().size() << " potions!\n";
    } 

    // Relic drop
    // La rareza del drop sube segun los modificadores del jugador.
    // Probabilidades base (igual que RelicPool::getRandomRelic):
    //   Comun 70% | Rara 25% | Legendaria 5%
    // Cada punto de modifier desplaza el umbral hacia raridades mayores.
    int roll = rand() % 100;

    if (isMiniBoss) {
        // Miniboss: Base values more generous: Rara 60%, Legendaria 20%, Comun 20%
        // minibossDropModifier sube la chance de Legendaria y Rara a costa de Comun
        float mod = player.getMinibossDropModifier();
        int legendaryThreshold = static_cast<int>(20 + mod);
        int rareThreshold      = static_cast<int>(80 + mod / 2);

        if (roll < legendaryThreshold) {
            std::cout << "You received a LEGENDARY relic!\n";
            player.addRelic(RelicPool::getRandomLegendaryRelic());
        } else if (roll < rareThreshold) {
            std::cout << "You received a RARE relic!\n";
            player.addRelic(RelicPool::getRandomRareRelic());
        } else {
            std::cout << "You received a COMMON relic.\n";
            player.addRelic(RelicPool::getRandomCommonRelic());
        }
    } else {
        // Enemigo normal
        // enemyDropModifier sube la chance de Rara por algo de Comun
        float mod = player.getEnemyDropModifier();
        int legendaryThreshold = static_cast<int>(5  + mod / 2);
        int rareThreshold = static_cast<int>(30 + mod);

        if (roll < legendaryThreshold) {
            std::cout << "You received a LEGENDARY relic!\n";
            player.addRelic(RelicPool::getRandomLegendaryRelic());
        } else if (roll < rareThreshold) {
            std::cout << "You received a RARE relic!\n";
            player.addRelic(RelicPool::getRandomRareRelic());
        } else {
            std::cout << "You received a COMMON relic.\n";
            player.addRelic(RelicPool::getRandomCommonRelic());
        }
    }

    player.displayStats(); // temp for testing

}