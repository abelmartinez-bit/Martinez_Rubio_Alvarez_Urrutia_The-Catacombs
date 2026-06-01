#include "../include/ScreenCombat.h"
#include "../include/FinalBoss.h"
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

//nueva funcion para dibujar botones, con efecto hover
void drawPanelBtn(sf::RenderWindow& window, sf::Font& font,
                         float x, float y, float w, float h,
                         const std::string& label, bool hovered)
{
    sf::Color border = hovered ? sf::Color(210, 160, 60) : sf::Color(100, 65, 15);
    sf::Color bg     = hovered ? sf::Color(55, 35, 10, 230) : sf::Color(18, 10, 3, 220);

    sf::RectangleShape bdr({w + 4.f, h + 4.f});
    bdr.setPosition({x - 2.f, y - 2.f});
    bdr.setFillColor(border);
    window.draw(bdr);

    sf::RectangleShape box({w, h});
    box.setPosition({x, y});
    box.setFillColor(bg);
    window.draw(box);

    sf::Text txt(font, label, 18);
    txt.setFillColor(hovered ? sf::Color(255, 205, 100) : sf::Color(210, 160, 60));
    sf::FloatRect b = txt.getLocalBounds();
    txt.setPosition({x + (w - b.size.x) / 2.f - b.position.x,
                     y + (h - b.size.y) / 2.f - b.position.y});
    window.draw(txt);
}

//nueva funcion para dibujar botones de moves, mas que nada para el hint del tipo/efecto
void drawMoveBtn(sf::RenderWindow& window, sf::Font& font,
                 float x, float y, float w, float h,
                 const Move& move, bool hovered)
{
    sf::Color border = hovered ? sf::Color(210, 160, 60) : sf::Color(100, 65, 15);
    sf::Color bg     = hovered ? sf::Color(55, 35, 10, 230) : sf::Color(18, 10, 3, 220);

    sf::RectangleShape bdr({w + 4.f, h + 4.f});
    bdr.setPosition({x - 2.f, y - 2.f});
    bdr.setFillColor(border);
    window.draw(bdr);

    sf::RectangleShape box({w, h});
    box.setPosition({x, y});
    box.setFillColor(bg);
    window.draw(box);

    // nombre del move centrado
    sf::Text txt(font, move.getName(), 16);
    txt.setFillColor(hovered ? sf::Color(255, 205, 100) : sf::Color(210, 160, 60));
    sf::FloatRect b = txt.getLocalBounds();
    txt.setPosition({x + (w - b.size.x) / 2.f - b.position.x,
                     y + (h - b.size.y) / 2.f - b.position.y - 10.f});
    window.draw(txt);

    // hint del tipo/efecto abajo del nombre
    std::string hint;
    if (move.getType() == MoveType::PHYSICAL) hint = "PHYSICAL";
    else if (move.getType() == MoveType::SPECIAL) hint = "SPECIAL";
    else {
        switch (move.getEffect()) {
            case MoveEffect::HEAL:      hint = "HEAL";      break;
            case MoveEffect::BLOCK:     hint = "BLOCK";     break;
            case MoveEffect::ATK_UP:    hint = "ATK UP";    break;
            case MoveEffect::ATK_DOWN:  hint = "ATK DOWN";  break;
            case MoveEffect::DEF_UP:    hint = "DEF UP";    break;
            case MoveEffect::DEF_DOWN:  hint = "DEF DOWN";  break;
            case MoveEffect::SPATK_UP:  hint = "SP.ATK UP"; break;
            case MoveEffect::SPATK_DOWN:hint = "SP.ATK DOWN";break;
            case MoveEffect::SPDEF_UP:  hint = "SP.DEF UP"; break;
            case MoveEffect::SPDEF_DOWN:hint = "SP.DEF DOWN";break;
            case MoveEffect::SPD_UP:    hint = "SPD UP";    break;
            case MoveEffect::SPD_DOWN:  hint = "SPD DOWN";  break;
            default: hint = "EFFECT"; break;
        }
    }

    sf::Text hintTxt(font, hint, 12);
    hintTxt.setFillColor(sf::Color(160, 130, 80));
    sf::FloatRect hb = hintTxt.getLocalBounds();
    hintTxt.setPosition({x + (w - hb.size.x) / 2.f - hb.position.x,
                         y + (h - hb.size.y) / 2.f - hb.position.y + 12.f});
    window.draw(hintTxt);
}

