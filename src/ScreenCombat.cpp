#include "../include/ScreenCombat.h"
#include <cstdlib>
#include <iostream>


void drawCombat(sf::RenderWindow& window, sf::Font& font, Player* player, Enemy* enemy, CombatEngine* combat, 
                sf::Text& playerText, sf::Text& enemyText, sf::Text& logText, sf::Text& resultText, sf::Text& instructionText, 
                sf::Text& combatTitleText, bool& rewardGiven, const sf::Sprite& combatSprite, Floor* floor, bool isMiniBoss) {

    // Actualizar textos con estado actual
    playerText.setString(
        player->getName() + " | " + combat->potionList(*player) + "\n" +
        "HP: " + std::to_string(player->getHp()) + "/" + std::to_string(player->getMaxHp()) + "\n" +
        player->getHighestAtkStr() + " | " + player->getHighestDefStr() + "\n" +
        "SPD: " + std::to_string(player->getSpd())
    );

    enemyText.setString(
        enemy->getName() + "\n" +
        "HP: " + std::to_string(enemy->getHp()) + "/" + std::to_string(enemy->getMaxHp()) + "\n" +
        enemy->getHighestAtkStr() + " | " + enemy->getHighestDefStr() + "\n" +
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
        instructionText.setString("Click a move to attack   |   P: usar pocion");
        resultText.setString("");
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

    //nuevos Botones de moves cuando el combate esta en curso
    if (!combat->isBattleOver()) {
        std::vector<Move> moves = player->getMoves();
        const float moveBtnW = 155.f, moveBtnH = 50.f;
        const float moveBtnY = 480.f;
        const float startX = (800.f - (moveBtnW * 4 + 20.f * 3)) / 2.f;

        for (int i = 0; i < 4; i++) {
            float bx = startX + i * (moveBtnW + 20.f);

            sf::RectangleShape border({moveBtnW + 4.f, moveBtnH + 4.f});
            border.setPosition({bx - 2.f, moveBtnY - 2.f});
            border.setFillColor(sf::Color(120, 80, 20));
            window.draw(border);

            sf::RectangleShape bg({moveBtnW, moveBtnH});
            bg.setPosition({bx, moveBtnY});
            bg.setFillColor(sf::Color(20, 12, 5, 210));
            window.draw(bg);

            sf::Text moveLabel(font, moves[i].getName(), 15);
            moveLabel.setFillColor(sf::Color(210, 160, 60));
            sf::FloatRect flb = moveLabel.getLocalBounds();
            moveLabel.setPosition({bx + (moveBtnW - flb.size.x) / 2.f - flb.position.x,
                                   moveBtnY + (moveBtnH - flb.size.y) / 2.f - flb.position.y});
            window.draw(moveLabel);
        }
    }
}