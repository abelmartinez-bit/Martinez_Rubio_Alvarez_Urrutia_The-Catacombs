#include "../include/GameLoop.h"
#include <iostream>
#include <cstdlib>

GameLoop::GameLoop():player(nullptr), currentFloor(nullptr), currentEnemy(nullptr), combat(nullptr),
                     currentFloorNumber(1), isMiniBoss(false), isFinalBoss(false), previewEnemies{nullptr, nullptr, nullptr},
                     rewardDescription(""), eventResultScreen(GameScreen::DOOR_SELECT), needsEventFloorAdvance(false) {}

GameLoop::~GameLoop() {
    // player NO se destruye aqui: main.cpp se encarga de crear/destruir
    delete combat;
    delete currentEnemy;
    delete currentFloor;

    for (int i = 0; i < 3; i++) {
        delete previewEnemies[i];
        previewEnemies[i] = nullptr;
    }
}

// main loop

void GameLoop::startRun(Player* p) {
    player = p;
    currentFloorNumber = 1;
    isMiniBoss = false;
    isFinalBoss = false;
    buildFloor();
}

GameScreen GameLoop::selectDoor(int doorIndex) {
    if (!currentFloor) return GameScreen::DOOR_SELECT;

    std::vector<Door> doors = currentFloor->getDoors();

    if (doorIndex < 0 || doorIndex >= static_cast<int>(doors.size()))
        return GameScreen::DOOR_SELECT;

    const Door& door = doors[doorIndex];

    if (door.isEvent()) {
        // Evento: aplica efecto inmediato (curacion, pocion o reliquia)
        // y avanza de piso sin combate
        rewardDescription = currentFloor->triggerEvent(*player, door);
        // Defer floor advancement until after the fade transition completes to avoid
        // showing the next floor's doors during the fade-out animation
        needsEventFloorAdvance = true;
        eventResultScreen = GameScreen::DOOR_SELECT;  // placeholder, will be updated in performPendingEventFloorAdvance()
        return GameScreen::EVENT;
    }

    // Puerta normal o corrupta -> combate
    spawnFromDoor(door, doorIndex);
    startCombat();
    return GameScreen::COMBAT;
}

GameScreen GameLoop::onCombatFinished() {
    // Llamado por main.cpp cuando combat->isBattleOver() && rewardGiven
    if (!player->isAlive()) {
        return GameScreen::GAME_OVER;
    }
    if (isFinalBoss) {
        return GameScreen::WIN;
    }
    return GameScreen::REWARDS;
}

GameScreen GameLoop::onRewardsDismissed() {
    // El jugador presiono ENTER en la pantalla de recompensas -> avanzar piso
    return advanceFloor();   // devuelve DOOR_SELECT o WIN
}

GameScreen GameLoop::onEventDismissed() {
    // Return the stored result from when event was triggered
    return eventResultScreen;
}

GameScreen GameLoop::performPendingEventFloorAdvance() {
    // Called by main.cpp after event transition completes to perform deferred floor advance
    if (needsEventFloorAdvance) {
        needsEventFloorAdvance = false;
        eventResultScreen = advanceFloor();  // builds next floor, returns DOOR_SELECT or WIN
    }
    return eventResultScreen;
}

// Avanza al siguiente piso o declara victoria
GameScreen GameLoop::advanceFloor() {
    currentFloorNumber++;
    if (currentFloorNumber > MAX_FLOORS) {
        startFinalBossCombat();
        return GameScreen::COMBAT; // se realizo el combate contra el boss final, main.cpp se encargara de verificar victoria o derrota
    }
    buildFloor();

    // Apply HEAL_ON_FLOOR relic effects for every matching relic the player owns
    for (Relic* r : player->getRelics()) {
        UtilityRelic* ur = dynamic_cast<UtilityRelic*>(r);
        if (ur && ur->getUtilityType() == UtilityType::HEAL_ON_FLOOR) {
            int healAmt = player->getMaxHp() * ur->getValue() / 100;
            player->heal(healAmt);
            std::cout << r->getName() << " heals you for " << ur->getValue() << "% HP as you ascend! (+" << healAmt << " HP)\n";
        }
    }

    return GameScreen::DOOR_SELECT;
}

