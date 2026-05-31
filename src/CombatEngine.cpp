#include "../include/CombatEngine.h"
#include <cstdlib> //rand
#include <ctime> //time for seeding rand
#include <iostream> // temp for testing

CombatEngine::CombatEngine(Player* p, Entity* e) : player(p), enemy(e) {
    srand(time(0)); // seed random for accuracy checks y despues para crit chance
    // set base stats of player and enemy at the start of combat
    setBaseStats();
    // asi se pueden resetear despues con resetStats() sin perder el valor original
}

void CombatEngine::executeTurn(int playerMoveIndex) {
    clearLog();
    
    int enemyMoveIndex = rand() % 4; // random move for enemy, se puede mejorar con IA mas adelante, por ahora es random

    // "->"" ya que tenemos entity como pointer, cant use "."
    Move playerMove = player->getMoves()[playerMoveIndex];
    Move enemyMove = enemy->getMoves()[enemyMoveIndex];
    
    // Turn order basado en SPD, si empate player va primero
    Entity* first = (player->getSpd() >= enemy->getSpd()) ? (Entity*)player : enemy;
    Entity* second = (first == player) ? enemy : (Entity*)player;
    Move firstMove = (first == player) ? playerMove : enemyMove;
    Move secondMove = (first == player) ? enemyMove : playerMove;
    
    // First entity moves (efectos no ocupan acc check)
    if (firstMove.getType() == MoveType::EFFECT && firstMove.getEffect() != MoveEffect::BLOCK) {
        Entity* target = (firstMove.getTarget() == MoveTarget::SELF) ? first : second;
        combatLog += first->getName() + " used " + firstMove.getName() + ", ";
        applyMoveEffect(firstMove, first, target);
        if (secondMove.getType() == MoveType::EFFECT && secondMove.getEffect() == MoveEffect::BLOCK) combatLog += second->getName() + " is blocking!\n"; 
        // para cuando bloquearon y no atacaron}
    } else {
        // Accuracy check
        int roll = rand() % 100; 
        if (secondMove.getEffect() == MoveEffect::BLOCK) applyMoveEffect(secondMove, second, first);
        // el bloque se aplica antes del daño para que el efecto de bloquear afecte el ataque del segundo movimiento
        if (roll < firstMove.getAccuracy()) { // si el segundo movimiento fue block, el primero no puede fallar porque el bloque ya se aplico antes del accuracy check

            if (firstMove.getEffect() != MoveEffect::BLOCK){
                int damage = calculateDamage(first, second, firstMove);
                second->takeDamage(damage);
                combatLog += first->getName() + " used " + firstMove.getName() + " for ";
                if (second->getIsBlocking()) combatLog += "a reduced ";
                combatLog += std::to_string(damage) + " damage!\n";

                if (firstMove.getEffect() != MoveEffect::NONE && firstMove.getEffect() != MoveEffect::BLOCK) {
                    combatLog +=  first->getName() + "'s " + firstMove.getName() + " had an additional effect, ";
                    applyMoveEffect(firstMove, first, second); // para movimientos de dmg con efectos secundarios
                }
            }
        } else combatLog += first->getName() + "'s " + firstMove.getName() + " missed!\n";
        
    }
    
    // Check if battle ended (died from first move so second doesnt get to move)
    if (isBattleOver()) {
        endReset(player, enemy);
        return;
    }
    
    // Second entity moves, same logic as first
    if (secondMove.getType() == MoveType::EFFECT && secondMove.getEffect() != MoveEffect::BLOCK) {
        Entity* target = (secondMove.getTarget() == MoveTarget::SELF) ? second : first;
        combatLog += second->getName() + " used " + secondMove.getName() + ", ";
        applyMoveEffect(secondMove, second, target);
        if (firstMove.getEffect() == MoveEffect::BLOCK) combatLog += first->getName() + " is blocking!\n"; 
        // para cuando bloquearon y no atacaron
    } else {
        int roll = rand() % 100;

        if (firstMove.getEffect() == MoveEffect::BLOCK) applyMoveEffect(firstMove, first, second); 
        // el bloque se aplica antes del daño para que el efecto de bloquear afecte el ataque del segundo movimiento

        if (roll < secondMove.getAccuracy()) { // si el primer movimiento fue block, el segundo no puede fallar porque el bloque ya se aplico antes del accuracy check

            if (secondMove.getEffect() != MoveEffect::BLOCK){ 
                int damage = calculateDamage(second, first, secondMove);
                first->takeDamage(damage);
                combatLog += second->getName() + " used " + secondMove.getName() + " for ";
                if (first->getIsBlocking()) combatLog += "a reduced ";
                combatLog += std::to_string(damage) + " damage!\n"; 
                
                if (secondMove.getEffect() != MoveEffect::NONE && secondMove.getEffect() != MoveEffect::BLOCK) {
                    combatLog +=  second->getName() + "'s " + secondMove.getName() + " had an additional effect; ";
                    applyMoveEffect(secondMove, second, first); // para movimientos de dmg con efectos secundarios
                }
            }

        } else combatLog += second->getName() + "'s " + secondMove.getName() + " missed!\n";
    
    }

    if (isBattleOver()) {
        endReset(player, enemy);
        return;
    }

    // Reset blocking state after turn
    if (first->getIsBlocking()) first->setBlocking(false);
    if (second->getIsBlocking()) second->setBlocking(false);

    // temp for testing
    std::cout << "---- End of Turn Stats ----\n";
    first->displayBaseStats();
    first->displayStats();
    std::cout << "------------------\n";
    second->displayBaseStats();
    second->displayStats();
    std::cout << "\n\n";
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
    int damage = static_cast<int>(((attackStat * move.getPower()) / defenseStat) / 5);
    
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
            user->setAtk(user->getAtk() + (user->getAtk() * potency / 100));
            combatLog += user->getName() + "'s ATK rose!\n";
            break;
        case MoveEffect::ATK_DOWN:
            target->setAtk(target->getAtk() - (target->getAtk() * potency / 100));
            combatLog += target->getName() + "'s ATK fell!\n";
            break;
        case MoveEffect::DEF_UP:
            user->setDef(user->getDef() + (user->getDef() * potency / 100));
            combatLog += user->getName() + "'s DEF rose!\n";
            break;
        case MoveEffect::DEF_DOWN:
            target->setDef(target->getDef() - (target->getDef() * potency / 100));
            combatLog += target->getName() + "'s DEF fell!\n";
            break;
        case MoveEffect::SPATK_UP:
            user->setSpAtk(user->getSpAtk() + (user->getSpAtk() * potency / 100));
            combatLog += user->getName() + "'s SP.ATK rose!\n";
            break;
        case MoveEffect::SPATK_DOWN:
            target->setSpAtk(target->getSpAtk() - (target->getSpAtk() * potency / 100));
            combatLog += target->getName() + "'s SP.ATK fell!\n";
            break;
        case MoveEffect::SPDEF_UP:
            user->setSpDef(user->getSpDef() + (user->getSpDef() * potency / 100));
            combatLog += user->getName() + "'s SP.DEF rose!\n";
            break;
        case MoveEffect::SPDEF_DOWN:
            target->setSpDef(target->getSpDef() - (target->getSpDef() * potency / 100));
            combatLog += target->getName() + "'s SP.DEF fell!\n";
            break;
        case MoveEffect::SPD_UP:
            user->setSpd(user->getSpd() + (user->getSpd() * potency / 100));
            combatLog += user->getName() + "'s SPD rose!\n";
            break;
        case MoveEffect::SPD_DOWN:
            target->setSpd(target->getSpd() - (target->getSpd() * potency / 100));
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
            combatLog += target.getName() + "'s SP.ATK rose!\n";
            break;
        case MoveEffect::SPATK_DOWN:
            combatLog += target.getName() + "'s SP.ATK fell!\n";
            break;
        case MoveEffect::SPDEF_UP:
            combatLog += target.getName() + "'s SP.DEF rose!\n";
            break;
        case MoveEffect::SPDEF_DOWN:
            combatLog += target.getName() + "'s SP.DEF fell!\n";
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

std::string CombatEngine::getCombatLog() {
    return combatLog;
}

void CombatEngine::clearLog() {
    combatLog = "";
}

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