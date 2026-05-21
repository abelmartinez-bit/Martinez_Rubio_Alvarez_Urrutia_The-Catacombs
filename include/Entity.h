// Para que se corra una vez y no se duplique al incluirlo en otros archivos
#pragma once
#include <string>
#include <vector>
#include "Move.h" //para crear el vector de movimientos

//La clase base, tanto jugador y enemigo son un entity, tienen stats , movimiuentos y acciones
class Entity {
//protected para que solo las subclases tengan acceso, Enemy y Player
protected:
    std::string name;
    int hp;
    int maxHp;
    int atk;
    int def;
    int spAtk;
    int spDef;
    int spd;
    bool isBlocking;

    // base es solo para hacer copias de los normales durante combat
    int baseAtk, baseDef, baseSpAtk, baseSpDef, baseSpd;
    std::vector<Move> moves; // 4 moves por entidad

public:
    Entity(std::string name, int hp, int atk, int def, int spAtk, int spDef, int spd);

    
    // Getters
    std::string getName() const { return name; }
    int getHp() const { return hp; }
    int getMaxHp() const { return maxHp; }
    int getAtk() const { return atk; }
    int getDef() const { return def; }
    int getSpAtk() const { return spAtk; }
    int getSpDef() const { return spDef; }
    int getSpd() const { return spd; }

    int getHighestStat() const {
        int highest = atk;
        if (def > highest) highest = def;
        if (spAtk > highest) highest = spAtk;
        if (spDef > highest) highest = spDef;
        if (spd > highest) highest = spd;
        return highest;
    }

    int getBaseDef() const { return baseDef; }
    int getBaseSpDef() const { return baseSpDef; }
    bool getIsBlocking() const { return isBlocking; }
    std::vector<Move> getMoves() const { return moves; }

    //Setters
    void setMaxHp(int amount) { maxHp = amount; }
    void setAtk(int amount) { atk = amount; }
    void setDef(int amount) { def = amount; }
    void setSpAtk(int amount) { spAtk = amount; }
    void setSpDef(int amount) { spDef = amount; }
    void setSpd(int amount) { spd = amount; }
    void setBaseAtk(int amount) { baseAtk = amount; }
    void setBaseDef(int amount) { baseDef = amount; }
    void setBaseSpAtk(int amount) { baseSpAtk = amount; }
    void setBaseSpDef(int amount) { baseSpDef = amount; }
    void setBaseSpd(int amount) { baseSpd = amount; }

    //acciones por ahora
    void takeDamage(int amount);
    void heal(int amount);
    void setBlocking(bool blocking);
    void setMove(int index, Move move);
    bool isAlive() const;
    void resetStats(); // para resetear stats a base despues de cada combate, por ahora resetea todo a los base, pero se puede modificar para solo resetear los stats que fueron boosteados/debuffeados

    /*virtual para que cada clase hija pueda mostrar sus stats/destruirse de forma diferente
    o sea redefinir la funcion */
    virtual void displayStats() const;
    virtual ~Entity() = default;
};