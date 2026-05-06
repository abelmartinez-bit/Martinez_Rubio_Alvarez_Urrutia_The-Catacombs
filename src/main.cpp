 #include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.h"
#include "Enemy.h"
#include "CombatEngine.h"
#include "StatRelic.h"
#include "UtilityRelic.h"
#include "ProbabilityRelic.h"
#include "MoveRelic.h"
#include <cstdlib>

Relic* createRandomRelic() {
    int roll = rand() % 7;

    switch (roll) {
        case 0:
            return new StatRelic(
                "Heart Stone",
                "Increases max HP by 20.",
                RelicRarity::COMMON,
                StatType::HP,
                20
            );

        case 1:
            return new StatRelic(
                "Warrior Fang",
                "Increases ATK by 5.",
                RelicRarity::COMMON,
                StatType::ATK,
                5
            );

        case 2:
            return new StatRelic(
                "Hunter Boots",
                "Increases SPD by 4.",
                RelicRarity::RARE,
                StatType::SPD,
                4
            );

        case 3:
            return new UtilityRelic(
                "Potion Belt",
                "Adds 1 extra potion slot.",
                RelicRarity::COMMON,
                UtilityType::EXPAND_POTION_SLOTS,
                1
            );

        case 4:
            return new UtilityRelic(
                "Healing Charm",
                "Heals 15 HP when obtained.",
                RelicRarity::COMMON,
                UtilityType::HEAL_ON_FLOOR,
                15
            );

        case 5:
            return new ProbabilityRelic(
                "Sharp Eye",
                "Increases accuracy of all moves by 5.",
                RelicRarity::RARE,
                ProbabilityType::MOVE_ACCURACY,
                5
            );

        default:
            return new StatRelic(
                "Golden Ring",
                "Increases SP.ATK by 5.",
                RelicRarity::RARE,
                StatType::SPATK,
                5
            );
    }
}

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

    // flag para saber si la recomensa de el combate ya se entrego para no ser dada varias veces
        bool rewardGiven = false;

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

        // verificar si el combate termino para mostrar el resultado y entregar la 
        //recompensa si el jugador gano y no se ha entregado ya

        if (combat.isBattleOver()) {
            resultText.setString(combat.getWinner() + " wins!");
            instructionText.setString("Combate terminado");

              if (player.isAlive() && !rewardGiven) {
                player.gainExp(enemy.getExpReward());
                player.addRelic(createRandomRelic());
                rewardGiven = true;
        }
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
