#pragma once
#include <SFML/Graphics.hpp>
#include "Floor.h"
#include "Player.h"
#include "Enemy.h"
#include "CombatEngine.h"

// Maneja teclas en combate (SPACE avanza turno, P usa pocion)
void handleCombatKey(sf::Keyboard::Key key, Player* player, CombatEngine* combat);

//Principal, con cambios para dibujar la pantalla de combate, tipo pokemon y entrega recompensa si termino
void drawCombat(sf::RenderWindow& window, sf::Font& font,
                Player* player, Enemy* enemy, CombatEngine* combat,
                sf::Text& playerText, sf::Text& enemyText,
                sf::Text& logText, sf::Text& resultText,
                sf::Text& instructionText,
                bool& rewardGiven, const sf::Sprite& combatSprite,
                Floor* floor, bool isMiniBoss, std::string& rewardDescription);
