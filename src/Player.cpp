#include "../include/Player.h"
#include <iostream>

Player::Player(std::string name, int hp, int atk, int def, int spAtk, int spDef, int spd, PlayerType playerType)
    : Entity(name, hp, atk, def, spAtk, spDef, spd), type(playerType), level(1), exp(0), expToNextLevel(100), potionSlots(2), expBoostPercent(0), restoreOnLevelUpPercent(0), corruptDoorModifier(0.f), minibossDropModifier(0.f), enemyDropModifier(0.f) {}

class Potion; // Forward declaration to avoid circular include

// funcion modificada para aplicar el efecto de las reliquias que modifican la experiencia ganada y la curacion al subir de nivel
void Player::gainExp(int amount) {
    int boostedAmount = amount + (amount * expBoostPercent / 100);

    exp += boostedAmount;

    std::cout << name << " gained " << boostedAmount << " EXP";
    if (expBoostPercent > 0) {
        std::cout << " (" << expBoostPercent << "% bonus applied)";
    }
    std::cout << "!\n";

    while (exp >= expToNextLevel) {
        levelUp();
    }
}

void Player::levelUp() {
    level++;
    exp -= expToNextLevel;
    expToNextLevel = 100 * level; // scales w level

     // 5% heal base al subir de nivel + bonus de relics RESTORE_ON_LEVEL_UP
    int totalRestorePercent = 5 + restoreOnLevelUpPercent;
    int restore = maxHp * totalRestorePercent / 100;

    heal(restore);

    std::cout << "Restored " << restore << " HP on level up";
    if (restoreOnLevelUpPercent > 0) {
        std::cout << " (" << totalRestorePercent << "% total restore)";
    }
    std::cout << "!\n";


    std::cout << name << " leveled up to level " << level << "!\n";
    std::cout << "Choose 2 stats to boost:\n";
    std::cout << "1. HP (+10)\n";
    std::cout << "2. ATK (+5)\n";
    std::cout << "3. DEF (+5)\n";
    std::cout << "4. SP.ATK (+5)\n";
    std::cout << "5. SP.DEF (+5)\n";
    std::cout << "6. SPD (+3)\n";

    for (int i = 0; i < 2; i++) {
        int choice;
        std::cout << "Choice " << i + 1 << ": ";
        std::cin >> choice;
        if (!(choice >= 1 && choice <= 6)) {
            std::cout << "Invalid choice = default HP boost.\n";
            choice = 1; // default HP boost
        }
        boostStat(choice);
    }
}

void Player::boostStat(int statChoice) {
    switch (statChoice) {
        case 1: maxHp += 10; hp += 10; break;
        case 2: atk += 5; break;
        case 3: def += 5; break;
        case 4: spAtk += 5; break;
        case 5: spDef += 5; break;
        case 6: spd += 3; break;
        default: std::cout << "Invalid choice!\n"; break;
    }
}

void Player::addRelic(Relic* relic) {
    relics.push_back(relic);
    relic->applyEffect(*this);
    checkRelicSynergies(); // revisar sinergias cada vez que se agrega una reliquia
}
// funciones para sinergias     
bool Player::hasRelic(const std::string& relicName) const {
    for (Relic* relic : relics) {
        if (relic->getName() == relicName) {
            return true;
        }
    }
    return false;
}

bool Player::hasSynergy(const std::string& synergyName) const {
    for (const std::string& synergy : activeSynergies) {
        if (synergy == synergyName) {
            return true;
        }
    }
    return false;
}

void Player::checkRelicSynergies() {
    if (hasRelic("Dragon Heart") && hasRelic("Golden Ring") && !hasSynergy("Dragon Wisdom")) {
        setMaxHp(getMaxHp() + 20);
        setSpAtk(getSpAtk() + 10);
        activeSynergies.push_back("Dragon Wisdom");
        std::cout << "Synergy activated: Dragon Wisdom! Max HP +20 and SP.ATK +10.\n";
    }

    if (hasRelic("Ancient Blade") && hasRelic("Warrior Fang") && !hasSynergy("Warrior Legacy")) {
        setAtk(getAtk() + 10);
        activeSynergies.push_back("Warrior Legacy");
        std::cout << "Synergy activated: Warrior Legacy! ATK +10.\n";
    }

    if (hasRelic("Phoenix Feather") && hasRelic("Healing Charm") && !hasSynergy("Rebirth Blessing")) {
        addRestoreOnLevelUp(10);
        activeSynergies.push_back("Rebirth Blessing");
        std::cout << "Synergy activated: Rebirth Blessing! Extra level-up restore +10%.\n";
    }

    if (hasRelic("Sharp Eye") && hasRelic("Steady Hand") && !hasSynergy("Perfect Aim")) {
        std::vector<Move> moves = getMoves();

        for (int i = 0; i < 4; i++) {
            moves[i].addAccuracy(5);
            setMove(i, moves[i]);
        }

        activeSynergies.push_back("Perfect Aim");
        std::cout << "Synergy activated: Perfect Aim! All moves accuracy +5.\n";
    }
}

void Player::addPotion(Potion potion) {
    if (potions.size() < potionSlots)
        potions.push_back(potion);
    else
        std::cout << "Gained potion, but no potion slots available!\n";
}

// aplica el efecto de la pocion antes de ser eliminada.

void Player::usePotion(int index, Entity& target) {
    if (index >= 0 && index < potions.size()) {
        potions[index].applyEffect(target);
        potions.erase(potions.begin() + index);
    } else {
        std::cout << "Invalid potion slot!\n";
    }
}