void GameLoop::buildFloor() {
    delete currentFloor;
    currentFloor = new Floor(currentFloorNumber);
    currentFloor->generateDoors(*player);  // usa los probability modifiers
    buildPreviewEnemies();
}

void GameLoop::buildPreviewEnemies() {
    std::vector<Door> doors = currentFloor->getDoors();
    for (int i = 0; i < static_cast<int>(doors.size()); i++) {
        delete previewEnemies[i];
        previewEnemies[i] = nullptr;
        if (i < static_cast<int>(doors.size()) && !doors[i].isEvent()) {
            // spawneamos el enemigo para mostrar sus stats en la pantalla de seleccion
            // spawnEnemyFromDoor devuelve Entity*, pero siempre crea Enemy o MiniBoss
            // (MiniBoss hereda de Enemy), por lo que el cast es seguro
            Entity* spawned = currentFloor->spawnEnemyFromDoor(doors[i]);
            previewEnemies[i] = dynamic_cast<Enemy*>(spawned);
        }
    }
}

void GameLoop::spawnFromDoor(const Door& door, int doorIndex) {

    delete currentEnemy;
    currentEnemy = nullptr;

    isMiniBoss = door.isCorrupt();  // Set flag here; startCombat doesnt reset

    // Transfer ownership: currentEnemy now owns this pointer.
    currentEnemy = previewEnemies[doorIndex];
    previewEnemies[doorIndex] = nullptr;  // FIX: Prevent double-free in startCombat loop.

    // Clean up the remaining previews (doorIndex slot is already nullptr).
    for (int i = 0; i < 3; i++) {
        if (i != doorIndex) {
            delete previewEnemies[i];
            previewEnemies[i] = nullptr;
        }
    }
}

void GameLoop::startCombat() {
    delete combat;
    combat = nullptr;

    // spawnFromDoor already transferred/deleted all preview enemies.
    // kept in case startCombat is ever called without spawnFromDoor.
    for (int i = 0; i < 3; i++) {
        delete previewEnemies[i];
        previewEnemies[i] = nullptr;
    }

    // Resetting them here made isMiniBoss always false during combat, breaking
    // miniboss rewards and the drawCombat miniboss branch.

    combat = new CombatEngine(player, currentEnemy);
}

void GameLoop::startFinalBossCombat() {
    // cleanup from the previous floor
    delete combat;
    combat = nullptr;
    delete currentEnemy;
    currentEnemy = nullptr;
    for (int i = 0; i < 3; i++) {
        delete previewEnemies[i];
        previewEnemies[i] = nullptr;
    }

    isFinalBoss = true;
    isMiniBoss = false;

    FinalBoss* boss = new FinalBoss("NECRON", EnemyType::SENTINEL, 500, 50, 65, 50, 65, 40);

    currentEnemy = boss;

    combat = new CombatEngine(player, currentEnemy);

    std::cout << "The ground trembles. NECRON awakens...\n";
}

// ahora recibe parametro del move
void GameLoop::advanceTurn(int moveIndex) {
    if (!combat || combat->isBattleOver()) return;
    combat->executeTurn(moveIndex);
}

void GameLoop::usePotionInCombat() {
    if (!combat || combat->isBattleOver()) return;

    // getPotions() devuelve una copia; verificamos si hay pociones disponibles
    std::vector<Potion> potions = player->getPotions();
    if (potions.empty()) return;

    // Decidir objetivo segun el target de la pocion
    Entity* target = (potions[0].getTarget() == MoveTarget::SELF) ? static_cast<Entity*>(player) : static_cast<Entity*>(currentEnemy);

    // combat->usePotion llama internamente a player->usePotion(0, target)
    // que aplica el efecto y borra la pocion del inventario real del jugador
    combat->usePotion(potions[0], *target);
}

void GameLoop::reset() {
    delete combat;
    combat = nullptr;
    delete currentEnemy;
    currentEnemy = nullptr;
    delete currentFloor;
    currentFloor = nullptr;

    for (int i = 0; i < 3; i++) {
        delete previewEnemies[i];
        previewEnemies[i] = nullptr;
    }

    // player no se destruye aqui
    player = nullptr;
    currentFloorNumber = 1;
    isMiniBoss = false;
    isFinalBoss = false;
}