#include "../include/CombatEngine.h"
#include "../include/FinalBoss.h"
#include <cstdlib> //rand   
#include <iostream> // temp for testing

CombatEngine::CombatEngine(Player* p, Entity* e) : player(p), enemy(e) {
    // set base stats of player and enemy at the start of combat
    setBaseStats();
    // asi se pueden resetear despues con resetStats() sin perder el valor original
}

void CombatEngine::executeTurn(int playerMoveIndex) {
    // Compatibilidad por si alguna parte vieja del codigo todavía llama executeTurn().
    // Ejecuta el turno completo sin delay.
    startTurn(playerMoveIndex);

    if (pendingSecondMove) {
        executePendingSecondMove();
    }
}

void CombatEngine::startTurn(int playerMoveIndex) {
    if (isBattleOver() || pendingSecondMove) return;

    clearLog();

    int enemyMoveIndex = rand() % 4;

    Move playerMove = player->getMoves()[playerMoveIndex];
    Move enemyMove = enemy->getMoves()[enemyMoveIndex];

    firstActor = (player->getSpd() >= enemy->getSpd()) ? static_cast<Entity*>(player) : enemy;
    secondActor = (firstActor == player) ? enemy : static_cast<Entity*>(player);

    firstMove = (firstActor == player) ? playerMove : enemyMove;
    secondMove = (firstActor == player) ? enemyMove : playerMove;

    executeSingleMove(firstActor, secondActor, firstMove, secondMove);
    updateFinalBossPhaseIfNeeded();

    if (isBattleOver()) {
        endReset(player, enemy);
        pendingSecondMove = false;
        return;
    }

    pendingSecondMove = true;
}

void CombatEngine::executePendingSecondMove() {
    if (!pendingSecondMove || isBattleOver()) return;

    executeSingleMove(secondActor, firstActor, secondMove, firstMove);
    updateFinalBossPhaseIfNeeded();

    pendingSecondMove = false;

    if (isBattleOver()) {
        endReset(player, enemy);
        return;
    }

    if (firstActor && firstActor->getIsBlocking()) {
        firstActor->setBlocking(false);
    }

    if (secondActor && secondActor->getIsBlocking()) {
        secondActor->setBlocking(false);
    }

    std::cout << "---- End of Turn Stats ----\n";
    firstActor->displayBaseStats();
    firstActor->displayStats();
    std::cout << "------------------\n";
    secondActor->displayBaseStats();
    secondActor->displayStats();
    std::cout << "\n\n";
}

void CombatEngine::executeSingleMove(Entity* attacker, Entity* defender, Move attackMove, Move defenseMove) {
    if (!attacker || !defender) return;

    if (attackMove.getType() == MoveType::EFFECT && attackMove.getEffect() != MoveEffect::BLOCK) {
        Entity* target = (attackMove.getTarget() == MoveTarget::SELF) ? attacker : defender;

        combatLog += attacker->getName() + " used " + attackMove.getName() + ", ";
        applyMoveEffect(attackMove, attacker, target);

        if (defenseMove.getType() == MoveType::EFFECT && defenseMove.getEffect() == MoveEffect::BLOCK) {
            combatLog += defender->getName() + " is blocking!\n";
        }
    } 
    else {
        int roll = rand() % 100;

        if (defenseMove.getEffect() == MoveEffect::BLOCK) {
            applyMoveEffect(defenseMove, defender, attacker);
        }

        if (roll < attackMove.getAccuracy()) {
            if (attackMove.getEffect() != MoveEffect::BLOCK) {
                int damage = calculateDamage(attacker, defender, attackMove);
                defender->takeDamage(damage);

                combatLog += attacker->getName() + " used " + attackMove.getName() + " for ";

                if (defender->getIsBlocking()) {
                    combatLog += "a reduced ";
                }

                combatLog += std::to_string(damage) + " damage!\n";

                if (attackMove.getEffect() != MoveEffect::NONE && attackMove.getEffect() != MoveEffect::BLOCK) {
                    combatLog += attacker->getName() + "'s " + attackMove.getName() + " had an additional effect; ";
                    applyMoveEffect(attackMove, attacker, defender);
                }
            }
        } 
        else {
            combatLog += attacker->getName() + "'s " + attackMove.getName() + " missed!\n";
        }
    }
}

