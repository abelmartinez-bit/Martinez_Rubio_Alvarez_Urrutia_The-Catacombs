#include "../include/ScreenDoorSelect.h"
#include <string>

void drawDoorSelect(sf::RenderWindow& window, sf::Font& font,const sf::Sprite& bgSprite, const Floor* floor, const Player* player,
                    int floorNumber, Enemy* previewEnemies[3], const std::array<std::string,3>& statOverrides) {

    if (!floor || !player) return;

    // lambda ya que nunca se usa fuera de este caso, auto para dejar que el compilador infiera el tipo de retorno
    // toma un puntero a enemigo para que si es null no tire error y asuma que el stat mas alto es atk (caso donde sea puerta de evento)
    // -> string porq en nuestros retornos no especificamos el tipo de dato
    auto highestStatName = [](const Enemy* e) -> std::string {
        if (!e) return "ATK";

        int h = e->getHighestStat();
        if (h == e->getAtk())   return "ATK";
        if (h == e->getDef())   return "DEF";
        if (h == e->getSpAtk()) return "SP.ATK";
        if (h == e->getSpDef()) return "SP.DEF";
        return "SPD";
    };

    window.draw(bgSprite);

    sf::RectangleShape overlay({800.f, 600.f});
    overlay.setFillColor(sf::Color(0, 0, 0, 155));
    window.draw(overlay);

    // Floor title
    sf::Text floorTitle(font, "FLOOR " + std::to_string(floorNumber) + "  /  10", 44);
    floorTitle.setFillColor(sf::Color(210, 160, 60));
    sf::FloatRect ftb = floorTitle.getLocalBounds();
    floorTitle.setPosition({(800.f - ftb.size.x) / 2.f - ftb.position.x, 38.f});
    window.draw(floorTitle);

    // title decor
    sf::RectangleShape divLine({480.f, 2.f});
    divLine.setFillColor(sf::Color(120, 80, 20));
    divLine.setPosition({160.f, 100.f});
    window.draw(divLine);

    // subtitle
    sf::Text subtitle(font, "Choose your path  |  Press  1   2   3", 19);
    subtitle.setFillColor(sf::Color(160, 120, 50));
    sf::FloatRect stb = subtitle.getLocalBounds();
    subtitle.setPosition({(800.f - stb.size.x) / 2.f - stb.position.x, 113.f});
    window.draw(subtitle);

    // Player stats
    std::string statsStr =
        player->getName() +
        "   HP: " + std::to_string(player->getHp()) + "/" + std::to_string(player->getMaxHp()) +
        "   LVL: " + std::to_string(player->getLevel()) +
        "   Relics: " + std::to_string(player->getRelics().size()) +
        "   Potions: " + std::to_string(player->getPotions().size()) + "/" + std::to_string(player->getPotionSlots());

    sf::Text playerStats(font, statsStr, 16);
    playerStats.setFillColor(sf::Color(100, 200, 100));
    sf::FloatRect psb = playerStats.getLocalBounds();
    playerStats.setPosition({(800.f - psb.size.x) / 2.f - psb.position.x, 148.f});
    window.draw(playerStats);

    // Doors cards
    std::vector<Door> doors = floor->getDoors();
    const float DOOR_W = 190.f;
    const float DOOR_H = 140.f;
    const float GAP = 35.f;
    const float TOTAL_W = DOOR_W * 3.f + GAP * 2.f;
    const float START_X = (800.f - TOTAL_W) / 2.f;
    const float DOOR_TOP = 220.f;

    for (int i = 0; i < static_cast<int>(doors.size()); i++) {
        float dx = START_X + i * (DOOR_W + GAP);

        bool corrupt = doors[i].isCorrupt();

        // Color del borde segun tipo
        sf::Color borderColor = corrupt ? sf::Color(160, 40, 40) : sf::Color(120, 80, 20);

        sf::RectangleShape border({DOOR_W + 4.f, DOOR_H + 4.f});
        border.setPosition({dx - 2.f, DOOR_TOP - 2.f});
        border.setFillColor(borderColor);
        window.draw(border);

        sf::RectangleShape bg({DOOR_W, DOOR_H});
        bg.setPosition({dx, DOOR_TOP});
        bg.setFillColor(sf::Color(20, 12, 5, 215));
        window.draw(bg);

        // Numero de puerta
        sf::Text doorNum(font, "Door " + std::to_string(i + 1), 24);
        doorNum.setFillColor(sf::Color(210, 160, 60));
        sf::FloatRect dnb = doorNum.getLocalBounds();
        doorNum.setPosition({dx + (DOOR_W - dnb.size.x) / 2.f - dnb.position.x, DOOR_TOP + 16.f});
        window.draw(doorNum);

        // Tipo de puerta
        std::string typeStr = corrupt ? "[CORRUPT]" : "[NORMAL]";
        sf::Color   typeCol = corrupt ? sf::Color(220, 80, 80) : sf::Color(160, 160, 160);

        sf::Text typeLabel(font, typeStr, 17);
        typeLabel.setFillColor(typeCol);
        sf::FloatRect tlb = typeLabel.getLocalBounds();
        typeLabel.setPosition({dx + (DOOR_W - tlb.size.x) / 2.f - tlb.position.x, DOOR_TOP + 58.f});
        window.draw(typeLabel);

        // corrupt hint indicator
        std::string hintStr = corrupt ? "Mini-Boss!" : "Enemy"; // aqui falta algun indicator del highest stat (excluding HP) del enemy
        sf::Text hint(font, hintStr, 14);
        hint.setFillColor(sf::Color(130, 110, 80));
        sf::FloatRect hb = hint.getLocalBounds();
        hint.setPosition({dx + (DOOR_W - hb.size.x) / 2.f - hb.position.x, DOOR_TOP + 88.f});
        window.draw(hint);

        // Tecla a presionar p/ hint (resaltada)
        sf::Text keyHint(font, "Press  " + std::to_string(i + 1), 18);
        keyHint.setFillColor(sf::Color::White);
        sf::FloatRect kb = keyHint.getLocalBounds();
        keyHint.setPosition({dx + (DOOR_W - kb.size.x) / 2.f - kb.position.x, DOOR_TOP + DOOR_H + 16.f});
        window.draw(keyHint);

        std::string statStr;
        if (!statOverrides[i].empty()) statStr = statOverrides[i];
        else statStr = (i < 3 && previewEnemies[i]) ? highestStatName(previewEnemies[i]) : " ATK";
        sf::Text statHint(font, statStr + " oriented", 13);
        statHint.setFillColor(sf::Color(180, 140, 100));
        sf::FloatRect shb = statHint.getLocalBounds();
        statHint.setPosition({dx + (DOOR_W - shb.size.x) / 2.f - shb.position.x, DOOR_TOP + DOOR_H - 18.f});
        window.draw(statHint);
    }
}