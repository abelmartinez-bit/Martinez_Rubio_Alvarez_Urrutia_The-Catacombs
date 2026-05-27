//Nueva para manejar gameover, win y pop ups de level up y move select sin cin
#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"

void drawGameOver(sf::RenderWindow& window, sf::Font& font);
void drawWin(sf::RenderWindow& window, sf::Font& font);
void drawLevelUp(sf::RenderWindow& window, sf::Font& font, Player* player, sf::Vector2f mousePos);
void drawMoveSelect(sf::RenderWindow& window, sf::Font& font, Player* player, sf::Vector2f mousePos);