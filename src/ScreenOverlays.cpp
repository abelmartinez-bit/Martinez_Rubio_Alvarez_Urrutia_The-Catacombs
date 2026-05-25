#include "../include/ScreenOverlays.h"
#include <string>

//dibujos para pop ups degame over, win, level up y move select
void drawGameOver(sf::RenderWindow& window, sf::Font& font) {
    sf::RectangleShape overlay({800.f, 600.f});
    overlay.setFillColor(sf::Color(0, 0, 0, 200));
    window.draw(overlay);

    sf::Text title(font, "GAME OVER", 72);
    title.setFillColor(sf::Color(200, 40, 40));
    sf::FloatRect tb = title.getLocalBounds();
    title.setPosition({(800.f - tb.size.x) / 2.f - tb.position.x, 180.f});
    window.draw(title);

    sf::Text sub(font, "The Catacombs claimed another soul.", 22);
    sub.setFillColor(sf::Color(180, 130, 130));
    sf::FloatRect sb = sub.getLocalBounds();
    sub.setPosition({(800.f - sb.size.x) / 2.f - sb.position.x, 290.f});
    window.draw(sub);

    sf::Text hint(font, "Press  R  to return to the main menu.", 19);
    hint.setFillColor(sf::Color(150, 150, 150));
    sf::FloatRect hb = hint.getLocalBounds();
    hint.setPosition({(800.f - hb.size.x) / 2.f - hb.position.x, 380.f});
    window.draw(hint);
}

void drawWin(sf::RenderWindow& window, sf::Font& font) {
    sf::RectangleShape overlay({800.f, 600.f});
    overlay.setFillColor(sf::Color(0, 0, 0, 190));
    window.draw(overlay);

    sf::Text title(font, "YOU ESCAPED!", 66);
    title.setFillColor(sf::Color(210, 160, 60));
    sf::FloatRect tb = title.getLocalBounds();
    title.setPosition({(800.f - tb.size.x) / 2.f - tb.position.x, 170.f});
    window.draw(title);

    sf::Text sub(font, "You conquered all 10 floors of The Catacombs.", 20);
    sub.setFillColor(sf::Color(180, 160, 100));
    sf::FloatRect sb = sub.getLocalBounds();
    sub.setPosition({(800.f - sb.size.x) / 2.f - sb.position.x, 285.f});
    window.draw(sub);

    sf::Text hint(font, "Press  R  to return to the main menu.", 19);
    hint.setFillColor(sf::Color(150, 150, 150));
    sf::FloatRect hb = hint.getLocalBounds();
    hint.setPosition({(800.f - hb.size.x) / 2.f - hb.position.x, 375.f});
    window.draw(hint);
}

