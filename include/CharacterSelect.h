#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "MainMenu.h"
#include "Player.h"

// Datos de cada personaje en la pantalla de seleccion
struct CharacterCard {
    float centerX;
    std::string name;
    PlayerType type;
    int hp, atk, def, spAtk, spDef, spd;
};

// Maneja el click en las cartas y devuelve el indice seleccionado
int handleCharacterSelectClick(sf::Vector2f mouse, CharacterCard cards[4], float cardW, float cardH, float cardTop);

// Al presionar C con personaje elegido, crea el Player y devuelve true.
// Enemy y CombatEngine ya no se crean aqui: GameLoop los maneja en selectDoor().
bool handleCharacterSelectKey(sf::Keyboard::Key key, int selectedCard, CharacterCard cards[4], Player*& player);

// Dibuja la pantalla de seleccion de personaje
void drawCharacterSelect(sf::RenderWindow& window, sf::Font& font, const sf::Sprite& csSprite, CharacterCard cards[4], int selectedCard, float cardW, float cardH, float cardTop, float time);
// hanles less things now