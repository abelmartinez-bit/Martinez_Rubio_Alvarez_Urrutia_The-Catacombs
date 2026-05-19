#include "../include/ScreenCombat.h"
#include <cstdlib>
#include <iostream>


void drawCombat(sf::RenderWindow& window,
                Player* player, Enemy* enemy, CombatEngine* combat,
                sf::Text& playerText, sf::Text& enemyText,
                sf::Text& logText, sf::Text& resultText,
                sf::Text& instructionText, sf::Text& combatTitleText,
                bool& rewardGiven, const sf::Sprite& combatSprite,
                Floor* floor, bool isMiniBoss) {

    // Actualizar textos con estado actual
    playerText.setString(
        player->getName() + " | " + combat->potionList(*player) + "\n" +
        "HP: " + std::to_string(player->getHp()) + "/" + std::to_string(player->getMaxHp()) + "\n" +
        "SP.ATK: " + std::to_string(player->getSpAtk()) + " | DEF: " + std::to_string(player->getDef()) + "\n" +
        "SPD: " + std::to_string(player->getSpd())
    );

    enemyText.setString(
        enemy->getName() + "\n" +
        "HP: " + std::to_string(enemy->getHp()) + "/" + std::to_string(enemy->getMaxHp()) + "\n" +
        "ATK: " + std::to_string(enemy->getAtk()) + " | SP.DEF: " + std::to_string(enemy->getSpDef()) + "\n" +
        "SPD: " + std::to_string(enemy->getSpd())
    );

    logText.setString("Log:\n" + combat->getCombatLog());

    // verificar si el combate termino para mostrar el resultado y entregar la
    // recompensa si el jugador gano y no se ha entregado ya
    if (combat->isBattleOver()) {
        resultText.setString(combat->getWinner() + " wins!");

        // Entregar recompensa una sola vez si el jugador gano
        if (player->isAlive() && !rewardGiven) {
            floor->giveRewards(*player, enemy, isMiniBoss);
            std::cout << combat->relicList(player) << std::endl; // mostrar reliquias obtenidas en la consola por ahora
            rewardGiven = true;
        } else if (!player->isAlive() && !rewardGiven) {
            // El jugador murio. no hay recompensa, solo marcamos para evitar re-entrar
            rewardGiven = true;
        }

        // Instruccion de avance — main.cpp escucha ENTER para continuar
        if (player->isAlive()) {
            instructionText.setString("Victory! Press ENTER to continue.");
        } else {
            instructionText.setString("Defeated...   Press ENTER to continue.");
        }
    } else {
        // Combate en curso
        instructionText.setString("SPACE: avanzar turno   |   P: usar pocion");
    }

    // Dibujar
    window.clear(sf::Color(0, 0, 0));
    window.draw(combatSprite);
    window.draw(combatTitleText);
    window.draw(playerText);
    window.draw(enemyText);
    window.draw(logText);
    window.draw(resultText);
    window.draw(instructionText);
}