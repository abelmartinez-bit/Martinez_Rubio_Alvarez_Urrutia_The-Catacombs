#include "../include/ScreenCombat.h"
#include "../include/FinalBoss.h"
#include <cstdlib>
#include <iostream>


void drawCombat(sf::RenderWindow& window, sf::Font& font, Player* player, Enemy* enemy, CombatEngine* combat, 
                sf::Text& playerText, sf::Text& enemyText, sf::Text& logText, sf::Text& resultText, sf::Text& instructionText, 
                sf::Text& combatTitleText, bool& rewardGiven, const sf::Sprite& combatSprite, Floor* floor, bool isMiniBoss, std::string& rewardDescription, bool isFinalBoss) {

    // Reset text colors to original values (in case they were modified by drawFinalBossCombat)
    playerText.setFillColor(sf::Color::Green);
    enemyText.setFillColor(sf::Color::Red);
    logText.setFillColor(sf::Color::Yellow);
    resultText.setFillColor(sf::Color::Red);
    instructionText.setFillColor(sf::Color::White);

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
            if (isFinalBoss) {
                rewardDescription = "Congratulations on defeating the Final Boss! You have conquered The Catacombs!";
                rewardGiven = true;  // was missing, caused rewards to re-trigger every frame
            } else {
                rewardDescription = floor->giveRewards(*player, enemy, isMiniBoss);
                rewardGiven = true;
            }   
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
            moveLabel.setPosition({bx + (moveBtnW - flb.size.x) / 2.f - flb.position.x, moveBtnY + (moveBtnH - flb.size.y) / 2.f - flb.position.y});
            window.draw(moveLabel);
        }
    }
}

// remember to swap final boss music and background in main.cpp when calling this function

