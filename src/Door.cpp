#include "../include/Door.h"

Door::Door(int number, DoorType type, EnemyType enemyType)
    : doorNumber(number), type(type), enemyType(enemyType) {}

std::string Door::getDisplayName() const {
    std::string name = "Door " + std::to_string(doorNumber);
    
    if (type == DoorType::CORRUPT) {
        name += " [CORRUPT]";
    }
    // Nunca se muestra event, esto es aproposito
    
    return name;
}