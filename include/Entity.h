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

    //acciones por ahora
    void takeDamage(int amount);
    void heal(int amount);
    void setBlocking(bool blocking);
    void setMove(int index, Move move);
    bool isAlive() const;

    /*virtual para que cada clase hija pueda mostrar sus stats/destruirse de forma diferente
    o sea redefinir la funcion */
    virtual void displayStats() const;
    virtual ~Entity() = default;
};