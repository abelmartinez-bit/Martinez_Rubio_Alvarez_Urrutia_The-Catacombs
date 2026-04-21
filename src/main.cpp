#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.h"
#include "Enemy.h"
#include "CombatEngine.h"

//prueba basica del combate sin interfaz gráfica, solo para testear que todo funcione bien 
//con los objetos y la lógica del combate antes de integrar con SFML, asi se pueden 
//detectar errores de memoria o lógica mas fácilmente
int main()
{
    //try para capturar cualquier error inesperado durante el testeo del combate, como errores de memoria o lógica
    try {
        std::cout << "\n========== TEST GAME ENGINE ==========\n" << std::endl;
        
        // Crear jugador y enemigo
        Player player("Archer", 100, 10, 15, 25, 15, 25, PlayerType::ARCHER);
        Enemy enemy("Sentinel", EnemyType::SENTINEL, 50, 10, 10, 10, 15, 8);
        
        std::cout << "✓ Objetos creados\n" << std::endl;
        
        // Inicializar movimientos
        player.initializeMovePool();
        player.selectMovesFromPool();
        enemy.initializeMovePool();
        enemy.selectMovesFromPool();
        
        std::cout << "Movements initialized\n" << std::endl;
        
        std::cout << player.getName() << " (HP: " << player.getHp() << ")" << std::endl;
        std::cout << "vs" << std::endl;
        std::cout << enemy.getName() << " (HP: " << enemy.getHp() << ")\n" << std::endl;
        
        // Crear motor de combate
        CombatEngine combat(&player, &enemy);
        
        std::cout << "Combat engine initialized\n" << std::endl;
        
        // Simular turnos automáticos
        int turn = 1;
        while (!combat.isBattleOver()) {
            std::cout << "--- TURNO " << turn << " ---" << std::endl;
            int playerMove = rand() % 4; 
            combat.executeTurn(playerMove);
            std::cout << combat.getCombatLog();
            std::cout << player.getName() << " HP: " << player.getHp() << " | " 
                      << enemy.getName() << " HP: " << enemy.getHp() << "\n" << std::endl;
            turn++;
        }
        
        std::cout << "========== FIGHT FINISHED  ==========" << std::endl;
        std::cout << combat.getWinner() << " wins!\n" << std::endl;
        
        std::cout << "Limpieza de memoria\n" << std::endl;
    }
    // catch para cualquier excepción estándar, como errores de memoria o lógica
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    // catch para cualquier otro tipo de error no estándar
    catch (...) {
        std::cerr << "Error desconocido" << std::endl;
        return 1;
    }

    return 0;
}