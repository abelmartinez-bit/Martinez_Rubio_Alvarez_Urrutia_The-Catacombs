#include "../include/Player.h"
#include "../include/RelicTracker.h"
#include <iostream>


Player::Player(std::string name, int hp, int atk, int def, int spAtk, int spDef, int spd, PlayerType playerType)
    : Entity(name, hp, atk, def, spAtk, spDef, spd), type(playerType), level(1), exp(0), expToNextLevel(100), 
            potionSlots(2), expBoostPercent(0), restoreOnLevelUpPercent(0), corruptDoorModifier(0.f), minibossDropModifier(0.f), 
            enemyDropModifier(0.f), pendingStatBoosts(0), hasPendingMove(false), pendingMove("Empty", MoveType::EFFECT, 0, 0) {}

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
    expToNextLevel = 120 * level; // scales w level

     // 10% heal base al subir de nivel + bonus de relics RESTORE_ON_LEVEL_UP
    int totalRestorePercent = 10 + restoreOnLevelUpPercent;
    int restore = maxHp * totalRestorePercent / 100;
    heal(restore); 

    std::cout << "Restored " << restore << " HP on level up";
    if (restoreOnLevelUpPercent > 0) 
        std::cout << " (" << totalRestorePercent << "% total restore)";
    std::cout << "!\n";
    std::cout << name << " leveled up to level " << level << "!\n";
   
    //ahora sin cin, se marcan 2 boosts pendientes y el jugador elige después en la pantalla de level up
    pendingStatBoosts = 2;
}

void Player::boostStat(int statChoice) {
    switch (statChoice) {
        case 1: maxHp += 30; hp += 20; break;
        case 2: atk += 15; break;
        case 3: def += 15; break;
        case 4: spAtk += 15; break;
        case 5: spDef += 15; break;
        case 6: spd += 9; break;
        default: std::cout << "Invalid choice!\n"; break;
    }
}

//boosteo de stat sin cin
//funcion nueva a boostStat para validar, aplicarlo y restar la variable pendingStatBoosts
void Player::applyStatBoost(int choice) {
    if (pendingStatBoosts <= 0) return; 
    boostStat(choice); 
    pendingStatBoosts--;
}

//para aplicar el move de MoveRelic sin cin
void Player::applyPendingMove(int slotIndex) {
    if (!hasPendingMove) return; //validaciones
    if (slotIndex >= 0 && slotIndex < 4)
        setMove(slotIndex, pendingMove); 
    else
        setMove(0, pendingMove); //se aplica al primer slot si la elección no es válida
    std::cout << "Move replaced with " << pendingMove.getName() << "!\n";
    hasPendingMove = false; //resetear la variable
}

