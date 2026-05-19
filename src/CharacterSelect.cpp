#include "CharacterSelect.h"
#include "Potion.h"

int handleCharacterSelectClick(sf::Vector2f mouse, CharacterCard cards[4],
                               float cardW, float cardH, float cardTop) {
    for (int i = 0; i < 4; i++) {
        float cardX = cards[i].centerX - cardW / 2.f;
        if (mouse.x >= cardX && mouse.x <= cardX + cardW &&
            mouse.y >= cardTop && mouse.y <= cardTop + cardH) {
            return i; // devuelve el indice de la carta clickeada
        }
    }
    return -1; // ninguna carta clickeada
}

bool handleCharacterSelectKey(sf::Keyboard::Key key, int selectedCard,
                              CharacterCard cards[4],
                              Player*& player, Enemy*& enemy, CombatEngine*& combat) {
    if (key == sf::Keyboard::Key::C && selectedCard != -1) {
        // Crear el jugador con el personaje elegido
        CharacterCard& chosen = cards[selectedCard];
        player = new Player(chosen.name, chosen.hp, chosen.atk, chosen.def,
                            chosen.spAtk, chosen.spDef, chosen.spd, chosen.type);
        player->addPotion(Potion::createRandomPotion()); // pociones random para probar
        player->initializeMovePool();
        player->selectMovesFromPool();

        // Crear el enemigo y el motor de combate
        enemy = new Enemy("Sentinel", EnemyType::SENTINEL, 50, 10, 10, 10, 15, 8);
        enemy->initialize();
        combat = new CombatEngine(player, enemy);

        return true; // cambiar a COMBAT
    }
    return false;
}

void drawCharacterSelect(sf::RenderWindow& window, sf::Font& font,
                         const sf::Sprite& csSprite,
                         CharacterCard cards[4], int selectedCard,
                         float cardW, float cardH, float cardTop) {

    //imagen de seleccion de personaje
    window.draw(csSprite);

    // Dibujar nombre y resaltado de cada carta
    for (int i = 0; i < 4; i++) {
        float cardX = cards[i].centerX - cardW / 2.f;

        // Si esta seleccionada, dibujar un borde dorado encima de la carta
        if (selectedCard == i) {
            sf::RectangleShape highlight({cardW, cardH});
            highlight.setPosition({cardX, cardTop});
            highlight.setFillColor(sf::Color(0, 0, 0, 0)); //transparente
            highlight.setOutlineColor(sf::Color(210, 160, 60));
            highlight.setOutlineThickness(4.f);
            window.draw(highlight);
        }

        // Nombre del personaje debajo de la carta
        sf::Text nameText(font, cards[i].name, 20);
        nameText.setFillColor(sf::Color::Black); //ahora negro para que se vea mejor con el fondo claro
        sf::FloatRect nb = nameText.getLocalBounds();
        nameText.setPosition({
            cards[i].centerX - nb.size.x / 2.f - nb.position.x,
            //ahora para que quede en el slot de nombre de la carta y no abajo
            cardTop + cardH + -40.f
        });
        window.draw(nameText);
    }

    // Texto de seleccion
    sf::Text tempText(font, "SELECCIONA TU PERSONAJE", 26);
    tempText.setFillColor(sf::Color::Black);
    sf::FloatRect tb = tempText.getLocalBounds();
    tempText.setPosition({
        (800.f - tb.size.x) / 2.f - tb.position.x,
        545.f
    });
    window.draw(tempText);

    // Hint: cambia segun si ya eligio o no
    sf::Text hintText(font, selectedCard == -1 ? "Haz click en un personaje" : "Presiona C para continuar", 18);
    hintText.setFillColor(sf::Color::Black);
    sf::FloatRect hb = hintText.getLocalBounds();
    hintText.setPosition({
        (800.f - hb.size.x) / 2.f - hb.position.x,
        575.f
    });
    window.draw(hintText);
}
