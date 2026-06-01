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
    // 
    Entity* firstActor = nullptr;
    Entity* secondActor = nullptr;

    Move firstMove = Move("Empty", MoveType::EFFECT, 0, 0);
    Move secondMove = Move("Empty", MoveType::EFFECT, 0, 0);

    bool pendingSecondMove = false;

    void executeSingleMove(Entity* attacker, Entity* defender, Move attackMove, Move defenseMove);
    void updateFinalBossPhaseIfNeeded();

    ///////

public:
    CombatEngine(Player* p, Entity* e);

    Entity* getEnemy() const { return enemy; }

    void executeTurn(int playerMoveIndex);
    ///
    void startTurn(int playerMoveIndex);
    void executePendingSecondMove();
    bool hasPendingSecondMove() const { return pendingSecondMove; }
    bool firstActorIsPlayer() const { return firstActor == player; }
    /// 
    int calculateDamage(Entity* attacker, Entity* defender, Move move);
    void applyMoveEffect(Move move, Entity* user, Entity* target);
    void usePotion(Potion potion, Entity& target);
    std::string potionList(const Player& player) const;
    std::string relicList(const Player* player) const;
    bool isBattleOver();
    std::string getWinner();
    std::string getCombatLog();
    void clearLog();
    void setBaseStats();
    void endReset(Entity* entity, Entity* entity2);
};