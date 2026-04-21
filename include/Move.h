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

class Move {
private:
    std::string name;
    MoveType type;
    MoveEffect effect;
    MoveTarget target;
    int power;      // base damage, 0 si effect
    int accuracy;   // 0-100
    int effectPotency; // para efectos, determina la cantidad de curacion o el grado de buff/debuff

public:
    //default effect NONE y default target ENEMY, default effect potency 10%
    Move(std::string name, MoveType type, int power, int accuracy, 
         MoveEffect effect = MoveEffect::NONE, 
         MoveTarget target = MoveTarget::ENEMY,
         int effectPotency = 10); 
    
         
    int getEffectPotency() const;
    std::string getName() const;
    MoveType getType() const;
    MoveEffect getEffect() const;
    MoveTarget getTarget() const;
    int getPower() const;
    int getAccuracy() const;

    void boostPower(int amount);
    void addAccuracy(int amount);
};