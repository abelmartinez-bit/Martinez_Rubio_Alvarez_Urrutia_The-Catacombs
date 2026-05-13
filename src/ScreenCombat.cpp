#include "ScreenCombat.h"
#include <cstdlib>
#include <iostream>

void handleCombatKey(sf::Keyboard::Key key, Player* player, CombatEngine* combat) {
    // SPACE avanza turno con un move random
    if (key == sf::Keyboard::Key::Space) {
        if (!combat->isBattleOver()) {
            int playerMove = rand() % 4;
            combat->executeTurn(playerMove);
        }
    }
    // P usa la primera potion disponible
    else if (key == sf::Keyboard::Key::P) {
        if (!combat->isBattleOver() && !player->getPotions().empty()) {
            combat->usePotion(player->getPotions()[0], *player);
        }
    }
}

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
        "SPATK: " + std::to_string(player->getSpAtk()) + " | DEF: " + std::to_string(player->getDef()) + "\n" +
        "SPD: " + std::to_string(player->getSpd())
    );

    enemyText.setString(
        enemy->getName() + "\n" +
        "HP: " + std::to_string(enemy->getHp()) + "/" + std::to_string(enemy->getMaxHp()) + "\n" +
        "ATK: " + std::to_string(enemy->getAtk()) + " | SPDEF: " + std::to_string(enemy->getSpDef()) + "\n" +
        "SPD: " + std::to_string(enemy->getSpd())
    );

    logText.setString("Log:\n" + combat->getCombatLog());

    // verificar si el combate termino para mostrar el resultado y entregar la
    // recompensa si el jugador gano y no se ha entregado ya
    if (combat->isBattleOver()) {
        resultText.setString(combat->getWinner() + " wins!");
        instructionText.setString("Combate Finalizado. Cierra la ventana para salir.");

        if (player->isAlive() && !rewardGiven) {
            // Floor decide exp + rareza del relic segun los modifiers del jugador
            floor->giveRewards(*player, enemy, isMiniBoss);
            std::cout << combat->relicList(player) << std::endl; // mostrar los relics obtenidos en el log por ahora, se puede mejorar con una pantalla de recompensa despues
            rewardGiven = true;
        }
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