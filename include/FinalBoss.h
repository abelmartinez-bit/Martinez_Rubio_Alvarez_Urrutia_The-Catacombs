#pragma once
#include "Enemy.h"

// El jefe final del juego. Hereda de Enemy igual que MiniBoss, pero su pelea
// es distinta: tiene 3 FASES. Conforme le bajas la vida, cambia sus movimientos
// por unos mas fuertes y se enfurece (sube sus stats). Es el enemigo mas dificil.
class FinalBoss : public Enemy {
private:
    int currentPhase;       // 1, 2 o 3
    void setPhase1Moves();  // moves iniciales (fuertes)
    void setPhase2Moves();  // moves al 66% HP (mas fuertes + se cura)
    void setPhase3Moves();  // moves al 33% HP (devastadores)

public:
    FinalBoss(std::string name, EnemyType type, int hp, int atk, int def,
              int spAtk, int spDef, int spd);

    // Revisa el HP actual y cambia de fase si corresponde.
    // Debe llamarse cada turno desde CombatEngine.
    void updatePhase();

    int getCurrentPhase() const;
};