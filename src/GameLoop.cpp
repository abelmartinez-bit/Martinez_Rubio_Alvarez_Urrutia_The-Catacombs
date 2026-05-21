#include "../include/GameLoop.h"
#include <iostream>
#include <cstdlib>

GameLoop::GameLoop() : player(nullptr), currentFloor(nullptr), currentEnemy(nullptr), combat(nullptr), currentFloorNumber(1), isMiniBoss(false) {}

GameLoop::~GameLoop() {
    // player NO se destruye aqui: main.cpp se encarga de crear/destruir
    delete combat;
    delete currentEnemy;
    delete currentFloor;
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
        currentFloor->triggerEvent(*player, door);
        return advanceFloor();
    }

    // Puerta normal o corrupta -> combate
    spawnFromDoor(door);
    startCombat();
    return GameScreen::COMBAT;
}

GameScreen GameLoop::onCombatFinished() {
    // Llamado por main.cpp cuando combat->isBattleOver() && rewardGiven
    if (!player->isAlive()) {
        return GameScreen::GAME_OVER;
    }
    return advanceFloor();
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
}

void GameLoop::spawnFromDoor(const Door& door) {
    delete currentEnemy;
    currentEnemy = nullptr;
    isMiniBoss = door.isCorrupt();

    // spawnEnemyFromDoor devuelve Entity*, pero siempre crea Enemy o MiniBoss
    // (MiniBoss hereda de Enemy), por lo que el cast es seguro
    Entity* spawned = currentFloor->spawnEnemyFromDoor(door);
    currentEnemy = dynamic_cast<Enemy*>(spawned);
}

void GameLoop::startCombat() {
    delete combat;
    // CombatEngine acepta Entity* para el enemigo
    combat = new CombatEngine(player, currentEnemy);
}


void GameLoop::advanceTurn() {
    if (!combat || combat->isBattleOver()) return;
    int playerMove = rand() % 4;   // aleatorio por ahora; la UI lo mejorara
    combat->executeTurn(playerMove);
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

    // player no se destruye aqui
    player = nullptr;
    currentFloorNumber = 1;
    isMiniBoss = false;
}