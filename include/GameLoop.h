#pragma once
#include "Player.h"
#include "Floor.h"
#include "Enemy.h"
#include "CombatEngine.h"
#include "MainMenu.h"   // GameScreen enum

// GameLoop coordina el flujo completo de una partida

class GameLoop {
private:
    Player* player;          // not owned, main.cpp crea y destruye
    Floor*  currentFloor;    // owned
    Enemy*  currentEnemy;    // owned (Enemy* cubre MiniBoss tambien)
    CombatEngine* combat;    // owned

    int  currentFloorNumber;
    bool isMiniBoss;

    static const int MAX_FLOORS = 5; // can be changed

    // helpers internos
    void       buildFloor();
    void       spawnFromDoor(const Door& door);
    void       startCombat();
    GameScreen advanceFloor();   // incrementa piso, genera doors o declara WIN

public:
    GameLoop();
    ~GameLoop();

    // Llamado despues de que el jugador elige su personaje.
    // Inicia el piso 1 y genera las 3 puertas.
    void startRun(Player* p);

    // Llamado cuando el jugador presiona 1 / 2 / 3 en DOOR_SELECT.
    GameScreen selectDoor(int doorIndex);

    // Llamado desde main.cpp despues de que el combate termino
    // Devuelve: DOOR_SELECT, WIN, GAME_OVER
    GameScreen onCombatFinished();

    void advanceTurn();       // SPACE: ejecuta un turno (move aleatorio por ahora)
    void usePotionInCombat(); // P: usa la primera pocion del inventario

    void reset(); // back to main menu

    // Getters
    Floor*        getCurrentFloor()  const { return currentFloor; }
    Enemy*        getCurrentEnemy()  const { return currentEnemy; }
    CombatEngine* getCombat()        const { return combat; }
    int           getFloorNumber()   const { return currentFloorNumber; }
    bool          getIsMiniBoss()    const { return isMiniBoss; }
};