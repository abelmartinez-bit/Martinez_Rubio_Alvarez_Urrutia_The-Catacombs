#include "../include/FinalBoss.h"
#include <iostream>

FinalBoss::FinalBoss(std::string name, EnemyType type, int hp, int atk, int def, int spAtk, int spDef, int spd)
    : Enemy(name, type, hp, atk, def, spAtk, spDef, spd) {
    expReward = static_cast<int>(expReward * 3); // 3x exp, es el boss final
    currentPhase = 1;
    setPhase1Moves();
}

int FinalBoss::getCurrentPhase() const {
    return currentPhase;
}

// Se llama cada turno. Cambia de fase segun el % de HP restante.
void FinalBoss::updatePhase() {
    float hpRatio = static_cast<float>(getHp()) / getMaxHp();

    if (currentPhase == 1 && hpRatio <= 0.66f) {
        currentPhase = 2;
        std::cout << getName() << " enters PHASE 2! It grows enraged!\n";
        setAtk(getAtk() + 15);
        setBaseAtk(getAtk());
        setSpAtk(getSpAtk() + 15);
        setBaseSpAtk(getSpAtk());
        setPhase2Moves();
    }
    else if (currentPhase == 2 && hpRatio <= 0.33f) {
        currentPhase = 3;
        std::cout << getName() << " enters PHASE 3! Its final form unleashes!\n";
        setAtk(getAtk() + 20);
        setBaseAtk(getAtk());
        setSpAtk(getSpAtk() + 20);
        setBaseSpAtk(getSpAtk());
        setSpd(getSpd() + 15);
        setBaseSpd(getSpd());
        setPhase3Moves();
    }
}

// FASE 1 - moves fuertes balanceados
void FinalBoss::setPhase1Moves() {
    setMove(0, Move("Malevolent Hit", MoveType::PHYSICAL, 110, 90));
    setMove(1, Move("Wither Shield", MoveType::EFFECT, 0, 100, MoveEffect::BLOCK, MoveTarget::SELF));
    setMove(2, Move("Wand of Atonement", MoveType::EFFECT, 0, 100, MoveEffect::HEAL, MoveTarget::SELF, 20));
    setMove(3, Move("Gyrokinetic Wand", MoveType::EFFECT, 0, 100, MoveEffect::SPD_DOWN, MoveTarget::ENEMY, 40));
}

// FASE 2 - mas fuertes, buffs
void FinalBoss::setPhase2Moves() {
    setMove(0, Move("Void Cleaver", MoveType::PHYSICAL, 125, 90));
    setMove(1, Move("Soul Rend", MoveType::SPECIAL, 120, 90));
    setMove(2, Move("Dread Aura", MoveType::EFFECT, 0, 100, MoveEffect::ATK_UP, MoveTarget::SELF, 50));
    setMove(3, Move("Necromancer's Brooch", MoveType::EFFECT, 0, 100, MoveEffect::SPATK_UP, MoveTarget::SELF, 50));
}

// FASE 3 - devastadores, maximo poder
void FinalBoss::setPhase3Moves() {
    setMove(0, Move("Implosion", MoveType::PHYSICAL, 145, 85));
    setMove(1, Move("Wither Impact", MoveType::SPECIAL, 140, 90, MoveEffect::HEAL, MoveTarget::ENEMY, 5)); // daño + algo de curacion para hacerlo mas impredecible + referemcia
    setMove(2, Move("Instant Transmission", MoveType::EFFECT, 0, 100, MoveEffect::SPD_UP, MoveTarget::SELF, 50));
    setMove(3, Move("Terminal Doomsday", MoveType::SPECIAL, 135, 95));
}