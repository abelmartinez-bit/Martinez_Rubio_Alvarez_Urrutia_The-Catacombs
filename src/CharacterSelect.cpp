#include "../include/CharacterSelect.h"
#include "../include/Potion.h"

int handleCharacterSelectClick(sf::Vector2f mouse, CharacterCard cards[4], float cardW, float cardH, float cardTop) {
    for (int i = 0; i < 4; i++) {
        float cardX = cards[i].centerX - cardW / 2.f;
        if (mouse.x >= cardX && mouse.x <= cardX + cardW && mouse.y >= cardTop && mouse.y <= cardTop + cardH) {
            return i; // devuelve el indice de la carta clickeada
        }
    }
    return -1; // ninguna carta clickeada
}

bool handleCharacterSelectKey(sf::Keyboard::Key key, int selectedCard, CharacterCard cards[4], Player*& player) {
    if (key == sf::Keyboard::Key::C && selectedCard != -1) {
        // Crear el jugador con el personaje elegido
        CharacterCard& chosen = cards[selectedCard];
        player = new Player(chosen.name, chosen.hp, chosen.atk, chosen.def, chosen.spAtk, chosen.spDef, chosen.spd, chosen.type);
        player->addPotion(Potion::createRandomPotion()); // pociones random para probar
        player->initializeMovePool();
        player->selectMovesFromPool();

        // Enemy y CombatEngine ya no se crean aqui.
        // GameLoop::selectDoor() los crea cuando el jugador elige una puerta.
        return true;
    }
    return false;
}

void drawCharacterSelect(sf::RenderWindow& window, sf::Font& font, const sf::Sprite& csSprite, CharacterCard cards[4], int selectedCard,
                         float cardW, float cardH, float cardTop, float time) {

    //imagen de seleccion de personaje
    window.draw(csSprite);

    // Dibujar nombre, HP y resaltado de cada carta
    for (int i = 0; i < 4; i++) {
        float cardX = cards[i].centerX - cardW / 2.f;

        // Calculate alpha usando seno
        // * 5.f +velocidad del parpadeo
        float sineWave = std::sin(time * 8.f); 

        // range para que no desaparezca completamente
        int minAlpha = 155; 
        int alphaRange = 255 - minAlpha;
        int alpha = minAlpha + static_cast<int>((sineWave + 1.f) * 0.5f * alphaRange);

        // Si esta seleccionada, dibujar un borde dorado encima de la carta
        if (selectedCard == i) {
            sf::RectangleShape highlight({cardW, cardH});
            highlight.setPosition({cardX, cardTop});
            highlight.setFillColor(sf::Color(0, 0, 0, 0)); //transparente
            
            // alpha es nuestro blinker
            highlight.setOutlineColor(sf::Color(210, 160, 60, alpha)); // dorado
            highlight.setOutlineThickness(5.f);
            
            window.draw(highlight);
        }


        // HP display en la tarjeta (parte superior)
        sf::Text hpText(font, "HP: " + std::to_string(cards[i].hp), 40);
        sf::Color lightGreen(144, 238, 144);
        hpText.setFillColor(lightGreen);
        sf::FloatRect hb = hpText.getLocalBounds();
        float hpX = cards[i].centerX - hb.size.x / 2.f - hb.position.x;
        float hpY = cardTop + 8.f;
        
        // Shadow rectangle (dark, more transparent)
        sf::RectangleShape hpShadow({hb.size.x + 16.f, hb.size.y + 10.f});
        hpShadow.setPosition({hpX - 8.f, hpY + 3.f});
        hpShadow.setFillColor(sf::Color(0, 0, 0, 80));
        window.draw(hpShadow);
        
        // Background rectangle (semi-transparent)
        sf::RectangleShape hpBg({hb.size.x + 16.f, hb.size.y + 10.f});
        hpBg.setPosition({hpX - 8.f, hpY});
        hpBg.setFillColor(sf::Color(0, 0, 0, 120));
        window.draw(hpBg);
        
        hpText.setPosition({hpX, hpY - 3.f});
        window.draw(hpText);

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

    // Rectangulo detras del texto
    sf::RectangleShape textBg({440.f, 56.f});
    textBg.setPosition({180.f, 545.f});
    textBg.setFillColor(sf::Color(10, 5, 0, 180));
    window.draw(textBg);

    // Texto de seleccion
    sf::Text tempText(font, "SELECCIONA TU PERSONAJE", 26);
    tempText.setFillColor(sf::Color::White);
    sf::FloatRect tb = tempText.getLocalBounds();
    tempText.setPosition({(800.f - tb.size.x) / 2.f - tb.position.x, 545.f});
    window.draw(tempText);

    sf::Text hintText(font, selectedCard == -1 ? "Haz click en un personaje" : "Presiona C para continuar", 18);
    hintText.setFillColor(sf::Color::White);
    sf::FloatRect hb = hintText.getLocalBounds();
    hintText.setPosition({(800.f - hb.size.x) / 2.f - hb.position.x, 575.f});
    window.draw(hintText);
}
