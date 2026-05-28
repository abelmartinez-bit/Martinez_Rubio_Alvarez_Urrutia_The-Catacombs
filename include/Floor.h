#pragma once
#include "Door.h"
#include "Player.h"
#include "Enemy.h"
#include "Miniboss.h"
#include <vector>

class Floor {
private:
    int floorNumber;
    std::vector<Door> doors;

    int calculateCorruptionChance(float modifier) const;
    EnemyType getRandomEnemyType() const;
    void scaleEnemyStats(int& hp, int& atk, int& def, int& spAtk, int& spDef, int& spd) const;

public:
    Floor(int number);

    int getFloorNumber() const { return floorNumber; }
    std::vector<Door> getDoors() const { return doors; }
            
    // Player para leer los probability modifiers de sus relics
    void generateDoors(const Player& player);
    Entity* spawnEnemyFromDoor(const Door& door);
    std::string triggerEvent(Player& player, const Door& door);

     // Floor entrega las recompensas al jugador al terminar el combate
    std::string giveRewards(Player& player, Entity* enemy, bool isMiniBoss);
};