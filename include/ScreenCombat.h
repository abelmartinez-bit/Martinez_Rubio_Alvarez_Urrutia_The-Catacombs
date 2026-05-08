#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Enemy.h"
#include "CombatEngine.h"

// Maneja teclas en combate (SPACE avanza turno, P usa pocion)
void handleCombatKey(sf::Keyboard::Key key, Player* player,
                     CombatEngine* combat);

// Dibuja la pantalla de combate y entrega recompensa si termino
void drawCombat(sf::RenderWindow& window,
                Player* player, Enemy* enemy, CombatEngine* combat,
                sf::Text& playerText, sf::Text& enemyText,
                sf::Text& logText, sf::Text& resultText,
                sf::Text& instructionText, sf::Text& combatTitleText,
                bool& rewardGiven);
