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

// FASE 1 - ataques fuertes balanceados
void FinalBoss::setPhase1Moves() {
    setMove(0, Move("Abyssal Strike", MoveType::PHYSICAL, 110, 90));
    setMove(1, Move("Dark Nova", MoveType::SPECIAL, 105, 90));
    setMove(2, Move("Dread Aura", MoveType::EFFECT, 0, 100, MoveEffect::ATK_UP, MoveTarget::SELF, 50));
    setMove(3, Move("Crippling Curse", MoveType::EFFECT, 0, 95, MoveEffect::DEF_DOWN, MoveTarget::ENEMY, 50));
}

// FASE 2 - mas fuertes + se cura
void FinalBoss::setPhase2Moves() {
    setMove(0, Move("Void Cleaver", MoveType::PHYSICAL, 125, 90));
    setMove(1, Move("Soul Rend", MoveType::SPECIAL, 120, 90));
    setMove(2, Move("Dark Restoration", MoveType::EFFECT, 0, 100, MoveEffect::HEAL, MoveTarget::SELF, 20));
    setMove(3, Move("Terrifying Roar", MoveType::EFFECT, 0, 95, MoveEffect::ATK_DOWN, MoveTarget::ENEMY, 55));
}

// FASE 3 - devastadores, maximo poder
void FinalBoss::setPhase3Moves() {
    setMove(0, Move("Apocalypse", MoveType::PHYSICAL, 145, 85));
    setMove(1, Move("Annihilation Beam", MoveType::SPECIAL, 140, 90));
    setMove(2, Move("Final Judgement", MoveType::PHYSICAL, 160, 80));
    setMove(3, Move("Doom Spiral", MoveType::SPECIAL, 135, 95));
}