void drawLevelUp(sf::RenderWindow& window, sf::Font& font, Player* player) {
    sf::RectangleShape overlay({800.f, 600.f});
    overlay.setFillColor(sf::Color(0, 0, 0, 210));
    window.draw(overlay);

    sf::Text title(font, "LEVEL UP!", 52);
    title.setFillColor(sf::Color(210, 160, 60));
    sf::FloatRect tb = title.getLocalBounds();
    title.setPosition({(800.f - tb.size.x) / 2.f - tb.position.x, 40.f});
    window.draw(title);

    sf::Text lvlText(font, "Now level " + std::to_string(player->getLevel()), 22);
    lvlText.setFillColor(sf::Color(160, 120, 50));
    sf::FloatRect lb = lvlText.getLocalBounds();
    lvlText.setPosition({(800.f - lb.size.x) / 2.f - lb.position.x, 110.f});
    window.draw(lvlText);

    sf::Text hint(font, "Click a stat to boost:", 18);
    hint.setFillColor(sf::Color(180, 180, 180));
    sf::FloatRect hb = hint.getLocalBounds();
    hint.setPosition({(800.f - hb.size.x) / 2.f - hb.position.x, 148.f});
    window.draw(hint);

    struct StatBtn { std::string label; float y; };
    StatBtn btns[6] = {
        {"1.  HP     +20", 195.f},
        {"2.  ATK    +10", 258.f},
        {"3.  DEF    +10", 321.f},
        {"4.  SP.ATK +10", 384.f},
        {"5.  SP.DEF +10", 447.f},
        {"6.  SPD    +6",  510.f},
    };

    const float luBtnW = 260.f, luBtnH = 48.f;
    const float luBtnX = (800.f - luBtnW) / 2.f;

    for (int i = 0; i < 6; i++) {
        sf::RectangleShape border({luBtnW + 4.f, luBtnH + 4.f});
        border.setPosition({luBtnX - 2.f, btns[i].y - 2.f});
        border.setFillColor(sf::Color(120, 80, 20));
        window.draw(border);

        sf::RectangleShape bg({luBtnW, luBtnH});
        bg.setPosition({luBtnX, btns[i].y});
        bg.setFillColor(sf::Color(20, 12, 5, 210));
        window.draw(bg);

        sf::Text label(font, btns[i].label, 22);
        label.setFillColor(sf::Color(210, 160, 60));
        sf::FloatRect flb = label.getLocalBounds();
        label.setPosition({luBtnX + (luBtnW - flb.size.x) / 2.f - flb.position.x,
                           btns[i].y + (luBtnH - flb.size.y) / 2.f - flb.position.y});
        window.draw(label);
    }
}

void drawMoveSelect(sf::RenderWindow& window, sf::Font& font, Player* player) {
    sf::RectangleShape overlay({800.f, 600.f});
    overlay.setFillColor(sf::Color(0, 0, 0, 210));
    window.draw(overlay);

    sf::Text title(font, "NEW MOVE RELIC!", 44);
    title.setFillColor(sf::Color(210, 160, 60));
    sf::FloatRect tb = title.getLocalBounds();
    title.setPosition({(800.f - tb.size.x) / 2.f - tb.position.x, 35.f});
    window.draw(title);

    sf::Text newMove(font, "New move: " + player->getPendingMove().getName() +
                     "  (Power: " + std::to_string(player->getPendingMove().getPower()) + ")", 20);
    newMove.setFillColor(sf::Color(100, 220, 100));
    sf::FloatRect nb = newMove.getLocalBounds();
    newMove.setPosition({(800.f - nb.size.x) / 2.f - nb.position.x, 100.f});
    window.draw(newMove);

    sf::Text hint(font, "Click a move slot to replace:", 18);
    hint.setFillColor(sf::Color(180, 180, 180));
    sf::FloatRect hb = hint.getLocalBounds();
    hint.setPosition({(800.f - hb.size.x) / 2.f - hb.position.x, 140.f});
    window.draw(hint);

    std::vector<Move> moves = player->getMoves();
    const float msBtnW = 320.f, msBtnH = 55.f;
    const float msBtnX = (800.f - msBtnW) / 2.f;
    const float msStartY = 185.f;

    for (int i = 0; i < 4; i++) {
        float by = msStartY + i * (msBtnH + 18.f);

        sf::RectangleShape border({msBtnW + 4.f, msBtnH + 4.f});
        border.setPosition({msBtnX - 2.f, by - 2.f});
        border.setFillColor(sf::Color(120, 80, 20));
        window.draw(border);

        sf::RectangleShape bg({msBtnW, msBtnH});
        bg.setPosition({msBtnX, by});
        bg.setFillColor(sf::Color(20, 12, 5, 210));
        window.draw(bg);

        sf::Text label(font, std::to_string(i + 1) + ".  " + moves[i].getName() +
                       "  (Power: " + std::to_string(moves[i].getPower()) + ")", 20);
        label.setFillColor(sf::Color(210, 160, 60));
        sf::FloatRect flb = label.getLocalBounds();
        label.setPosition({msBtnX + (msBtnW - flb.size.x) / 2.f - flb.position.x,
                           by + (msBtnH - flb.size.y) / 2.f - flb.position.y});
        window.draw(label);
    }
}