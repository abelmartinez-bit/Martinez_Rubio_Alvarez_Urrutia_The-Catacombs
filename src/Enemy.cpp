#include "../include/Enemy.h"
#include <iostream>
#include <cstdlib>

Enemy::Enemy(std::string name, EnemyType type, int hp, int atk, int def,
             int spAtk, int spDef, int spd)
    : Entity(name, hp, atk, def, spAtk, spDef, spd), type(type) {
    expReward = calculateExpReward();
}



EnemyType Enemy::getType() const { return type; }
int Enemy::getExpReward() const { return expReward; }

int Enemy::calculateExpReward() {
    // HP and SPD weighted diff. than combat stats
    return (atk + def + spAtk + spDef) / 2 + (spd) + (maxHp / 10);
}

void Enemy::initialize() {
    initializeMovePool();
    selectMovesFromPool();
}

void Enemy::initializeMovePool() {
    switch (type) {
        case EnemyType::SENTINEL:
            movePool = {
                // Name, Type, Power, Accuracy, Effect, Target (los non effects no ocupan los ultimos 2 parametros)
                Move("Crush", MoveType::PHYSICAL, 80, 90),
                Move("Shield Bash", MoveType::PHYSICAL, 70, 95),
                Move("Fortify", MoveType::EFFECT, 0, 100, MoveEffect::DEF_UP, MoveTarget::SELF),
                Move("Slam", MoveType::PHYSICAL, 90, 80),
                Move("Iron Wall", MoveType::EFFECT, 0, 100, MoveEffect::BLOCK, MoveTarget::SELF),
                Move("Overhead Strike", MoveType::PHYSICAL, 75, 90),
                Move("Berserk", MoveType::EFFECT, 0, 100, MoveEffect::ATK_UP, MoveTarget::SELF)
            };
            break;
        case EnemyType::SHADE:
            movePool = {
                Move("Quick Strike", MoveType::PHYSICAL, 60, 100),
                Move("Backstab", MoveType::PHYSICAL, 90, 75),
                Move("Shadowstep", MoveType::EFFECT, 0, 100, MoveEffect::SPD_UP, MoveTarget::SELF),
                Move("Eviscerate", MoveType::PHYSICAL, 85, 80),
                Move("Cripple", MoveType::EFFECT, 0, 90, MoveEffect::SPD_DOWN, MoveTarget::ENEMY),
                Move("Flurry", MoveType::PHYSICAL, 70, 95),
                Move("Evasion", MoveType::EFFECT, 0, 100, MoveEffect::BLOCK, MoveTarget::SELF)
            };
            break;
        case EnemyType::WRAITH:
            movePool = {
                Move("Soul Drain", MoveType::SPECIAL, 80, 90),
                Move("Curse", MoveType::EFFECT, 0, 85, MoveEffect::SPATK_DOWN, MoveTarget::ENEMY),
                Move("Phantom Bolt", MoveType::SPECIAL, 90, 80),
                Move("Wail", MoveType::EFFECT, 0, 90, MoveEffect::SPDEF_DOWN, MoveTarget::ENEMY),
                Move("Ethereal Blast", MoveType::SPECIAL, 95, 75),
                Move("Dark Pulse", MoveType::SPECIAL, 75, 95),
                Move("Mana Shield", MoveType::EFFECT, 0, 100, MoveEffect::BLOCK, MoveTarget::SELF)
            };
            break;
        case EnemyType::GOLEM:
            movePool = {
                Move("Stone Fist", MoveType::PHYSICAL, 70, 90),
                Move("Harden", MoveType::EFFECT, 0, 100, MoveEffect::DEF_UP, MoveTarget::SELF),
                Move("Earthquake", MoveType::PHYSICAL, 95, 75),
                Move("Rock Shield", MoveType::EFFECT, 0, 100, MoveEffect::BLOCK, MoveTarget::SELF),
                Move("Petrify", MoveType::EFFECT, 0, 85, MoveEffect::SPD_DOWN, MoveTarget::ENEMY),
                Move("Boulder Toss", MoveType::PHYSICAL, 85, 80),
                Move("Stone Slam", MoveType::PHYSICAL, 80, 90)
            };
            break;
        case EnemyType::SHAMAN:
            movePool = {
                Move("Hex", MoveType::EFFECT, 0, 90, MoveEffect::ATK_DOWN, MoveTarget::ENEMY),
                Move("Spirit Strike", MoveType::SPECIAL, 70, 90),
                Move("Enfeeble", MoveType::EFFECT, 0, 85, MoveEffect::DEF_DOWN, MoveTarget::ENEMY),
                Move("Ritual Bolt", MoveType::SPECIAL, 75, 85),
                Move("Weaken", MoveType::EFFECT, 0, 90, MoveEffect::SPATK_DOWN, MoveTarget::ENEMY),
                Move("Dark Ritual", MoveType::EFFECT, 0, 100, MoveEffect::SPDEF_UP, MoveTarget::SELF),
                Move("Dark Empowerment", MoveType::EFFECT, 0, 100, MoveEffect::SPATK_UP, MoveTarget::SELF, 20),
            };
            break;
        case EnemyType::SPECTER:
            movePool = {
                Move("Void Bolt", MoveType::SPECIAL, 85, 90),
                Move("Phase Strike", MoveType::SPECIAL, 95, 80),
                Move("Incorporeal", MoveType::EFFECT, 0, 100, MoveEffect::SPD_UP, MoveTarget::SELF),
                Move("Soul Rend", MoveType::SPECIAL, 90, 85),
                Move("Spectral Slash", MoveType::SPECIAL, 80, 90),
                Move("Haunt", MoveType::EFFECT, 0, 90, MoveEffect::SPDEF_DOWN, MoveTarget::ENEMY),
                Move("Spectral Curse", MoveType::EFFECT, 0, 95, MoveEffect::SPDEF_DOWN, MoveTarget::ENEMY, 35),
            };
            break;
    }
}

void Enemy::selectMovesFromPool() {

    std::vector<int> indices;


    // se crea un indice que a traves de un for por rango guarda numeros del 0 al tamaño del movePool(por ahora 5 para todos)
    //y se van agregando a indices hasta tener 4 numeros distintos, que seran los moves que se le asignen al enemigo
    while (indices.size() < 4) {
        int idx = rand() % movePool.size();
        bool already = false;
        for (int i : indices)
            if (i == idx) { already = true; break; }
        if (!already)
            indices.push_back(idx);
    }

    //se asignan los moves al enemigo segun los indices obtenidos
    for (int i = 0; i < 4; i++)
        setMove(i, movePool[indices[i]]);
}

void Enemy::displayStats() const {
    Entity::displayStats();
    std::cout << "EXP Reward: " << expReward << "\n";
}