#include "../include/Enemy.h"
#include <iostream>
#include <cstdlib>

Enemy::Enemy(std::string name, EnemyType type, int hp, int atk, int def,
             int spAtk, int spDef, int spd)
    : Entity(name, hp, atk, def, spAtk, spDef, spd), type(type) {
    expReward = calculateExpReward();
}

// enemy presets

static const std::vector<Move> PRESET_AGGRESSIVE = {
    // Damage
    Move("Slam", MoveType::PHYSICAL, 90, 80),
    Move("Crush", MoveType::PHYSICAL, 80, 90),
    Move("Overhead Strike", MoveType::PHYSICAL, 75, 90),
    Move("Flurry", MoveType::PHYSICAL, 70, 95),
    Move("Eviscerate", MoveType::PHYSICAL, 85, 80, MoveEffect::DEF_DOWN, MoveTarget::ENEMY, 10),
    Move("Phase Strike", MoveType::SPECIAL, 95, 80, MoveEffect::SPD_DOWN, MoveTarget::ENEMY, 10),
    // Effect
    Move("Berserk", MoveType::EFFECT, 0, 100, MoveEffect::ATK_UP, MoveTarget::SELF),
    Move("Shadowstep", MoveType::EFFECT, 0, 100, MoveEffect::SPD_UP, MoveTarget::SELF),
    Move("Iron Will", MoveType::EFFECT, 0, 100, MoveEffect::BLOCK, MoveTarget::ENEMY),
};

static const std::vector<Move> PRESET_DEFENSIVE = {
    // Damage
    Move("Shield Bash", MoveType::PHYSICAL, 70, 95),
    Move("Stone Fist", MoveType::PHYSICAL, 70, 90),
    Move("Boulder Toss", MoveType::PHYSICAL, 85, 80),
    Move("Suppressing Shot", MoveType::SPECIAL, 65, 95, MoveEffect::ATK_DOWN, MoveTarget::ENEMY, 10),
    Move("Soul Drain", MoveType::SPECIAL, 60, 90, MoveEffect::HEAL, MoveTarget::ENEMY, 10),
    // Effect
    Move("Fortify", MoveType::EFFECT, 0, 100, MoveEffect::DEF_UP, MoveTarget::SELF),
    Move("Harden", MoveType::EFFECT, 0, 100, MoveEffect::DEF_UP, MoveTarget::SELF),
    Move("Iron Wall", MoveType::EFFECT, 0, 100, MoveEffect::BLOCK, MoveTarget::ENEMY),
};

static const std::vector<Move> PRESET_DEBUFFER = {
    // Damage
    Move("Quick Strike", MoveType::PHYSICAL, 50, 100, MoveEffect::SPD_UP, MoveTarget::ENEMY, 15),
    Move("Gutting Slash", MoveType::PHYSICAL, 75, 90, MoveEffect::DEF_DOWN, MoveTarget::ENEMY, 15),
    Move("Spirit Strike", MoveType::SPECIAL, 70, 90),
    Move("Soul Rend", MoveType::SPECIAL, 80, 85, MoveEffect::SPDEF_DOWN, MoveTarget::ENEMY, 10),
    Move("Draining Touch", MoveType::SPECIAL, 65, 90, MoveEffect::HEAL, MoveTarget::ENEMY, 15),
    // Effect
    Move("Cripple", MoveType::EFFECT, 0, 100, MoveEffect::SPD_DOWN, MoveTarget::ENEMY, 20),
    Move("Hex", MoveType::EFFECT, 0, 100, MoveEffect::ATK_DOWN, MoveTarget::ENEMY, 20),
    Move("Enfeeble", MoveType::EFFECT, 0, 100, MoveEffect::DEF_DOWN, MoveTarget::ENEMY, 20),
    Move("Wail", MoveType::EFFECT, 0, 100, MoveEffect::SPDEF_DOWN, MoveTarget::ENEMY, 20),
    Move("Mana Shield", MoveType::EFFECT, 0, 100, MoveEffect::BLOCK, MoveTarget::ENEMY, 20),
};

static const std::vector<Move> PRESET_BALANCED = {
    // Damage
    Move("Phantom Bolt", MoveType::SPECIAL, 90, 80),
    Move("Dark Pulse", MoveType::SPECIAL, 75, 95),
    Move("Flurry", MoveType::PHYSICAL, 70, 95),
    Move("Boulder Toss", MoveType::PHYSICAL, 85, 80),
    Move("Absorb", MoveType::PHYSICAL, 0, 100, MoveEffect::HEAL, MoveTarget::SELF),
    Move("Ritual Bolt", MoveType::SPECIAL, 75, 85, MoveEffect::SPDEF_DOWN, MoveTarget::ENEMY, 20),
    // Effect
    Move("Fortify", MoveType::EFFECT, 0, 100, MoveEffect::DEF_UP, MoveTarget::SELF),
    Move("Cripple", MoveType::EFFECT, 0, 100, MoveEffect::SPD_DOWN, MoveTarget::ENEMY, 20),
    Move("Phase Shield", MoveType::EFFECT, 0, 100, MoveEffect::BLOCK, MoveTarget::ENEMY, 20),
};

