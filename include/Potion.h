#pragma once
#include <string>
#include "Move.h"

//objetos que el jugador puede usar durante el combate para efectos como curar o subir stats
class Potion {
private:
    std::string name;
    MoveEffect effect;
    MoveTarget target;
    int potency;  // 10 para debil, 20 para fuerte

public:
    Potion(std::string name, MoveEffect effect, MoveTarget target, int potency);

    std::string getName() const;
    MoveEffect getEffect() const;
    MoveTarget getTarget() const;
    int getPotency() const;
};