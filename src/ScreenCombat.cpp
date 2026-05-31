#include "../include/ScreenCombat.h"
#include <cstdlib>
#include <iostream>

//nueva funcion para dibujar barras de hp, con colores segun porcentaje
void drawHpBar(sf::RenderWindow& window,
                      float x, float y, float w, float h,
                      int hp, int maxHp)
{
    // Calcular el ratio de HP y determinar el color de la barra
    float ratio = (maxHp > 0) ? static_cast<float>(hp) / maxHp : 0.f;
    if (ratio < 0.f) ratio = 0.f;
    if (ratio > 1.f) ratio = 1.f;

    sf::Color barColor = (ratio > 0.5f) ? sf::Color(80, 200, 80)
                       : (ratio > 0.25f) ? sf::Color(220, 200, 40)
                                         : sf::Color(210, 50, 50);

    sf::RectangleShape bg({w, h});
    bg.setPosition({x, y});
    bg.setFillColor(sf::Color(60, 60, 60));
    window.draw(bg);

    sf::RectangleShape fill({w * ratio, h});
    fill.setPosition({x, y});
    fill.setFillColor(barColor);
    window.draw(fill);
}

//funcion principal, con cambios para dibujar la pantalla de combate, tipo pokemon y entrega recompensa si termino
void drawCombat(sf::RenderWindow& window, sf::Font& font, Player* player, Enemy* enemy, CombatEngine* combat, 
                sf::Text& playerText, sf::Text& enemyText, sf::Text& logText, sf::Text& resultText, sf::Text& instructionText, 
                bool& rewardGiven, const sf::Sprite& combatSprite, Floor* floor, bool isMiniBoss, 
                std::string& rewardDescription) {

    // Actualizar textos con estado actual
    playerText.setString(
        player->getName() + " | " + combat->potionList(*player) + "\n" +
        "HP: " + std::to_string(player->getHp()) + "/" + std::to_string(player->getMaxHp()) + "\n" +
        player->getHighestAtkStr() + " | " + player->getHighestDefStr() + "\n" +
        "SPD: " + std::to_string(player->getSpd())
    );

    //ya no mostramos stats de enemy, solo la caja con nombre y hp bar

    logText.setString("Log:\n" + combat->getCombatLog());

    // verificar si el combate termino para mostrar el resultado y entregar la
    // recompensa si el jugador gano y no se ha entregado ya
    if (combat->isBattleOver()) {
        resultText.setString(combat->getWinner() + " wins!");

        // Entregar recompensa una sola vez si el jugador gano
        if (player->isAlive() && !rewardGiven) {
            rewardDescription = floor->giveRewards(*player, enemy, isMiniBoss);
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

    //nuevas cajas de player y enemy
    // Caja enemy (arriba derecha)
    const float eBoxX = 430.f, eBoxY = 30.f, eBoxW = 330.f, eBoxH = 80.f;

    sf::RectangleShape eBorder({eBoxW + 4.f, eBoxH + 4.f});
    eBorder.setPosition({eBoxX - 2.f, eBoxY - 2.f});
    eBorder.setFillColor(sf::Color(100, 65, 15));
    window.draw(eBorder);

    sf::RectangleShape eBox({eBoxW, eBoxH});
    eBox.setPosition({eBoxX, eBoxY});
    eBox.setFillColor(sf::Color(15, 8, 2, 215));
    window.draw(eBox);

    sf::Text eName(font, enemy->getName(), 20);
    eName.setFillColor(sf::Color(240, 20, 20)); 
    eName.setPosition({eBoxX + 10.f, eBoxY + 8.f});
    window.draw(eName);

    sf::Text eHpLbl(font, "HP", 14);
    eHpLbl.setFillColor(sf::Color(180, 140, 80));
    eHpLbl.setPosition({eBoxX + 10.f, eBoxY + 40.f});
    window.draw(eHpLbl);

    drawHpBar(window, eBoxX + 40.f, eBoxY + 44.f, eBoxW - 55.f, 12.f,
            enemy->getHp(), enemy->getMaxHp());

    // Caja player (abajo izquierda)
    const float pBoxX = 30.f, pBoxY = 330.f, pBoxW = 330.f, pBoxH = 95.f;

    sf::RectangleShape pBorder({pBoxW + 4.f, pBoxH + 4.f});
    pBorder.setPosition({pBoxX - 2.f, pBoxY - 2.f});
    pBorder.setFillColor(sf::Color(100, 65, 15));
    window.draw(pBorder);

    sf::RectangleShape pBox({pBoxW, pBoxH});
    pBox.setPosition({pBoxX, pBoxY});
    pBox.setFillColor(sf::Color(15, 8, 2, 215));
    window.draw(pBox);

    sf::Text pName(font, player->getName() + "   Lv." + std::to_string(player->getLevel()), 20);
    pName.setFillColor(sf::Color(210, 160, 60));
    pName.setPosition({pBoxX + 10.f, pBoxY + 8.f});
    window.draw(pName);

    sf::Text pHpLbl(font, "HP", 14);
    pHpLbl.setFillColor(sf::Color(180, 140, 80));
    pHpLbl.setPosition({pBoxX + 10.f, pBoxY + 42.f});
    window.draw(pHpLbl);

    drawHpBar(window, pBoxX + 40.f, pBoxY + 46.f, pBoxW - 55.f, 12.f,
          player->getHp(), player->getMaxHp());

    std::string hpStr = std::to_string(player->getHp()) + " / " + std::to_string(player->getMaxHp());
    sf::Text pHpNum(font, hpStr, 14);
    pHpNum.setFillColor(sf::Color(200, 185, 150));
    sf::FloatRect hnb = pHpNum.getLocalBounds();
    pHpNum.setPosition({pBoxX + pBoxW - hnb.size.x - hnb.position.x - 10.f, pBoxY + 68.f});
    window.draw(pHpNum);
    //fin cajas player y enemy

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