void Player::expandPotionSlots(int amount) {
    potionSlots += amount;
}
// funciones para los efectos de las reliquias
void Player::addExpBoost(int percent) {
    expBoostPercent += percent;
}
// para reliquias que curan al subir de nivel, se suma el porcentaje a restoreOnLevelUpPercent, que se usa en levelUp para calcular la curacion total   
void Player::addRestoreOnLevelUp(int percent) {
    restoreOnLevelUpPercent += percent;
}

void Player::addCorruptDoorModifier(float amount) {
    corruptDoorModifier += amount;
}
void Player::addMinibossDropModifier(float amount) {
    minibossDropModifier += amount;
}
void Player::addEnemyDropModifier(float amount) {
    enemyDropModifier += amount;
}


// funcion modificada, aparte de decir cuantas relics hay, tambien dira la descripcion de las reliquias.
void Player::displayStats() const {
    Entity::displayStats();
    std::cout << "Level: " << level << " | EXP: " << exp << "/" << expToNextLevel << "\n";
    std::cout << "Relics: " << relics.size() << "\n";

        for (Relic* relic : relics) {
        std::cout << "- " << relic->getName() << ": " << relic->getDescription() << "\n";
    }
    std::cout << "Potions: " << potions.size() << "/" << potionSlots << "\n";
}

void Player::initializeMovePool() {
    switch (type) {
        case PlayerType::WARRIOR:
            movePool = {
                // Name, Type, Power, Accuracy, Effect, Target (los non effects no ocupan los ultimos 2 parametros)
                Move("Crush", MoveType::PHYSICAL, 80, 90),
                Move("Shield Bash", MoveType::PHYSICAL, 70, 95),
                Move("Fortify", MoveType::EFFECT, 0, 100, MoveEffect::DEF_UP, MoveTarget::SELF),
                Move("Slam", MoveType::PHYSICAL, 90, 80),
                Move("Iron Wall", MoveType::EFFECT, 0, 100, MoveEffect::BLOCK, MoveTarget::SELF),
                Move("Overhead Strike", MoveType::PHYSICAL, 75, 90),
                Move("Berserk", MoveType::EFFECT, 0, 100, MoveEffect::ATK_UP, MoveTarget::SELF)
            };
            break;
        case PlayerType::ROGUE:
            movePool = {
                Move("Quick Strike", MoveType::PHYSICAL, 60, 100),
                Move("Backstab", MoveType::PHYSICAL, 90, 75),
                Move("Stealth", MoveType::EFFECT, 0, 100, MoveEffect::SPD_UP, MoveTarget::SELF),
                Move("Stab", MoveType::PHYSICAL, 85, 80),
                Move("Cripple", MoveType::EFFECT, 0, 100, MoveEffect::SPD_DOWN, MoveTarget::ENEMY),
                Move("Flurry", MoveType::PHYSICAL, 70, 95),
                Move("Evasion", MoveType::EFFECT, 0, 100, MoveEffect::BLOCK, MoveTarget::SELF)
            };
            break;
        case PlayerType::MAGE:
            movePool = {
                Move("Soul Drain", MoveType::SPECIAL, 80, 90),
                Move("Curse", MoveType::EFFECT, 0, 100, MoveEffect::SPATK_DOWN, MoveTarget::ENEMY),
                Move("Thunderbolt", MoveType::SPECIAL, 90, 80),
                Move("Weaken", MoveType::EFFECT, 0, 100, MoveEffect::SPDEF_DOWN, MoveTarget::ENEMY),
                Move("Ethereal Blast", MoveType::SPECIAL, 95, 75),
                Move("Arcane Shot", MoveType::SPECIAL, 75, 95),
                Move("Mana Shield", MoveType::EFFECT, 0, 100, MoveEffect::BLOCK, MoveTarget::SELF),
            };
            break;
        case PlayerType::ARCHER:
            movePool = {
                Move("Cripple", MoveType::EFFECT, 0, 100, MoveEffect::ATK_DOWN, MoveTarget::ENEMY),
                Move("Clean Shot", MoveType::SPECIAL, 70, 100),
                Move("Trap", MoveType::EFFECT, 0, 100, MoveEffect::SPDEF_DOWN, MoveTarget::ENEMY),
                Move("Arrow Storm", MoveType::SPECIAL, 75, 85),
                Move("Weaken", MoveType::EFFECT, 0, 100, MoveEffect::SPATK_DOWN, MoveTarget::ENEMY),
                Move("Armored", MoveType::EFFECT, 0, 100, MoveEffect::DEF_UP, MoveTarget::SELF),
                Move("Piercing Shot", MoveType::SPECIAL, 85, 80)
            };
            break;
    }
}

 void Player::selectMovesFromPool() {

    std::vector<int> indices;


    // se crea un indice que a traves de un for por rango guarda numeros del 0 al tamaño del movePool(por ahora 5 para todos)
    //y se van agregando a indices hasta tener 4 numeros distintos, que seran los moves que se le asignen al enemigo
    while (indices.size() < 4) {
        int idx = rand() % movePool.size();
        bool already = false;
        for (int i : indices)
            if (i == idx) { already = true; break; }
        if (!already)
            indices.push_back(idx);
    }

    //se asignan los moves al enemigo segun los indices obtenidos
    for (int i = 0; i < 4; i++)
        setMove(i, movePool[indices[i]]);
}

Player::~Player() {
    // Limpiar todos los pointers de relics
    for (Relic* relic : relics) {
        delete relic;
    }
    relics.clear();
}