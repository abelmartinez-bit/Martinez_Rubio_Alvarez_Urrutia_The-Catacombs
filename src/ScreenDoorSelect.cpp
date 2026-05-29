#include "../include/ScreenDoorSelect.h"
#include <string>

void drawDoorSelect(sf::RenderWindow& window, sf::Font& font,const sf::Sprite& bgSprite, const Floor* floor, const Player* player,
                    int floorNumber, Enemy* previewEnemies[3], const sf::Sprite& doorSprite, const sf::Sprite& doorCorruptSprite,
                    const std::array<std::string,3>& statOverrides) {

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

    sf::Text floorTitle(font, "FLOOR " + std::to_string(floorNumber) + "  /  10", 44);
    sf::FloatRect ftb = floorTitle.getLocalBounds();

    sf::RectangleShape overlay({600.f, 128.f + ftb.size.y });
    overlay.setPosition({100.f, 20.f});
    overlay.setFillColor(sf::Color(0, 0, 0, 155));
    window.draw(overlay);

    // Floor title
    floorTitle.setFillColor(sf::Color(160, 120, 50));
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
    
    // Door cards
    std::vector<Door> doors = floor->getDoors();
    const float DOOR_W = 200.f;
    const float DOOR_H = 200.f;
    const float GAP = 50.f;
    const float TOTAL_W = DOOR_W * 3.f + GAP * 2.f;
    const float START_X = 815.f - TOTAL_W - DOOR_W / 3.f; // start a half door width from the left edge, so that the 4th door is also half a door width from the right edge
    const float DOOR_TOP = 320.f;

    for (int i = 0; i < static_cast<int>(doors.size()); i++) {
        float dx = START_X + i * (DOOR_W + GAP);
        bool corrupt = doors[i].isCorrupt();

        // Door image fills the card
        sf::Sprite selectedDoorSprite = corrupt ? doorCorruptSprite : doorSprite;
        sf::FloatRect doorBounds = selectedDoorSprite.getLocalBounds();
        selectedDoorSprite.setScale({DOOR_W / doorBounds.size.x, DOOR_H / doorBounds.size.y});
        selectedDoorSprite.setPosition({dx, DOOR_TOP});
        window.draw(selectedDoorSprite);

        // Gradient overlays, dark band at top and bottom for text legibility
        sf::RectangleShape topBand({DOOR_W - 36.f, 48.f});
        topBand.setPosition({dx + 18.f, DOOR_TOP - 46.f});
        topBand.setFillColor(sf::Color(0, 0, 0, 150));
        window.draw(topBand);

        sf::RectangleShape botBand({DOOR_W - 36.f, 36.f});
        botBand.setPosition({dx + 18.f, DOOR_TOP + DOOR_H - 36.f});
        botBand.setFillColor(sf::Color(0, 0, 0, 150));
        window.draw(botBand);

        // "Door X" — top of card
        sf::Text doorNum(font, "Door " + std::to_string(i + 1), 26);
        doorNum.setFillColor(sf::Color(210, 160, 60));
        sf::FloatRect dnb = doorNum.getLocalBounds();
        doorNum.setPosition({dx + (DOOR_W - dnb.size.x) / 2.f, DOOR_TOP - 40.f});
        window.draw(doorNum);

        // Stat hint — bottom of card
        std::string statStr;
        if (!statOverrides[i].empty()) statStr = statOverrides[i];
        else statStr = (i < 3 && previewEnemies[i]) ? highestStatName(previewEnemies[i]) : "ATK";
        sf::Text statHint(font, statStr + " oriented", 14);
        statHint.setFillColor(sf::Color(200, 170, 110));
        sf::FloatRect shb = statHint.getLocalBounds();
        statHint.setPosition({dx + (DOOR_W - shb.size.x) / 2.f, DOOR_TOP + DOOR_H - 28.f});
        window.draw(statHint);

        // "Press X" — below card
        sf::Text keyHint(font, "Press  " + std::to_string(i + 1), 18);
        keyHint.setFillColor(sf::Color::White);
        sf::FloatRect kb = keyHint.getLocalBounds();
        keyHint.setPosition({dx + (DOOR_W - kb.size.x) / 2.f - kb.position.x,
                             DOOR_TOP + DOOR_H + 14.f});
        window.draw(keyHint);
    }
}
