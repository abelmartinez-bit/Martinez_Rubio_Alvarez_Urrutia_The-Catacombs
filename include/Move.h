#pragma once
#include <string>


//enums para crear variables de tipo MoveType, MoveEffect y MoveTarget
enum class MoveType {
    PHYSICAL,   //  ATK vs DEF
    SPECIAL,    //  SP.ATK vs SP.DEF
    EFFECT      // efectos como curar, bloquear, subir/bajar stats, etc. No hace daño directo, power = 0
};

enum class MoveEffect {
    NONE,
    HEAL,
    BLOCK,
    ATK_UP,
    ATK_DOWN,
    DEF_UP,
    DEF_DOWN,
    SPATK_UP,
    SPATK_DOWN,
    SPDEF_UP,
    SPDEF_DOWN,
    SPD_UP,
    SPD_DOWN
};

enum class MoveTarget {
    SELF,    // heals, buffs, block
    ENEMY    // damage, debuffs
};

//Moviemiento, turno, o accion en combate 
class Move {
private:
    std::string name;
    MoveType type;
    MoveEffect effect;
    MoveTarget target;
    int power;      // daño base, 0 si es un efecto sin daño directo
    int accuracy;   // 0-100

public:
    //efecto default NONE y target default ENEMY
    Move(std::string name, MoveType type, int power, int accuracy, 
         MoveEffect effect = MoveEffect::NONE, 
         MoveTarget target = MoveTarget::ENEMY);
         
    // Getters
    std::string getName() const;
    MoveType getType() const;
    MoveEffect getEffect() const;
    MoveTarget getTarget() const;
    int getPower() const;
    int getAccuracy() const;

    void boostPower(int amount);
    void addAccuracy(int amount);
};