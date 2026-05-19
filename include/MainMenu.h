#pragma once
#include <SFML/Graphics.hpp>

//Para saber en que pantalla estamos y ver lo de cada una, por ahorita solo estas 3
enum class GameScreen {
    MAIN_MENU,
    CHARACTER_SELECT,
    DOOR_SELECT,   // door select screen
    COMBAT,        // regular combat
    EVENT,         // combatless event (no hace nada aun)
    GAME_OVER,     // died
    WIN            // completo los 5 pisos
};

//Funcion para un boton de play y quit
void drawButton(sf::RenderWindow& window, sf::Font& font,
                float x, float y, float w, float h,
                const std::string& label);

// Maneja eventos y dibuja todo lo del main menu
// Devuelve la pantalla a la que hay que cambiar (o MAIN_MENU si no hay cambio)
GameScreen updateMainMenu(sf::RenderWindow& window, sf::Font& font,
                          const sf::Sprite& bgSprite,
                          const sf::Text& titleText, const sf::Text& subtitleText,
                          float btnX, float playY, float quitY, float btnW, float btnH,
                          const std::optional<sf::Event>& event);

// Solo dibuja el main menu (sin manejar eventos)
void drawMainMenu(sf::RenderWindow& window, sf::Font& font,
                  const sf::Sprite& bgSprite,
                  const sf::Text& titleText, const sf::Text& subtitleText,
                  float btnX, float playY, float quitY, float btnW, float btnH);