//nueva funcion para dibujar pop ups al hacer hover 
void drawPopup(sf::RenderWindow& window, sf::Font& font,
                      float x, float y, float w, float h,
                      const std::string& title, const std::vector<std::string>& lines)
{
    // borde dorado
    sf::RectangleShape border({w + 4.f, h + 4.f});
    border.setPosition({x - 2.f, y - 2.f});
    border.setFillColor(sf::Color(120, 80, 20));
    window.draw(border);

    // fondo oscuro
    sf::RectangleShape bg({w, h});
    bg.setPosition({x, y});
    bg.setFillColor(sf::Color(15, 8, 2, 235));
    window.draw(bg);

    // titulo del popup
    sf::Text ttl(font, title, 16);
    ttl.setFillColor(sf::Color(210, 160, 60));
    sf::FloatRect tb = ttl.getLocalBounds();
    ttl.setPosition({x + (w - tb.size.x) / 2.f - tb.position.x, y + 8.f});
    window.draw(ttl);

    // linea separadora
    sf::RectangleShape line({w - 20.f, 1.f});
    line.setPosition({x + 10.f, y + 30.f});
    line.setFillColor(sf::Color(100, 65, 15));
    window.draw(line);

    // lineas de contenido
    float ly = y + 38.f;
    for (const std::string& l : lines) {
        sf::Text row(font, l, 14);
        row.setFillColor(sf::Color(210, 195, 165));
        row.setPosition({x + 12.f, ly});
        window.draw(row);
        ly += 20.f;
    }
}


