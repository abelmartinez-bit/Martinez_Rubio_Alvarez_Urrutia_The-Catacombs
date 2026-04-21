// Para que se corra una vez y no se duplique al incluirlo en otros archivos
#pragma once
#include <string>
#include <vector>
#include "Move.h"

class Entity {
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

    int baseAtk, baseDef, baseSpAtk, baseSpDef, baseSpd;
    std::vector<Move> moves; // 4 moves por entidad

public:
    Entity(std::string name, int hp, int atk, int def, int spAtk, int spDef, int spd);

    // Getters
    std::string getName() const;
    int getHp() const;
    int getMaxHp() const;
    int getAtk() const;
    int getDef() const;
    int getSpAtk() const;
    int getSpDef() const;
    int getSpd() const;
    bool getIsBlocking() const;
    std::vector<Move> getMoves() const;

    // Setters
    void setMaxHp(int amount);
    void setAtk(int amount);
    void setDef(int amount);
    void setSpAtk(int amount);
    void setSpDef(int amount);
    void setSpd(int amount);

    void setBaseAtk(int amount);
    void setBaseDef(int amount);
    void setBaseSpAtk(int amount);
    void setBaseSpDef(int amount);
    void setBaseSpd(int amount);

    // Acciones por ahora
    void takeDamage(int amount);
    void heal(int amount);
    void setBlocking(bool blocking);
    void setMove(int index, Move move);
    bool isAlive() const;
    void resetStats(); // para resetear stats a base despues de cada combate, por ahora resetea todo a los base, pero se puede modificar para solo resetear los stats que fueron boosteados/debuffeados

    //virtual para que cada clase hija pueda mostrar sus stats/destruirse de forma diferente
    virtual void displayStats() const;
    virtual ~Entity() = default;
};