void drawFinalBossCombat(sf::RenderWindow& window, sf::Font& font, Player* player, Enemy* enemy, CombatEngine* combat,
                         sf::Text& playerText, sf::Text& enemyText, sf::Text& logText, sf::Text& resultText,sf::Text& instructionText, 
                         bool& rewardGiven, const sf::Sprite& finalBossBackground, Floor* floor, std::string& rewardDescription) {

    // check phase for display
    FinalBoss* boss = dynamic_cast<FinalBoss*>(enemy);
    int phase = boss ? boss->getCurrentPhase() : 1;

    // stat texts
    playerText.setString(
        player->getName() + "  |  " + combat->potionList(*player) + "\n" +
        "HP: " + std::to_string(player->getHp()) + " / " + std::to_string(player->getMaxHp()) + "\n" +
        player->getHighestAtkStr() + "   " + player->getHighestDefStr() + "\n" +
        "SPD: " + std::to_string(player->getSpd())
    );

    std::string phaseTag = "  [Phase " + std::string(phase == 1 ? "I" : phase == 2 ? "II" : "III") + "]";
    enemyText.setString(
        enemy->getName() + phaseTag + "\n" +
        "HP: " + std::to_string(enemy->getHp()) + " / " + std::to_string(enemy->getMaxHp()) + "\n" +
        enemy->getHighestAtkStr() + "   " + enemy->getHighestDefStr() + "\n" +
        "SPD: " + std::to_string(enemy->getSpd())
    );

    logText.setString("Log:\n" + combat->getCombatLog());

    // handle end of battle
    if (combat->isBattleOver()) {
        resultText.setString(combat->getWinner() + " wins!");

        if (player->isAlive() && !rewardGiven) {
            rewardDescription = "The darkness lifts. NECRON has fallen.\nThe Catacombs are silent at last.\n\nYou have conquered The Catacombs!";
            rewardGiven = true;
        } else if (!player->isAlive() && !rewardGiven) {
            rewardGiven = true;
        }

        if (player->isAlive())
            instructionText.setString("NECRON has been vanquished!   Press ENTER to continue.");
        else
            instructionText.setString("Consumed by the darkness...   Press ENTER to continue.");
    } else {
        instructionText.setString("Click a move to attack   |   P: use potion");
        resultText.setString("");
    }

    window.clear(sf::Color(0, 0, 0));
    window.draw(finalBossBackground);

    // red tint overlay
    sf::RectangleShape atmo({800.f, 600.f});
    atmo.setFillColor(sf::Color(90, 0, 0, 110));
    window.draw(atmo);

    // boss banner
    sf::RectangleShape titleBg({800.f, 58.f});
    titleBg.setFillColor(sf::Color(25, 0, 0, 235));
    window.draw(titleBg);

    // gold divider line at the bottom of the banner
    sf::RectangleShape bannerLine({800.f, 2.f});
    bannerLine.setPosition({0.f, 58.f});
    bannerLine.setFillColor(sf::Color(140, 20, 20));
    window.draw(bannerLine);

    // "NECRON" title
    sf::Text bossName(font, "N E C R O N", 32);
    bossName.setFillColor(sf::Color(210, 40, 40));
    {
        sf::FloatRect b = bossName.getLocalBounds();
        bossName.setPosition({(800.f - b.size.x) / 2.f - b.position.x, 10.f});
    }
    window.draw(bossName);

    // Three boxes: I  II  III — current phase is bright, others dim
    const float boxW = 54.f, boxH = 26.f, boxGap = 6.f;
    const float boxStartX = 800.f - (boxW * 3.f + boxGap * 2.f) - 14.f;
    const float boxY = 16.f;

    const char* phaseLabels[3] = {"I", "II", "III"};
    // Active color escalates with phase
    sf::Color activeColors[3] = {
        sf::Color(170, 60, 60),   // phase 1 — dim red
        sf::Color(200, 100, 30),  // phase 2 — orange
        sf::Color(230, 20, 20)    // phase 3 — bright red
    };
    sf::Color dimColor(55, 20, 20, 200);

    for (int p = 0; p < 3; p++) {
        float px = boxStartX + p * (boxW + boxGap);
        bool active = (p + 1 == phase);

        sf::RectangleShape pBox({boxW, boxH});
        pBox.setPosition({px, boxY});
        pBox.setFillColor(active ? activeColors[p] : dimColor);
        pBox.setOutlineColor(sf::Color(140, 30, 30));
        pBox.setOutlineThickness(1.5f);
        window.draw(pBox);

        sf::Text pLabel(font, phaseLabels[p], 14);
        pLabel.setFillColor(active ? sf::Color(255, 220, 220) : sf::Color(110, 60, 60));
        sf::FloatRect pb = pLabel.getLocalBounds();
        pLabel.setPosition({px + (boxW - pb.size.x) / 2.f - pb.position.x,
                            boxY + (boxH - pb.size.y) / 2.f - pb.position.y});
        window.draw(pLabel);
    }

    // HP bar
    const float barY = 68.f, barH = 10.f;

    // Player HP bar (left)
    {
        const float barX = 50.f, barW = 260.f;
        float ratio = (float)player->getHp() / (float)player->getMaxHp();
        if (ratio < 0.f) ratio = 0.f;
        if (ratio > 1.f) ratio = 1.f;

        sf::RectangleShape track({barW, barH});
        track.setPosition({barX, barY});
        track.setFillColor(sf::Color(30, 60, 30, 200));
        window.draw(track);

        sf::RectangleShape fill({barW * ratio, barH});
        fill.setPosition({barX, barY});
        fill.setFillColor(ratio > 0.5f ? sf::Color(60, 180, 60) :
                          ratio > 0.25f ? sf::Color(200, 180, 30) : sf::Color(200, 40, 40));
        window.draw(fill);

        sf::Text hpLabel(font, "HP", 11);
        hpLabel.setFillColor(sf::Color(150, 255, 150));
        hpLabel.setPosition({barX, barY - 13.f});
        window.draw(hpLabel);
    }

    // Boss HP bar (right) with phase threshold markers at 66% and 33%
    {
        const float barX = 490.f, barW = 260.f;
        float ratio = (float)enemy->getHp() / (float)enemy->getMaxHp();
        if (ratio < 0.f) ratio = 0.f;
        if (ratio > 1.f) ratio = 1.f;

        sf::RectangleShape track({barW, barH});
        track.setPosition({barX, barY});
        track.setFillColor(sf::Color(60, 10, 10, 200));
        window.draw(track);

        sf::RectangleShape fill({barW * ratio, barH});
        fill.setPosition({barX, barY});
        fill.setFillColor(sf::Color(200, 30, 30));
        window.draw(fill);

        // Phase threshold markers
        // 66% line — transition to phase 2
        sf::RectangleShape mark66({2.f, barH + 4.f});
        mark66.setPosition({barX + barW * 0.66f - 1.f, barY - 2.f});
        mark66.setFillColor(sf::Color(230, 130, 30));
        window.draw(mark66);

        // 33% line — transition to phase 3
        sf::RectangleShape mark33({2.f, barH + 4.f});
        mark33.setPosition({barX + barW * 0.33f - 1.f, barY - 2.f});
        mark33.setFillColor(sf::Color(230, 20, 20));
        window.draw(mark33);

        sf::Text hpLabel(font, "HP", 11);
        hpLabel.setFillColor(sf::Color(255, 100, 100));
        hpLabel.setPosition({barX, barY - 13.f});
        window.draw(hpLabel);
    }

    // Player panel (left)
    sf::RectangleShape playerPanel({310.f, 180.f});
    playerPanel.setPosition({40.f, 88.f});
    playerPanel.setFillColor(sf::Color(0, 20, 0, 160));
    window.draw(playerPanel);

    playerText.setFillColor(sf::Color(100, 220, 100));
    playerText.setPosition({54.f, 96.f});
    window.draw(playerText);

    // Boss panel (right)
    sf::RectangleShape enemyPanel({310.f, 180.f});
    enemyPanel.setPosition({450.f, 88.f});
    enemyPanel.setFillColor(sf::Color(30, 0, 0, 160));
    window.draw(enemyPanel);

    enemyText.setFillColor(sf::Color(220, 80, 80));
    enemyText.setPosition({464.f, 96.f});
    window.draw(enemyText);

    // Combat log panel (bottom)
    sf::RectangleShape logPanel({700.f, 140.f});
    logPanel.setPosition({50.f, 278.f});
    logPanel.setFillColor(sf::Color(10, 0, 0, 180));
    logPanel.setOutlineColor(sf::Color(100, 20, 20));
    logPanel.setOutlineThickness(1.f);
    window.draw(logPanel);

    logText.setFillColor(sf::Color(220, 180, 100));
    logText.setPosition({60.f, 285.f});
    window.draw(logText);

    // Result text (center)
    resultText.setFillColor(sf::Color(220, 50, 50));
    resultText.setCharacterSize(28);
    {
        sf::FloatRect rb = resultText.getLocalBounds();
        resultText.setPosition({(800.f - rb.size.x) / 2.f - rb.position.x, 428.f});
    }
    window.draw(resultText);

    instructionText.setFillColor(sf::Color(170, 130, 130));
    {
        sf::FloatRect ib = instructionText.getLocalBounds();
        instructionText.setPosition({(800.f - ib.size.x) / 2.f - ib.position.x, 545.f});
    }
    window.draw(instructionText);

    // moves
    if (!combat->isBattleOver()) {
        std::vector<Move> moves = player->getMoves();
        const float moveBtnW = 155.f, moveBtnH = 50.f;
        const float moveBtnY = 470.f;
        const float startX = (800.f - (moveBtnW * 4 + 20.f * 3)) / 2.f;

        for (int i = 0; i < 4; i++) {
            float bx = startX + i * (moveBtnW + 20.f);

            // Dark crimson border
            sf::RectangleShape border({moveBtnW + 4.f, moveBtnH + 4.f});
            border.setPosition({bx - 2.f, moveBtnY - 2.f});
            border.setFillColor(sf::Color(120, 20, 20));
            window.draw(border);

            sf::RectangleShape bg({moveBtnW, moveBtnH});
            bg.setPosition({bx, moveBtnY});
            bg.setFillColor(sf::Color(35, 5, 5, 220));
            window.draw(bg);

            sf::Text moveLabel(font, moves[i].getName(), 15);
            moveLabel.setFillColor(sf::Color(220, 140, 60));
            sf::FloatRect flb = moveLabel.getLocalBounds();
            moveLabel.setPosition({bx + (moveBtnW - flb.size.x) / 2.f - flb.position.x,
                                   moveBtnY + (moveBtnH - flb.size.y) / 2.f - flb.position.y});
            window.draw(moveLabel);
        }
    }
}