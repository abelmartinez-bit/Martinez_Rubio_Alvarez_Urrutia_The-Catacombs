#include "MainMenu.h"

void drawButton(sf::RenderWindow& window, sf::Font& font, float x, float y, float w, float h, 
    const std::string& label, sf::Vector2f mousePos) {
    //si el mouse esta dentro del area del boton, hover es true
    bool hover = mousePos.x >= x && mousePos.x <= x + w && mousePos.y >= y && mousePos.y <= y + h;

    //si hover, el borde es dorado mas claro, sino es dorado oscuro
    sf::Color borderColor = hover ? sf::Color(210, 160, 60) : sf::Color(120, 80, 20);
    // Fondo: casi negro normal, cafe oscuro en hover
    sf::Color bgColor = hover ? sf::Color(50, 30, 10, 220) : sf::Color(20, 12, 5, 200);

    //Crear exterior dorado oscuro
    sf::RectangleShape border({w + 4.f, h + 4.f});
    border.setPosition({x - 2.f, y - 2.f});
    border.setFillColor(borderColor); //ahora depende del hover
    window.draw(border);

    // Fondo del boton semitransparente
    sf::RectangleShape btn({w, h});
    btn.setPosition({x, y});
    btn.setFillColor(bgColor);
    window.draw(btn);

    // Texto centrado dentro del boton
    sf::Text text(font, label, 26);
    text.setFillColor(sf::Color(210, 160, 60)); 
    sf::FloatRect bounds = text.getLocalBounds();
    text.setPosition({x + (w - bounds.size.x) / 2.f - bounds.position.x, y + (h - bounds.size.y) / 2.f - bounds.position.y});
    window.draw(text);
}

void drawMainMenu(sf::RenderWindow& window, sf::Font& font, sf::Font& fontSmall, sf::Font& fontTitle,
                  const sf::Sprite& bgSprite, const sf::Text& titleText, const sf::Text& subtitleText,
                  float btnX, float playY, float relicsY, float quitY, float btnW, float btnH, 
                  sf::Vector2f mousePos) {

    //imagen de fondo
    window.draw(bgSprite);

    //overlay semitransparente para oscurecer un poco todo
    sf::RectangleShape overlay({800.f, 600.f});
    overlay.setFillColor(sf::Color(0, 0, 0, 80));
    window.draw(overlay);

    //panel semitransparente para el titulo y botones
    sf::RectangleShape panel({520.f, 380.f});
    panel.setPosition({140.f, 100.f});
    panel.setFillColor(sf::Color(10, 5, 0, 160));
    window.draw(panel);

    //linea decorativa arriba del titulo
    sf::RectangleShape lineTop({480.f, 2.f});
    lineTop.setFillColor(sf::Color(120, 80, 20));
    lineTop.setPosition({160.f, 112.f});
    window.draw(lineTop);

    // Titulo y subtitulo
    window.draw(titleText);
    window.draw(subtitleText);

    // Linea decorativa debajo del subtitulo
    sf::RectangleShape lineBot({480.f, 2.f});
    lineBot.setFillColor(sf::Color(120, 80, 20));
    lineBot.setPosition({160.f, 232.f});
    window.draw(lineBot);

    // Botones
    drawButton(window, font, btnX, playY, btnW, btnH, "PLAY", mousePos);
    drawButton(window, font, btnX, relicsY, btnW, btnH, "RELICS", mousePos); //nuevo boton de reliquias, por ahora no hace nada
    drawButton(window, font, btnX, quitY, btnW, btnH, "QUIT", mousePos);

    //Creditos hasta abajo
    sf::Text creditsText(fontSmall, "CETYS Universidad  |  ICC C2  |  2026", 13);
    creditsText.setFillColor(sf::Color(210, 160, 60));
    sf::FloatRect credBounds = creditsText.getLocalBounds();
    creditsText.setPosition({(800.f - credBounds.size.x) / 2.f - credBounds.position.x, 565.f});
    window.draw(creditsText);
}

