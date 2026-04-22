#include "../include/Player.h"
#include <iostream>

Player::Player(std::string name, int hp, int atk, int def, int spAtk, int spDef, int spd, PlayerType playerType)
    : Entity(name, hp, atk, def, spAtk, spDef, spd), type(playerType), level(1), exp(0), expToNextLevel(100), potionSlots(2) {}

// Getters
int Player::getLevel() const { return level; }
int Player::getExp() const { return exp; }
int Player::getExpToNextLevel() const { return expToNextLevel; }
int Player::getPotionSlots() const { return potionSlots; }
std::vector<Relic*> Player::getRelics() const { return relics; }
std::vector<Potion> Player::getPotions() const { return potions; }

void Player::gainExp(int amount) {
    exp += amount;
    if (exp >= expToNextLevel)
        levelUp();
}

void Player::levelUp() {
    level++;
    exp -= expToNextLevel;
    expToNextLevel = 100 * level; // scales w level

    // 5% heal al subir de nivel (base), mas lo que den los relics. Tal vez sea aqui o en 
    //un futuro GameEngine o algo asi
   int restore = maxHp * 0.05;
    heal(restore);

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
            std::cout << "Invalid choice = HP boost.\n";
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
}

void Player::addPotion(Potion potion) {
    if (potions.size() < potionSlots)
        potions.push_back(potion);
    else
        std::cout << "No potion slots available!\n";
}

void Player::usePotion(int index, Entity& target) {
    if (index >= 0 && index < potions.size()) {
        potions.erase(potions.begin() + index);
    } else {
        std::cout << "Invalid potion slot!\n";
    }
}

void Player::expandPotionSlots(int amount) {
    potionSlots += amount;
}

void Player::displayStats() const {
    Entity::displayStats();
    std::cout << "Level: " << level << " | EXP: " << exp << "/" << expToNextLevel << "\n";
    std::cout << "Relics: " << relics.size() << "\n";
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