void CombatEngine::updateFinalBossPhaseIfNeeded() {
    if (enemy->isAlive()) {
        if (FinalBoss* boss = dynamic_cast<FinalBoss*>(enemy)) {
            boss->updatePhase();
        }
    }
}   

int CombatEngine::calculateDamage(Entity* attacker, Entity* defender, Move move) {
    int attackStat = (move.getType() == MoveType::PHYSICAL) ? attacker->getAtk() : attacker->getSpAtk();
    int defenseStat = (move.getType() == MoveType::PHYSICAL) ? defender->getDef() : defender->getSpDef();

    int baseDefenseStat = (move.getType() == MoveType::PHYSICAL) ? defender->getBaseDef() : defender->getBaseSpDef();
    int baseAttackStat = (move.getType() == MoveType::PHYSICAL) ? attacker->getBaseAtk() : attacker->getBaseSpAtk();


    int critChance = 5 + (5 * attacker->getSpd() / defender->getSpd()); // base crit chance 5%, scales con SPD
    int roll = rand() % 100;

    if (roll < critChance) {

        if (baseDefenseStat < defenseStat) defenseStat = baseDefenseStat; // if defender buffed ignore buffs
        if (baseAttackStat > attackStat) attackStat = baseAttackStat; // if attacker debuffed ignore debuffs
        
        if (defender->getIsBlocking()) {
            combatLog += defender->getName() + "'s block broke because ";
            defenseStat = static_cast<int>(defenseStat * 2.4);
            defender->setBlocking(false); // el bloque se rompe si le pegan un crit
        }

        attackStat *= 2.4; // mismo mult que el block
        combatLog += attacker->getName() + " landed a critical hit!\n";
    }
    
    // Apply blocking bonus when not crit, static por int
    if (defender->getIsBlocking()) {
        defenseStat = static_cast<int>(defenseStat * 2.4);
        attackStat = static_cast<int>(attackStat * 0.9); // simula el debuff que le esta aplicando
        combatLog += attacker->getName() + "'s offense got debuffed!\n";
        attacker->setAtk(attacker->getAtk() - (attacker->getAtk() * 0.1)); // debuff de ataques para simular que el enemigo esta mas enfocado en defender que atacar
        attacker->setSpAtk(attacker->getSpAtk() - (attacker->getSpAtk() * 0.1)); // debuff de ataques para simular que el enemigo esta mas enfocado en defender que atacar
    }
    
    // Simple damage formula
    int damage = static_cast<int>(((attackStat * move.getPower()) / defenseStat) / 6);
    
    if (damage < 1) damage = 1; // minimum 1 damage
    
    return damage;
}

void CombatEngine::applyMoveEffect(Move move, Entity* user, Entity* target) {
    int potency = move.getEffectPotency();
    
    switch (move.getEffect()) {
        case MoveEffect::HEAL:
            user->heal(user->getMaxHp() * potency / 100);
            combatLog += user->getName() + " healed " + std::to_string(potency) + "% HP!\n";
            break;
        case MoveEffect::BLOCK:
            user->setBlocking(true);
            combatLog += user->getName() + " anticipated the attack and is blocking!\n";
            break;
        case MoveEffect::ATK_UP:
            user->setAtk(user->getAtk() + (user->getAtk() * potency / 150));
            combatLog += user->getName() + "'s ATK rose!\n";
            break;
        case MoveEffect::ATK_DOWN:
            target->setAtk(target->getAtk() - (target->getAtk() * potency / 150));
            combatLog += target->getName() + "'s ATK fell!\n";
            break;
        case MoveEffect::DEF_UP:
            user->setDef(user->getDef() + (user->getDef() * potency / 150));
            combatLog += user->getName() + "'s DEF rose!\n";
            break;
        case MoveEffect::DEF_DOWN:
            target->setDef(target->getDef() - (target->getDef() * potency / 150));
            combatLog += target->getName() + "'s DEF fell!\n";
            break;
        case MoveEffect::SPATK_UP:
            user->setSpAtk(user->getSpAtk() + (user->getSpAtk() * potency / 150));
            combatLog += user->getName() + "'s SP,ATK rose!\n";
            break;
        case MoveEffect::SPATK_DOWN:
            target->setSpAtk(target->getSpAtk() - (target->getSpAtk() * potency / 150));
            combatLog += target->getName() + "'s SP,ATK fell!\n";
            break;
        case MoveEffect::SPDEF_UP:
            user->setSpDef(user->getSpDef() + (user->getSpDef() * potency / 150));
            combatLog += user->getName() + "'s SP,DEF rose!\n";
            break;
        case MoveEffect::SPDEF_DOWN:
            target->setSpDef(target->getSpDef() - (target->getSpDef() * potency / 150));
            combatLog += target->getName() + "'s SP,DEF fell!\n";
            break;
        case MoveEffect::SPD_UP:
            user->setSpd(user->getSpd() + (user->getSpd() * potency / 150));
            combatLog += user->getName() + "'s SPD rose!\n";
            break;
        case MoveEffect::SPD_DOWN:
            target->setSpd(target->getSpd() - (target->getSpd() * potency / 150));
            combatLog += target->getName() + "'s SPD fell!\n";
            break;
        default:
            break;
    }
}

