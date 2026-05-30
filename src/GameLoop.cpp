#include "../include/GameLoop.h"
#include <iostream>
#include <cstdlib>

GameLoop::GameLoop():player(nullptr), currentFloor(nullptr), currentEnemy(nullptr), combat(nullptr), 
                     currentFloorNumber(1), isMiniBoss(false), previewEnemies{nullptr, nullptr, nullptr}, 
                     rewardDescription(""), eventResultScreen(GameScreen::DOOR_SELECT) {}

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
        eventResultScreen = advanceFloor();   // builds next floor, returns DOOR_SELECT or WIN
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
    return GameScreen::REWARDS;
}

GameScreen GameLoop::onRewardsDismissed() {
    // El jugador presiono ENTER en la pantalla de recompensas -> avanzar piso
    return advanceFloor();   // devuelve DOOR_SELECT o WIN
}

// Avanza al siguiente piso o declara victoria
GameScreen GameLoop::advanceFloor() {
    currentFloorNumber++;
    if (currentFloorNumber > MAX_FLOORS) {
        return GameScreen::WIN; // aqui podriamos poner la llamada a el final boss
    }
    buildFloor();
    if (player->hasRelic("Healing Charm")) {
        std::cout << "Healing Charm heals you for 10% HP as you ascend to the next floor!\n";
        player->heal(player->getMaxHp() * 10 / 100); // curacion de la reliquia al subir de piso
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
    isMiniBoss = door.isCorrupt();

    currentEnemy = previewEnemies[doorIndex];

    // spawnEnemyFromDoor devuelve Entity*, pero siempre crea Enemy o MiniBoss
    // (MiniBoss hereda de Enemy), por lo que el cast es seguro
    Entity* spawned = currentFloor->spawnEnemyFromDoor(door);
    currentEnemy = dynamic_cast<Enemy*>(spawned);

    // clean de los previews
    for (int i = 0; i < 3; i++) {
        delete previewEnemies[i];
        previewEnemies[i] = nullptr;
    }
}

void GameLoop::startCombat() {
    delete combat;
    // CombatEngine acepta Entity* para el enemigo
    combat = new CombatEngine(player, currentEnemy);
}

//ahora recibe parametro del move
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
}