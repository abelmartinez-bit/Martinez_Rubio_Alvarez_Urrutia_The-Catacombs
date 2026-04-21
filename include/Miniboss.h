#pragma once
#include "Enemy.h"

class MiniBoss : public Enemy {
public:
    MiniBoss(std::string name, EnemyType type, int hp, int atk, int def,
             int spAtk, int spDef, int spd);

private:
    void setMiniBossMoves();
};