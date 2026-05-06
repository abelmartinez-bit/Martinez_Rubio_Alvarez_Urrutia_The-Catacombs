 #include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.h"
#include "Enemy.h"
#include "CombatEngine.h"

int main() {
    // Crear ventana
    sf::RenderWindow window(sf::VideoMode({800, 600}), "The Catacombs");
    window.setFramerateLimit(60);

    // Cargar fuente
    sf::Font font;
    if (!font.openFromFile("assets/fonts/Arial.ttf")) {
        std::cerr << "Error: no se pudo cargar la fuente\n";
        return 1;
    }

    // Crear jugador y enemigo
    Player player("Archer", 100, 10, 15, 25, 15, 25, PlayerType::ARCHER);
    Enemy enemy("Sentinel", EnemyType::SENTINEL, 50, 10, 10, 10, 15, 8);

    player.initializeMovePool();
    player.selectMovesFromPool();
    enemy.initialize();

    CombatEngine combat(&player, &enemy);

    // Textos
    sf::Text titleText(font, "THE CATACOMBS", 36);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition({250.f, 20.f});

    sf::Text playerText(font, "", 20);
    playerText.setFillColor(sf::Color::Green);
    playerText.setPosition({50.f, 100.f});

    sf::Text enemyText(font, "", 20);
    enemyText.setFillColor(sf::Color::Red);
    enemyText.setPosition({500.f, 100.f});

    sf::Text logText(font, "", 16);
    logText.setFillColor(sf::Color::Yellow);
    logText.setPosition({50.f, 350.f});

    sf::Text instructionText(font, "Presiona ESPACIO para avanzar turno", 16);
    instructionText.setFillColor(sf::Color::White);
    instructionText.setPosition({200.f, 550.f});

    sf::Text resultText(font, "", 28);
    resultText.setFillColor(sf::Color::White);
    resultText.setPosition({250.f, 260.f});

    // Game loop
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (event->is<sf::Event::KeyPressed>()) {
                auto* keyEvent = event->getIf<sf::Event::KeyPressed>();
                if (keyEvent && keyEvent->code == sf::Keyboard::Key::Space) {
                    if (!combat.isBattleOver()) {
                        int playerMove = rand() % 4;
                        combat.executeTurn(playerMove);
                    }
                }
            }
        }

        // Actualizar textos con estado actual
        playerText.setString(
            player.getName() + "\n" +
            "HP: " + std::to_string(player.getHp()) + "/" + std::to_string(player.getMaxHp()) + "\n" +
            "ATK: " + std::to_string(player.getAtk()) + " | DEF: " + std::to_string(player.getDef()) + "\n" +
            "SPD: " + std::to_string(player.getSpd())
        );

        enemyText.setString(
            enemy.getName() + "\n" +
            "HP: " + std::to_string(enemy.getHp()) + "/" + std::to_string(enemy.getMaxHp()) + "\n" +
            "ATK: " + std::to_string(enemy.getAtk()) + " | DEF: " + std::to_string(enemy.getDef()) + "\n" +
            "SPD: " + std::to_string(enemy.getSpd())
        );

        logText.setString("Log:\n" + combat.getCombatLog());

        if (combat.isBattleOver()) {
            resultText.setString(combat.getWinner() + " wins!");
            instructionText.setString("Combate terminado");
        }

        // Dibujar
        window.clear(sf::Color(20, 20, 40)); // fondo azul oscuro
        window.draw(titleText);
        window.draw(playerText);
        window.draw(enemyText);
        window.draw(logText);
        window.draw(resultText);
        window.draw(instructionText);
        window.display();
    }

    return 0;
}