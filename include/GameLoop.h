#pragma once
#include "Player.h"
#include "Floor.h"
#include "Enemy.h"
#include "FinalBoss.h"
#include "CombatEngine.h"
#include "MainMenu.h"   // GameScreen enum

// GameLoop coordina el flujo completo de una partida

class GameLoop {
private:
    Player* player;          // not owned, main.cpp crea y destruye
    Floor*  currentFloor;    // owned
    Enemy*  currentEnemy;    // owned (Enemy* cubre MiniBoss tambien)
    CombatEngine* combat;    // owned

    Enemy* previewEnemies[3]; // preview for the stat show on the door select screen

    int  currentFloorNumber;
    bool isMiniBoss;
    bool isFinalBoss;

    static const int MAX_FLOORS = 10; // can be changed

    // helpers internos
    void buildFloor();
    void buildPreviewEnemies();
    void spawnFromDoor(const Door& door, int doorIndex);
    void startCombat();
    void startFinalBossCombat();
    GameScreen advanceFloor();   // incrementa piso, genera doors o declara WIN

    std::string rewardDescription;
    GameScreen eventResultScreen;

public:
    GameLoop();
    ~GameLoop();

    // Llamado despues de que el jugador elige su personaje.
    // Inicia el piso 1 y genera las 3 puertas.
    void startRun(Player* p);

    // Llamado cuando el jugador presiona 1 / 2 / 3 en DOOR_SELECT.
    GameScreen selectDoor(int doorIndex);

    // Llamado desde main.cpp despues de que el combate termino
    GameScreen onCombatFinished(); // Devuelve: DOOR_SELECT, WIN, GAME_OVER
    GameScreen onEventDismissed() const { return eventResultScreen; }
    GameScreen onRewardsDismissed();

    void advanceTurn(int moveIndex); //ahora recibe el index del movimiento del player
    void usePotionInCombat(); // P: usa la primera pocion del inventario

    void reset(); // back to main menu

    // Getters
    Floor* getCurrentFloor()  const { return currentFloor; }
    Enemy* getCurrentEnemy()  const { return currentEnemy; }

    Enemy* getPreviewEnemy(int index) const { 
        if (index < 0 || index >= 3) return nullptr;
        return previewEnemies[index]; 
    }

    std::string getRewardDescription()  const { return rewardDescription; }

    CombatEngine* getCombat()        const { return combat; }
    int getFloorNumber()   const { return currentFloorNumber; }
    bool getIsMiniBoss()    const { return isMiniBoss; }
    bool getIsFinalBoss() const { return isFinalBoss; } 
};