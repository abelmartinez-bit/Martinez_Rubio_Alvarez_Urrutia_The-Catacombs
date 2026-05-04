#pragma once
#include <string>
#include "Move.h"   //mismo enum class
#include "Entity.h" // para usar applyEffect en el uso de pociones
//objetos que el jugador puede usar durante el combate para efectos como curar o subir stats

class Potion {
private:
    std::string name;
    MoveEffect effect;
    MoveTarget target;
    int potency;  // 10 para debil, 20 para fuerte (maybe needs balancing)

public:
    Potion(std::string name, MoveEffect effect, MoveTarget target, int potency);


    std::string getName() const { return name; }
    MoveEffect getEffect() const { return effect; }
    MoveTarget getTarget() const { return target; }
    int getPotency() const { return potency; }

    void applyEffect(Entity& target);
};