#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.h"
#include "Enemy.h"
#include "CombatEngine.h"
#include "RelicPool.h"
#include <cstdlib>

//Para saber en que pantalla estamos y ver lo de cada una, por ahorita solo estas 3
enum class GameScreen {
    MAIN_MENU,
    CHARACTER_SELECT,
    COMBAT // placeholder por ahora, se vera despues
};

//funcion de boton para play y quit en el main menu
void drawButton(sf::RenderWindow& window, sf::Font& font,
                float x, float y, float w, float h,
                const std::string& label) {

    //Crear exterior dorado oscuro
    sf::RectangleShape border({w + 4.f, h + 4.f});
    border.setPosition({x - 2.f, y - 2.f});
    border.setFillColor(sf::Color(120, 80, 20));
    window.draw(border);

    // Fondo del boton semitransparente
    sf::RectangleShape btn({w, h});
    btn.setPosition({x, y});
    btn.setFillColor(sf::Color(20, 12, 5, 200)); //rgba, 200/255
    window.draw(btn);

    // Texto centrado dentro del boton
    sf::Text text(font, label, 26);
    text.setFillColor(sf::Color(210, 160, 60));
    sf::FloatRect bounds = text.getLocalBounds();
    text.setPosition({
        x + (w - bounds.size.x) / 2.f - bounds.position.x,
        y + (h - bounds.size.y) / 2.f - bounds.position.y
    });
    window.draw(text);

} //fin de la funcion drawButton

//main, abre la ventana, carga recursos, maneja eventos y dibuja todo
int main() {
    //creacion de la ventana, con titulo y tamano 800x600
    sf::RenderWindow window(sf::VideoMode({800, 600}), "The Catacombs");
    window.setFramerateLimit(60);

    //cargar la fuente de los textos
    sf::Font font;
    if (!font.openFromFile("assets/fonts/Arial.ttf")) {
        std::cerr << "Error: no se pudo cargar la fuente\n";
        return 1;
    }

    //cargar la imagen de fondo para el menu principal
    sf::Texture bgTexture;
    if (!bgTexture.loadFromFile("assets/images/CTHomeScreen1.jpg")) {
        std::cerr << "Error: no se pudo cargar el fondo\n";
        return 1;
    }

    //cargar la imagen de fondo para la seleccion de player
    sf::Texture csTexture;
    if (!csTexture.loadFromFile("assets/images/CharacterSelect.jpeg")) {
        std::cerr << "Error: no se pudo cargar el fondo\n";
        return 1;
    }

    // sf::Sprite es el objeto que DIBUJA la textura en pantalla
    // Texture = los datos de la imagen, Sprite = como y donde se dibuja
    sf::Sprite bgSprite(bgTexture);
    sf::Sprite csSprite(csTexture);

    // Escala para cubrir toda la ventana 800x600 porque la imagen original es 686x386
    float scaleX = 800.f / bgTexture.getSize().x; // 800/686 = ~1.16
    float scaleY = 600.f / bgTexture.getSize().y; // 600/386 = ~1.55
    bgSprite.setScale({scaleX, scaleY}); //estira

    //Segunda escala para la imagen de selecccion de personaje, que es 764 × 434
    float csScaleX = 800.f / csTexture.getSize().x; // 800/764 = ~1.04
    float csScaleY = 600.f / csTexture.getSize().y; // 600/434 = ~1.38
    csSprite.setScale({csScaleX, csScaleY}); //estira

    //empezar la pantalla actual en el menu principal
    GameScreen currentScreen = GameScreen::MAIN_MENU;

    //textos de la pantalla inicial
    sf::Text titleText(font, "THE CATACOMBS", 58);
    titleText.setFillColor(sf::Color(210, 160, 60));
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setPosition({
        (800.f - titleBounds.size.x) / 2.f - titleBounds.position.x,
        130.f
    });

    sf::Text subtitleText(font, "- Roguelike Dungeon Crawler -", 17);
    subtitleText.setFillColor(sf::Color(160, 120, 50));
    sf::FloatRect subBounds = subtitleText.getLocalBounds();
    subtitleText.setPosition({
        (800.f - subBounds.size.x) / 2.f - subBounds.position.x,
        205.f
    });

    // Posicion y tamanio de los botones
    float btnW = 220.f, btnH = 55.f;
    float btnX = (800.f - btnW) / 2.f;
    float playY = 310.f;
    float quitY = 390.f;

    //game loop
    while (window.isOpen()) {

        //eventos, como clicks o cerrar la ventana
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            // Si se hizo click, revisar si fue en alguno de los botones del menu principal
            if (const auto* click = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (click->button == sf::Mouse::Button::Left) {
                    sf::Vector2i mouse = sf::Mouse::getPosition(window); 

                    if (currentScreen == GameScreen::MAIN_MENU) {
                        // Play, cambia a la pantalla de seleccion de personaje
                        if (mouse.x >= btnX && mouse.x <= btnX + btnW &&
                            mouse.y >= playY  && mouse.y <= playY + btnH)
                            currentScreen = GameScreen::CHARACTER_SELECT;
                        // Quit el juego, cierra la ventana
                        if (mouse.x >= btnX && mouse.x <= btnX + btnW &&
                            mouse.y >= quitY  && mouse.y <= quitY + btnH)
                            window.close();
                    }
                }
            }
        }

        //Dibujo, lo primero queda atras, lo ultimo queda adelante
        window.clear(sf::Color(10, 5, 0));

        // Si estamos en el menu principal, dibujar todo lo de el
        if (currentScreen == GameScreen::MAIN_MENU) {

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
            drawButton(window, font, btnX, playY, btnW, btnH, "PLAY");
            drawButton(window, font, btnX, quitY, btnW, btnH, "QUIT");

            //Creditos hasta abajo
            sf::Text creditsText(font, "CETYS Universidad  |  ICC C2  |  2026", 13);
            creditsText.setFillColor(sf::Color(100, 75, 30));
            sf::FloatRect credBounds = creditsText.getLocalBounds();
            creditsText.setPosition({
                (800.f - credBounds.size.x) / 2.f - credBounds.position.x,
                565.f
            });
            window.draw(creditsText);
        }

        //si estamis en seleccion de personaje
        else if (currentScreen == GameScreen::CHARACTER_SELECT) {

            //imagen de seleccion de personaje
            window.draw(csSprite);

            sf::Text tempText(font, "SELECCIONA TU PERSONAJE", 26);
            tempText.setFillColor(sf::Color(1, 1, 1)); //de color negro
            sf::FloatRect tb = tempText.getLocalBounds();
            tempText.setPosition({
                (800.f - tb.size.x) / 2.f - tb.position.x,
                260.f
            });
            window.draw(tempText);
        }

        window.display();
    }

    return 0;
}