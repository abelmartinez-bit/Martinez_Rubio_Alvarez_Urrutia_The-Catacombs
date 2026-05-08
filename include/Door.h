#pragma once
#include "Enemy.h"
#include <string>

enum class DoorType {
    NORMAL,
    CORRUPT,
    EVENT
};

class Door {
private:
    int doorNumber;        // 1, 2, or 3
    DoorType type;
    EnemyType enemyType;   //  ignorado para eventos, ya que no tendran enemies

public:
    Door(int number, DoorType type, EnemyType enemyType);

    int getDoorNumber() const { return doorNumber; }
    DoorType getType() const { return type; }
    EnemyType getEnemyType() const { return enemyType; }
    
    bool isCorrupt() const { return type == DoorType::CORRUPT; }
    bool isEvent() const { return type == DoorType::EVENT; }
    
    std::string getDisplayName() const;
};