std::string Player::addRelic(Relic* relic) {
    relics.push_back(relic);
    relic->applyEffect(*this);
    RelicTracker::unlockRelic(relic->getName(), relic->getDescription()); // se desbloquea en el tracker
   
   return checkRelicSynergies(); // revisar  sinergias cada vez que se agrega una reliquia
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

std::string Player::checkRelicSynergies() {
    std::string synergyMessages = "";
    if (hasRelic("Dragon Heart") && hasRelic("Golden Ring") && !hasSynergy("Dragon Wisdom")) {
        setMaxHp(getMaxHp() + 20);
        setSpAtk(getSpAtk() + 10);
        activeSynergies.push_back("Dragon Wisdom");
        
        synergyMessages += "Synergy activated: Dragon Wisdom! \nMax HP +20 and SP.ATK +10.\n";
    }

    if (hasRelic("Ancient Blade") && hasRelic("Warrior Fang") && !hasSynergy("Warrior Legacy")) {
        setAtk(getAtk() + 10);
        activeSynergies.push_back("Warrior Legacy");

        synergyMessages += "Synergy activated: \nWarrior Legacy! ATK +10.\n";
    }

    if (hasRelic("Phoenix Feather") && hasRelic("Healing Charm") && !hasSynergy("Rebirth Blessing")) {
        addRestoreOnLevelUp(10);
        activeSynergies.push_back("Rebirth Blessing");

        synergyMessages += "Synergy activated: \nRebirth Blessing! Extra level-up restore +10%.\n";
    }

    if (hasRelic("Sharp Eye") && hasRelic("Steady Hand") && !hasSynergy("Perfect Aim")) {
        std::vector<Move> moves = getMoves();

        for (int i = 0; i < 4; i++) {
            moves[i].addAccuracy(5);
            setMove(i, moves[i]);
        }

        activeSynergies.push_back("Perfect Aim");

        synergyMessages += "Synergy activated: \nPerfect Aim! All moves accuracy +5.\n";
    }

    if (hasRelic("Stone Shield") && hasRelic("Iron Helmet") && !hasSynergy("Iron Defense")) {
        setDef(getDef() + 8);
        activeSynergies.push_back("Iron Defense");

        synergyMessages += "Synergy activated: \nIron Defense! DEF +8.\n";
    }

    if (hasRelic("Cloth Mantle") && hasRelic("Silver Thread") && !hasSynergy("Mystic Protection")) {
        setSpDef(getSpDef() + 8);
        activeSynergies.push_back("Mystic Protection");

        synergyMessages += "Synergy activated: \nMystic Protection! SP.DEF +8.\n";
    }

    if (hasRelic("Traveler Boots") && hasRelic("Light Sandals") && !hasSynergy("Swift Step")) {
        setSpd(getSpd() + 5);
        activeSynergies.push_back("Swift Step");

        synergyMessages += "Synergy activated: \nSwift Step! SPD +5.\n";
    }

    if (hasRelic("Heart Stone") && hasRelic("Old Medal") && !hasSynergy("Vital Core")) {
        setMaxHp(getMaxHp() + 15);
        heal(15);
        activeSynergies.push_back("Vital Core");

        synergyMessages += "Synergy activated: \nVital Core! Max HP +15 and healed 15 HP.\n";
    }

    if (hasRelic("Potion Belt") && hasRelic("Large Potion Belt") && !hasSynergy("Alchemist Pack")) {
        expandPotionSlots(1);
        activeSynergies.push_back("Alchemist Pack");

        synergyMessages += "Synergy activated: \nAlchemist Pack! Extra potion slot +1.\n";
    }

    if (hasRelic("Minor Scholar Coin") && hasRelic("Scholar Coin") && !hasSynergy("Scholar Fortune")) {
        addExpBoost(10);
        activeSynergies.push_back("Scholar Fortune");

        synergyMessages += "Synergy activated: \nScholar Fortune! Extra EXP boost +10%.\n";
    }

    if (hasRelic("Small Ruby") && hasRelic("Ancient Blade") && !hasSynergy("Crimson Edge")) {
        setAtk(getAtk() + 8);
        activeSynergies.push_back("Crimson Edge");

        synergyMessages += "Synergy activated: \nCrimson Edge! ATK +8.\n";
    }
    if (hasRelic("Blue Crystal") && hasRelic("Golden Ring") && !hasSynergy("Arcane Focus")) {
        setSpAtk(getSpAtk() + 8);
        activeSynergies.push_back("Arcane Focus");

        synergyMessages += "Synergy activated: \nArcane Focus! SP.ATK +8.\n";
    }

    if (hasRelic("Knight Armor") && hasRelic("Mystic Cape") && !hasSynergy("Guardian Balance")) {
        setDef(getDef() + 6);
        setSpDef(getSpDef() + 6);
        activeSynergies.push_back("Guardian Balance");

        synergyMessages += "Synergy activated: \nGuardian Balance! DEF +6 and SP.DEF +6.\n";
    }

    if (hasRelic("Hunter Boots") && hasRelic("Sharp Eye") && !hasSynergy("Predator Instinct")) {
        setSpd(getSpd() + 4);

        std::vector<Move> moves = getMoves();

        for (int i = 0; i < 4; i++) {
            moves[i].addAccuracy(3);
            setMove(i, moves[i]);
        }

        activeSynergies.push_back("Predator Instinct");

        synergyMessages += "Synergy activated: \nPredator Instinct! SPD +4 and all moves accuracy +3.\n";
    }

    if (hasRelic("Iron Knuckle") && hasRelic("Berserker's Axe") && !hasSynergy("Berserker Grip")) {
        setAtk(getAtk() + 12);
        activeSynergies.push_back("Berserker Grip");

        synergyMessages += "Synergy activated: \nBerserker Grip! ATK +12.\n";
    }

    if (hasRelic("Stone Guard") && hasRelic("Guardian Shield") && !hasSynergy("Stonewall Guard")) {
    setDef(getDef() + 12);
    activeSynergies.push_back("Stonewall Guard");

    synergyMessages += "Synergy activated: \nStonewall Guard! DEF +12.\n";
    }

    if (hasRelic("Sage's Amulet") && hasRelic("Sage's Robe") && !hasSynergy("Sage's Awakening")) {
    setSpAtk(getSpAtk() + 15);
    setSpDef(getSpDef() + 8);
    activeSynergies.push_back("Sage's Awakening");

    synergyMessages += "Synergy activated: \nSage's Awakening! SP.ATK +15 and SP.DEF +8.\n";
    }

    if (hasRelic("Healing Talisman") && hasRelic("Healing Pendant") && !hasSynergy("Blessed Recovery")) {
    addRestoreOnLevelUp(10);
    heal(20);
    activeSynergies.push_back("Blessed Recovery");

    synergyMessages += "Synergy activated: \nBlessed Recovery! Extra level-up restore +10% and healed 20 HP.\n";
    }

    if (hasRelic("Apprentice's Coin") && hasRelic("Scholar Coin") && !hasSynergy("Scholar's Path")) {
    addExpBoost(10);
    activeSynergies.push_back("Scholar's Path");

    synergyMessages += "Synergy activated: \nScholar's Path! Extra EXP boost +10%.\n";
    }

    if (hasRelic("Scholar Coin") && hasRelic("Timekeeper's Hourglass") && !hasSynergy("Accelerated Learning")) {
    addExpBoost(15);
    activeSynergies.push_back("Accelerated Learning");

    synergyMessages += "Synergy activated: \nAccelerated Learning! Extra EXP boost +15%.\n";
    }

    if (hasRelic("Ice Scroll") && hasRelic("Mystic Cape") && !hasSynergy("Frozen Veil")) {
    setSpDef(getSpDef() + 10);
    setSpAtk(getSpAtk() + 6);
    activeSynergies.push_back("Frozen Veil");

    synergyMessages += "Synergy activated: \nFrozen Veil! SP.DEF +10 and SP.ATK +6.\n";
    }

    if (hasRelic("Flame Scroll") && hasRelic("Berserker's Axe") && !hasSynergy("Flame Rage")) {
    setAtk(getAtk() + 8);
    setSpAtk(getSpAtk() + 8);
    activeSynergies.push_back("Flame Rage");

    synergyMessages += "Synergy activated: \nFlame Rage! ATK +8 and SP.ATK +8.\n";
    }

    if (hasRelic("Earth Tome") && hasRelic("Eternal Shield") && !hasSynergy("Earthbound Guardian")) {
    setDef(getDef() + 15);
    setMaxHp(getMaxHp() + 20);
    heal(20);
    activeSynergies.push_back("Earthbound Guardian");

    synergyMessages += "Synergy activated: \nEarthbound Guardian! DEF +15, Max HP +20 and healed 20 HP.\n";
    }

    if (hasRelic("Wind Scroll") && hasRelic("Hunter Boots") && !hasSynergy("Storm Hunter")) {
    setSpd(getSpd() + 8);
    setAtk(getAtk() + 6);
    activeSynergies.push_back("Storm Hunter");

    synergyMessages += "Synergy activated: \nStorm Hunter! SPD +8 and ATK +6.\n";
    }

    if (hasRelic("Thunder Tome") && hasRelic("Sharp Eye") && !hasSynergy("Thunder Precision")) {
    std::vector<Move> moves = getMoves();

    for (int i = 0; i < 4; i++) {
        moves[i].addAccuracy(4);
        setMove(i, moves[i]);
    }

    setSpAtk(getSpAtk() + 6);
    activeSynergies.push_back("Thunder Precision");

    synergyMessages += "Synergy activated: \nThunder Precision! SP.ATK +6 and all moves accuracy +4.\n";
    }

    if (hasRelic("Ancient Blade") && hasRelic("Berserker's Axe") && !hasSynergy("Ancient Rage")) {
    setAtk(getAtk() + 18);
    activeSynergies.push_back("Ancient Rage");

    synergyMessages += "Synergy activated: \nAncient Rage! ATK +18.\n";
    }

    if (hasRelic("Dragon Heart") && hasRelic("Healing Pendant") && !hasSynergy("Dragon Recovery")) {
    setMaxHp(getMaxHp() + 20);
    heal(30);
    activeSynergies.push_back("Dragon Recovery");

    synergyMessages += "Synergy activated: \nDragon Recovery! Max HP +20 and healed 30 HP.\n";
    }

    if (hasRelic("Eternal Shield") && hasRelic("Guardian Shield") && !hasSynergy("Eternal Guardian")) {
    setDef(getDef() + 15);
    setSpDef(getSpDef() + 8);
    activeSynergies.push_back("Eternal Guardian");

    synergyMessages += "Synergy activated: \nEternal Guardian! DEF +15 and SP.DEF +8.\n";
    }

    if (hasRelic("Potion Belt") && hasRelic("Healing Talisman") && !hasSynergy("Field Medic")) {
    expandPotionSlots(1);
    addRestoreOnLevelUp(5);
    activeSynergies.push_back("Field Medic");

    synergyMessages += "Synergy activated: \nField Medic! Potion slots +1 and extra level-up restore +5%.\n";
    }

    return synergyMessages; // se devuelve el mensaje con las sinergias activadas para mostrarlo en el log del combate

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

    std::cout << "Potions: " << potions.size() << "/" << potionSlots << "\n\n";
}

void Player::initializeMovePool() {

    int preset = rand() % 3; // 3 movePool presets por personaje, for better balancing
    
    // despues podemos agregarle mas de 4 moves por preset

    switch (type) {
        case PlayerType::WARRIOR:
        switch(preset) {
            case 0:
                movePool = {
                // Name, Type, Power, Accuracy, Effect, Target (los non effects no ocupan los ultimos 2 [o 3] parametros)
                Move("Crush", MoveType::PHYSICAL, 90, 90, MoveEffect::DEF_DOWN, MoveTarget::ENEMY, 10),
                Move("Slam", MoveType::PHYSICAL, 100, 80),
                Move("Iron Wall", MoveType::EFFECT, 0, 100, MoveEffect::BLOCK, MoveTarget::ENEMY),
                Move("Berserk", MoveType::EFFECT, 0, 100, MoveEffect::ATK_UP, MoveTarget::SELF, 30)
                };
                break;
            case 1:
                movePool = {
                Move("Shield Bash", MoveType::PHYSICAL, 60, 100, MoveEffect::SPD_DOWN, MoveTarget::ENEMY, 20),
                Move("Fortify", MoveType::EFFECT, 0, 100, MoveEffect::DEF_UP, MoveTarget::SELF, 35),
                Move("Magic Armor", MoveType::EFFECT, 0, 100, MoveEffect::SPDEF_UP, MoveTarget::SELF),
                Move("Overhead Strike", MoveType::PHYSICAL, 90, 100)
                };
                break;
            case 2:
                movePool = {
                Move("Shield Block", MoveType::EFFECT, 0, 100, MoveEffect::BLOCK, MoveTarget::ENEMY),
                Move("Shield Throw", MoveType::SPECIAL, 130, 90, MoveEffect::SPD_UP, MoveTarget::SELF, 20),
                Move("Taunt", MoveType::EFFECT, 0, 100, MoveEffect::DEF_DOWN, MoveTarget::ENEMY),
                Move("Leech Strike", MoveType::PHYSICAL, 100, 60, MoveEffect::HEAL, MoveTarget::SELF, 20)
                };
            };
            break;

        case PlayerType::ROGUE:
        switch(preset) {
            case 0:
                movePool = {
                Move("Backstab", MoveType::PHYSICAL, 100, 85),
                Move("Shank", MoveType::PHYSICAL, 75, 100, MoveEffect::SPD_DOWN, MoveTarget::ENEMY, 15),
                Move("Smoke Bomb", MoveType::EFFECT, 0, 100, MoveEffect::BLOCK, MoveTarget::ENEMY),
                Move("Adrenaline Rush", MoveType::EFFECT, 0, 100, MoveEffect::ATK_UP, MoveTarget::SELF, 40)
                };
                break;
            case 1:
                movePool = {
                Move("Poison Dagger", MoveType::PHYSICAL, 60, 95, MoveEffect::ATK_DOWN, MoveTarget::ENEMY),
                Move("Exploit Weakness", MoveType::PHYSICAL, 80, 90, MoveEffect::DEF_DOWN, MoveTarget::ENEMY),
                Move("Quickstep", MoveType::EFFECT, 0, 100, MoveEffect::SPD_UP, MoveTarget::SELF, 45),
                Move("Eviscerate", MoveType::PHYSICAL, 100, 85)
                };
                break;
            case 2:
                movePool = {
                Move("Lethal Strike", MoveType::PHYSICAL, 120, 80),
                Move("Distract", MoveType::EFFECT, 0, 100, MoveEffect::SPATK_DOWN, MoveTarget::ENEMY, 30),
                Move("Vanish", MoveType::EFFECT, 0, 100, MoveEffect::BLOCK, MoveTarget::ENEMY),
                Move("Garrote", MoveType::PHYSICAL, 70, 100, MoveEffect::DEF_DOWN, MoveTarget::ENEMY)
                };
            };
            break;
            
        case PlayerType::MAGE:
        switch(preset) {
            case 0:
                movePool = {
                Move("Soul Drain", MoveType::SPECIAL, 80, 70, MoveEffect::HEAL, MoveTarget::ENEMY, 15),
                Move("Curse", MoveType::EFFECT, 0, 100, MoveEffect::SPATK_DOWN, MoveTarget::ENEMY, 30),
                Move("Thunderbolt", MoveType::SPECIAL, 90, 80, MoveEffect::SPD_DOWN, MoveTarget::ENEMY),
                Move("Ethereal Blast", MoveType::SPECIAL, 120, 75),
                Move("Arcane Shot", MoveType::SPECIAL, 85, 100)
                };
                break;
            case 1:
                movePool = {
                Move("Weaken", MoveType::EFFECT, 0, 100, MoveEffect::SPDEF_DOWN, MoveTarget::ENEMY, 40),
                Move("Mana Shield", MoveType::EFFECT, 0, 100, MoveEffect::BLOCK, MoveTarget::ENEMY),
                Move("Fireball", MoveType::SPECIAL, 105, 85),
                Move("Frost Nova", MoveType::SPECIAL, 70, 95, MoveEffect::SPDEF_DOWN, MoveTarget::ENEMY, 25)
                };
                break;
            case 2:
                movePool = {
                Move("Arcane Missiles", MoveType::SPECIAL, 90, 100),
                Move("Silence", MoveType::EFFECT, 0, 100, MoveEffect::ATK_DOWN, MoveTarget::ENEMY, 40),
                Move("Magic Barrier", MoveType::EFFECT, 0, 100, MoveEffect::BLOCK, MoveTarget::ENEMY),
                Move("Meteor Strike", MoveType::SPECIAL, 130, 70)
                };
            };
            break;
            
        case PlayerType::ARCHER:
        switch(preset) {
            case 0:
            movePool = {
                Move("Cripple", MoveType::EFFECT, 0, 100, MoveEffect::ATK_DOWN, MoveTarget::ENEMY, 35),
                Move("Clean Shot", MoveType::SPECIAL, 85, 100),
                Move("Trap", MoveType::EFFECT, 0, 100, MoveEffect::SPDEF_DOWN, MoveTarget::ENEMY, 30),
                Move("Arrow Storm", MoveType::SPECIAL, 95, 85),
                Move("Piercing Shot", MoveType::SPECIAL, 85, 90, MoveEffect::DEF_DOWN, MoveTarget::ENEMY, 20)
            };
                break;
            case 1:
                movePool = {
                Move("Weaken", MoveType::EFFECT, 0, 100, MoveEffect::SPATK_DOWN, MoveTarget::ENEMY, 35),
                Move("Armored", MoveType::EFFECT, 0, 100, MoveEffect::DEF_UP, MoveTarget::SELF, 30),
                Move("Lightning Arrow", MoveType::SPECIAL, 80, 90, MoveEffect::SPD_DOWN, MoveTarget::ENEMY, 20),
                Move("Snipe", MoveType::SPECIAL, 115, 80)
                 };
                break;
            case 2:
                movePool = {
                Move("Mending Arrow", MoveType::EFFECT, 0, 100, MoveEffect::HEAL, MoveTarget::SELF, 20),
                Move("Poison Arrow", MoveType::SPECIAL, 60, 95, MoveEffect::ATK_DOWN, MoveTarget::ENEMY),
                Move("Bow Block", MoveType::EFFECT, 0, 100, MoveEffect::BLOCK, MoveTarget::ENEMY),
                Move("Multi Shot", MoveType::SPECIAL, 100, 90)

                };
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