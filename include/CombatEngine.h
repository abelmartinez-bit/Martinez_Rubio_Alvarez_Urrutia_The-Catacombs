#pragma once
#include "Player.h"
#include "Enemy.h"
#include "Potion.h"
#include <string>


//NO FINAL, solo para probar el combate y ver que todo funcione con SFML
class CombatEngine {
private:
    Player* player;
    Entity* enemy;
    std::string combatLog;

public:
    CombatEngine(Player* p, Entity* e);

    void executeTurn(int playerMoveIndex);
    int calculateDamage(Entity* attacker, Entity* defender, Move move);
    void applyMoveEffect(Move move, Entity* user, Entity* target);
    bool isBattleOver();
    std::string getWinner();
    std::string getCombatLog();
    void clearLog();
    void setBaseStats();
    void endReset(Entity* entity, Entity* entity2);
};