void CombatEngine::usePotion(Potion potion, Entity& target) {
    player->usePotion(0, target); // por ahora siempre usa la primera pocion, se puede mejorar para seleccionar cual usar (solo borra la usada)
    combatLog += "Used " + potion.getName() + " on " + target.getName() + ", ";

    switch(potion.getEffect()){
        case MoveEffect::HEAL:
            combatLog += target.getName() + " healed for " + std::to_string(potion.getPotency()) + "% HP!\n";
            break;
        case MoveEffect::ATK_UP:
            combatLog += target.getName() + "'s ATK rose!\n";
            break;
        case MoveEffect::ATK_DOWN:
            combatLog += target.getName() + "'s ATK fell!\n";
            break;
        case MoveEffect::DEF_UP:
            combatLog += target.getName() + "'s DEF rose!\n";
            break;
        case MoveEffect::DEF_DOWN:
            combatLog += target.getName() + "'s DEF fell!\n";
            break;
        case MoveEffect::SPATK_UP:
            combatLog += target.getName() + "'s SP,ATK rose!\n";
            break;
        case MoveEffect::SPATK_DOWN:
            combatLog += target.getName() + "'s SP,ATK fell!\n";
            break;
        case MoveEffect::SPDEF_UP:
            combatLog += target.getName() + "'s SP,DEF rose!\n";
            break;
        case MoveEffect::SPDEF_DOWN:
            combatLog += target.getName() + "'s SP,DEF fell!\n";
            break;
        case MoveEffect::SPD_UP:
            combatLog += target.getName() + "'s SPD rose!\n";
            break;
        case MoveEffect::SPD_DOWN:
            combatLog += target.getName() + "'s SPD fell!\n";
            break;
        default:
            break;
    } 
}

    std::string CombatEngine::potionList(const Player& player) const {
    std::vector<Potion> potions = player.getPotions();
    std::string potionList = "Potions: ";
    for (size_t i = 0; i < potions.size(); i++) {
        potionList += potions[i].getName();
        if (i < potions.size() - 1) potionList += ", ";
    }
    if (potions.empty()) potionList += "None";
    return potionList;
}

    std::string CombatEngine::relicList(const Player* player) const {
    std::vector<Relic*> relics = player->getRelics();
    std::string relicList = "Relics: ";
    for (size_t i = 0; i < relics.size(); i++) {
        relicList += relics[i]->getName();
        if (i < relics.size() - 1) relicList += ", ";
    }
    if (relics.empty()) relicList += "None";
    return relicList;
}

bool CombatEngine::isBattleOver() {
    return !player->isAlive() || !enemy->isAlive();
}

std::string CombatEngine::getWinner() {
    if (player->isAlive()) return player->getName();
    if (enemy->isAlive()) return enemy->getName();
    return "Draw";
}

std::string CombatEngine::getCombatLog() {return combatLog;}

void CombatEngine::clearLog() {combatLog = "";}

void CombatEngine::setBaseStats() {
    player->setBaseAtk(player->getAtk());
    player->setBaseDef(player->getDef());
    player->setBaseSpAtk(player->getSpAtk());
    player->setBaseSpDef(player->getSpDef());
    player->setBaseSpd(player->getSpd());

    enemy->setBaseAtk(enemy->getAtk());
    enemy->setBaseDef(enemy->getDef());
    enemy->setBaseSpAtk(enemy->getSpAtk());
    enemy->setBaseSpDef(enemy->getSpDef());
    enemy->setBaseSpd(enemy->getSpd());
}

void CombatEngine::endReset(Entity* entity, Entity* entity2) {
    if (entity) entity->resetStats();
    if (entity2) entity2->resetStats();
}