static const std::vector<Move> PRESET_BALANCED_PHYSICAL = {
    // Damage
    Move("Slam", MoveType::PHYSICAL, 90, 80),
    Move("Shield Bash", MoveType::PHYSICAL, 70, 95),
    Move("Boulder Toss", MoveType::PHYSICAL, 85, 80),
    Move("Overhead Strike", MoveType::PHYSICAL, 75, 90),
    Move("Crushing Advance", MoveType::PHYSICAL, 80, 85, MoveEffect::DEF_DOWN, MoveTarget::ENEMY, 20),
    Move("Gutting Slash", MoveType::PHYSICAL, 75, 90, MoveEffect::DEF_DOWN, MoveTarget::ENEMY, 20),
    // Effect
    Move("Fortify", MoveType::EFFECT, 0, 100, MoveEffect::DEF_UP, MoveTarget::SELF),
    Move("Berserk", MoveType::EFFECT, 0, 100, MoveEffect::ATK_UP, MoveTarget::SELF),
    Move("Cripple", MoveType::EFFECT, 0, 90, MoveEffect::SPD_DOWN, MoveTarget::ENEMY, 20),
    Move("Stone Wall", MoveType::EFFECT, 0, 100, MoveEffect::BLOCK, MoveTarget::ENEMY, 20),
};

static const std::vector<Move> PRESET_BALANCED_SPECIAL = {
    // Damage
    Move("Phantom Bolt", MoveType::SPECIAL, 90, 80),
    Move("Dark Pulse", MoveType::SPECIAL, 75, 95),
    Move("Void Bolt", MoveType::SPECIAL, 85, 90),
    Move("Ritual Bolt", MoveType::SPECIAL, 75, 85),
    Move("Soul Rend", MoveType::SPECIAL, 80, 85, MoveEffect::SPDEF_DOWN, MoveTarget::ENEMY, 20),
    Move("Draining Touch", MoveType::SPECIAL, 65, 90, MoveEffect::HEAL, MoveTarget::SELF, 20),
    // Effect
    Move("Shadowstep", MoveType::EFFECT, 0, 100, MoveEffect::SPD_UP, MoveTarget::SELF),
    Move("Wail", MoveType::EFFECT, 0, 90, MoveEffect::SPDEF_DOWN, MoveTarget::ENEMY, 20),
    Move("Weaken", MoveType::EFFECT, 0, 90, MoveEffect::SPATK_DOWN, MoveTarget::ENEMY, 20),
    Move("Mana Shield", MoveType::EFFECT, 0, 100, MoveEffect::BLOCK, MoveTarget::ENEMY, 20),
};

// enemy signature moves by type
static std::vector<Move> getTypeMoves(EnemyType type) {
    switch (type) {
        case EnemyType::SENTINEL: 
            return std::vector<Move>{
                    Move("Bulwark", MoveType::EFFECT, 0, 100, MoveEffect::DEF_UP, MoveTarget::SELF, 30),
                    Move("Crushing Advance", MoveType::PHYSICAL, 85, 85, MoveEffect::DEF_DOWN, MoveTarget::ENEMY) };

        case EnemyType::SHADE: 
            return std::vector<Move>{
                    Move("Hemorrhage", MoveType::PHYSICAL, 90, 80, MoveEffect::DEF_DOWN, MoveTarget::ENEMY, 30),
                    Move("Smoke Step", MoveType::EFFECT, 0, 100, MoveEffect::SPD_UP, MoveTarget::SELF, 30) };

        case EnemyType::WRAITH: 
            return std::vector<Move>{
                    Move("Soul Siphon", MoveType::SPECIAL, 85, 90, MoveEffect::HEAL, MoveTarget::ENEMY, 15),
                    Move("Wither", MoveType::EFFECT, 0, 90, MoveEffect::SPATK_DOWN, MoveTarget::ENEMY) };

        case EnemyType::GOLEM:
            return std::vector<Move>{
                    Move("Tectonic Slam", MoveType::PHYSICAL, 95, 75, MoveEffect::SPD_DOWN, MoveTarget::ENEMY),
                    Move("Stone Skin", MoveType::EFFECT, 0, 100, MoveEffect::DEF_UP, MoveTarget::SELF, 30),
                    Move("Stengthen Core", MoveType::EFFECT, 0, 100, MoveEffect::SPDEF_UP, MoveTarget::SELF, 30) };

        case EnemyType::SHAMAN: 
            return std::vector<Move>{
                    Move("Dark Empowerment", MoveType::SPECIAL, 20, 100, MoveEffect::SPATK_UP, MoveTarget::ENEMY),
                    Move("Cursed Bolt", MoveType::SPECIAL, 75, 90, MoveEffect::SPDEF_DOWN, MoveTarget::ENEMY) };

        case EnemyType::SPECTER:
            return std::vector<Move>{
                    Move("Spectral Curse", MoveType::EFFECT, 0, 100, MoveEffect::SPDEF_DOWN, MoveTarget::ENEMY),
                    Move("Void Surge", MoveType::SPECIAL, 100, 75, MoveEffect::SPATK_DOWN, MoveTarget::ENEMY, 10) };
        default:
            return {};
    }
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
    const std::vector<Move>* presetPools[] = {
        &PRESET_AGGRESSIVE,
        &PRESET_DEFENSIVE,
        &PRESET_DEBUFFER,
        &PRESET_BALANCED,
        &PRESET_BALANCED_PHYSICAL,
        &PRESET_BALANCED_SPECIAL
    };
    const std::vector<Move>& preset = *presetPools[rand() % 6];

    movePool.clear();
    int effectCount = 0;
    int dmgAdded = 0;

    for (const auto& move : preset) {
        if (move.getType() == MoveType::EFFECT) {
            if (effectCount < 2) { movePool.push_back(move); effectCount++; }
        } else {
            if (dmgAdded < 3) { movePool.push_back(move); dmgAdded++; }
        }
    }

    for (const auto& move : getTypeMoves(type))
        movePool.push_back(move);
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