//include header diferente porq ocupamos mandarlo fuera de src y dentro de include
#include "../include/Entity.h"
#include <iostream>

Entity::Entity(std::string name, int hp, int atk, int def, int spAtk, int spDef, int spd)
    : name(name), hp(hp), maxHp(hp),  atk(atk), baseAtk(atk), def(def), baseDef(def), spAtk(spAtk), baseSpAtk(spAtk),
      spDef(spDef), baseSpDef(spDef), spd(spd), baseSpd(spd), isBlocking(false), moves(4, Move("Empty", MoveType::EFFECT, 0, 0)) {}

void Entity::resetStats() {
    atk = baseAtk;
    def = baseDef;
    spAtk = baseSpAtk;
    spDef = baseSpDef;
    spd = baseSpd;
    isBlocking = false;
}

// Actions
void Entity::takeDamage(int amount) {
    hp -= amount;
    if (hp < 0) hp = 0;
}

void Entity::heal(int amount) {
    hp += amount;
    if (hp > maxHp) hp = maxHp;
}

void Entity::setBlocking(bool blocking) {
    isBlocking = blocking;
}

void Entity::setMove(int index, Move move) {
    if (index >= 0 && index < 4)
        moves[index] = move;
}

bool Entity::isAlive() const {
    return hp > 0;
}

void Entity::displayStats() const {
    std::cout << "Name: " << name << "\n"
              << "HP: " << hp << "/" << maxHp << "\n"
              << "ATK: " << atk << " | DEF: " << def << "\n"
              << "SP.ATK: " << spAtk << " | SP.DEF: " << spDef << "\n"
              << "SPD: " << spd << "\n";

    std::cout << "Moves:\n";
    for (const Move& move : moves) {
        std::cout << "- " << move.getName() << "\n";
    }
}