//funcion principal, con cambios para dibujar la pantalla de combate, tipo pokemon y entrega recompensa si termino
void drawCombat(sf::RenderWindow& window, sf::Font& font, Player* player, Enemy* enemy, CombatEngine* combat, 
                sf::Text& playerText, sf::Text& enemyText, sf::Text& logText, sf::Text& resultText, sf::Text& instructionText, 
                bool& rewardGiven, const sf::Sprite& combatSprite, Floor* floor, bool isMiniBoss, 
                std::string& rewardDescription, bool showingMoves, sf::Vector2f mousePos) {

    //ya no mostramos stats de player, solo la caja con nombre y hp bar, stats en el popup
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
    } 
    else {
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

    sf::Text pName(font, player->getName() + "  -  Lvl." + std::to_string(player->getLevel()), 20);
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
    //ahora sin draw de instructionText, porque lo ponemos dentro del panel inferior

    //Panel inferior con log y botones, tipo pokemon
    const float panelY  = 440.f;
    const float panelH  = 185.f;
    const float logW    = 390.f;
    const float btnAreaX = logW;
    const float btnAreaW = 800.f - logW;

    // fondo del panel
    sf::RectangleShape panelBg({800.f, panelH});
    panelBg.setPosition({0.f, panelY});
    panelBg.setFillColor(sf::Color(12, 7, 2, 230));
    window.draw(panelBg);

    // borde superior
    sf::RectangleShape panelTop({800.f, 3.f});
    panelTop.setPosition({0.f, panelY});
    panelTop.setFillColor(sf::Color(120, 80, 20));
    window.draw(panelTop);

    // divisor vertical entre log y botones
    sf::RectangleShape divider({2.f, panelH - 10.f});
    divider.setPosition({logW, panelY + 5.f});
    divider.setFillColor(sf::Color(90, 55, 10));
    window.draw(divider);

    // Nuevo log dentro del panel
    if (combat->isBattleOver()) {
        std::string endMsg = player->isAlive()
            ? "Victory!\nPress ENTER to continue."
            : "Defeated...\nPress ENTER to continue.";
        sf::Text endTxt(font, endMsg, 20);
        endTxt.setFillColor(player->isAlive() ? sf::Color(100, 220, 100)
                                            : sf::Color(210, 80, 80));
        endTxt.setPosition({18.f, panelY + 20.f});
        window.draw(endTxt);
    } 
    else {
        std::string logStr = combat->getCombatLog();
        if (logStr.empty()) logStr = "What will " + player->getName() + " do?";

        //Separar el log en lineas por \n y por puntos, y limites de caracteres, para que quepan en la caja del panel
        std::vector<std::string> logLines;
        std::string line;
        const int maxChars = 38;

        for (char c : logStr) {
            if (c == '\n') {
                if (!line.empty()) logLines.push_back(line);
                line = "";
            } 
            else {
                line += c;
                if (c == '.') { //si se llega a un punto, cortar la linea ahi para que no se amontonen las oraciones
                    logLines.push_back(line);
                    line = "";
                } 
                else if ((int)line.size() >= maxChars) { //si se pasa el limite de caracteres, cortar la linea
                    int cutPos = line.rfind(' ');
                    if (cutPos != (int)std::string::npos) {
                        logLines.push_back(line.substr(0, cutPos));
                        line = line.substr(cutPos + 1);
                    } else {
                        logLines.push_back(line);
                        line = "";
                    }
                }
            }
        }
        if (!line.empty()) logLines.push_back(line);

        // Tomar solo las ultimas 6 lineas para que quepan en la caja
        int startLine = std::max(0, (int)logLines.size() - 6);
        float ly = panelY + 16.f;
        for (int i = startLine; i < (int)logLines.size(); i++) {
            sf::Text logTxt(font, logLines[i], 16);
            logTxt.setFillColor(sf::Color(210, 195, 165));
            logTxt.setPosition({18.f, ly});
            window.draw(logTxt);
            ly += 26.f;
        }
    }

    //nuevo panel de botones, abajo derecha, moves, stats, relics y potions, con hovers para pop ups
    if (!combat->isBattleOver()) {
        const float bW = 170.f, bH = 58.f, bGap = 8.f;
        const float bStartX = btnAreaX + (btnAreaW - bW * 2.f - bGap) / 2.f;
        const float bRow1Y  = panelY + 20.f;
        const float bRow2Y  = panelY + 20.f + bH + bGap;
        float bx[4] = { bStartX, bStartX+bW+bGap, bStartX, bStartX+bW+bGap };
        float by[4] = { bRow1Y, bRow1Y, bRow2Y, bRow2Y };

        if (showingMoves) {
            // modo MOVES: mostrar los 4 moves del jugador
            std::vector<Move> moves = player->getMoves();
            for (int i = 0; i < 4; i++) {
                bool hov = (mousePos.x >= bx[i] && mousePos.x <= bx[i]+bW &&
                            mousePos.y >= by[i] && mousePos.y <= by[i]+bH);
                drawMoveBtn(window, font, bx[i], by[i], bW, bH, moves[i], hov); //ahora su propia funcion 
            }
            // boton BACK
            const float backW = 100.f, backH = 30.f;
            float backX = btnAreaX + (btnAreaW - backW) / 2.f;
            float backY = panelY + panelH - backH - 20.f;
            bool backHov = (mousePos.x >= backX && mousePos.x <= backX+backW &&
                            mousePos.y >= backY && mousePos.y <= backY+backH);
            drawPanelBtn(window, font, backX, backY, backW, backH, "BACK", backHov);

        } else {
            // menu principal: MOVES / STATS / RELICS / POTIONS
            const char* labels[4] = { "MOVES", "STATS", "RELICS", "POTIONS" };
            for (int i = 0; i < 4; i++) {
                bool hov = (mousePos.x >= bx[i] && mousePos.x <= bx[i]+bW &&
                            mousePos.y >= by[i] && mousePos.y <= by[i]+bH);
                drawPanelBtn(window, font, bx[i], by[i], bW, bH, labels[i], hov);

                // popups de hover
                if (hov && i > 0) {
                    const float popW = 320.f;
                    float popX = bx[i] - popW - 10.f;
                    if (popX < 5.f) popX = bx[i] + bW + 10.f;
                    float popY, popH;

                    if (i == 1) { // STATS
                        std::vector<std::string> lines = {
                            "ATK:    " + std::to_string(player->getAtk()),
                            "DEF:    " + std::to_string(player->getDef()),
                            "SP.ATK: " + std::to_string(player->getSpAtk()),
                            "SP.DEF: " + std::to_string(player->getSpDef()),
                            "SPD:    " + std::to_string(player->getSpd()),
                            "EXP: " + std::to_string(player->getExp()) +
                                " / " + std::to_string(player->getExpToNextLevel()),
                        };
                        popH = 38.f + lines.size() * 20.f + 10.f;
                        popY = panelY - popH - 6.f;
                        drawPopup(window, font, popX, popY, popW, popH, "STATS", lines);
                    }
                    else if (i == 2) { // RELICS
                        std::vector<Relic*> relics = player->getRelics();
                        std::vector<std::string> lines;
                        if (relics.empty()) lines.push_back("No relics yet.");
                        for (Relic* r : relics)
                            lines.push_back(r->getName() + ": " + r->getDescription());
                        popH = 38.f + std::max((int)lines.size(), 1) * 20.f + 10.f;
                        if (popH > 300.f) popH = 300.f;
                        popY = panelY - popH - 6.f;
                        popX = (800.f - popW) / 2.f;
                        drawPopup(window, font, popX, popY, popW, popH, "RELICS", lines);
                    }
                    else if (i == 3) { // POTIONS
                        std::vector<Potion> potions = player->getPotions();
                        std::vector<std::string> lines;
                        lines.push_back("Slots: " + std::to_string(potions.size()) +
                                        " / " + std::to_string(player->getPotionSlots()));
                        if (potions.empty()) lines.push_back("No potions.");
                        for (Potion& p : potions)
                            lines.push_back("- " + p.getName());
                        lines.push_back("(P: usar primera pocion)");
                        popH = 38.f + lines.size() * 20.f + 10.f;
                        popY = panelY - popH - 6.f;
                        drawPopup(window, font, popX, popY, popW, popH, "POTIONS", lines);
                    }
                }
            }
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