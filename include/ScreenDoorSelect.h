#pragma once
#include <SFML/Graphics.hpp>
#include "Floor.h"
#include "Player.h"
#include <array>

// con este y el .cpp haces hook up las nuevas door screens
void drawDoorSelect(sf::RenderWindow& window, sf::Font& font, const sf::Sprite& bgSprite, const Floor* floor, const Player* player, 
                    int floorNumber, Enemy* previewEnemies[3], const std::array<std::string,3>& statOverrides = {});