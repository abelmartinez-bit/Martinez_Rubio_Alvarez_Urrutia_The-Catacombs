#pragma once //que se incluya el archivo una sola vez
#include "Entity.h" //porque enemy hereda de Entity
#include <vector>

//opciones de enemigos
enum class EnemyType {
    SENTINEL, // melee, alta hp, baja spd
    SHADE,   // melee, baja hp, alta spd
    WRAITH,  // ranged, media hp, media spd
    GOLEM,   // tanque melee, muy alta hp, muy baja spd
    SHAMAN,  // versatil, depende de effects
    SPECTER  // ranged, baja hp, alta spd
};

//Herencia de Entity, cada enemigo tiene tipo, recompensa de exp y movimientos
class Enemy : public Entity { 
protected: //atributos aparte de los ya heredados de Entity
    EnemyType type;
    int expReward;
    std::vector<Move> movePool; // banco de movimientos de donde se seleccionan  

public:
    Enemy(std::string name, EnemyType type, int hp, int atk, int def, int spAtk, int spDef, int spd);

    EnemyType getType() const;
    int getExpReward() const;


    void initialize(); //para que los futuros mini-bosses o bosses no tengan movimientos randoms normales por default
    //segun el tipo de enemigo, se hara un vector de su tipo y se tomaran 4 de ellos en select
    void initializeMovePool();
    void selectMovesFromPool();
    int calculateExpReward();

    void displayStats() const override;
};