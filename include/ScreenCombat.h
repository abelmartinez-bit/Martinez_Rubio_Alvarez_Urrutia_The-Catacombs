#pragma once
#include <SFML/Graphics.hpp>
#include "Floor.h"
#include "Player.h"
#include "Enemy.h"
#include "FinalBoss.h"
#include "CombatEngine.h"

// Maneja teclas en combate (SPACE avanza turno, P usa pocion)
void handleCombatKey(sf::Keyboard::Key key, Player* player, CombatEngine* combat);

// Dibuja la pantalla de combate y entrega recompensa si termino
void drawCombat(sf::RenderWindow& window, sf::Font& font,
                Player* player, Enemy* enemy, CombatEngine* combat,
                sf::Text& playerText, sf::Text& enemyText,
                sf::Text& logText, sf::Text& resultText,
                sf::Text& instructionText, sf::Text& combatTitleText,
                bool& rewardGiven, const sf::Sprite& combatSprite,
                Floor* floor, bool isMiniBoss, std::string& rewardDescription, bool isFinalBoss = false);

void drawFinalBossCombat(sf::RenderWindow& window, sf::Font& font,
                         Player* player, Enemy* enemy, CombatEngine* combat,
                         sf::Text& playerText, sf::Text& enemyText,
                         sf::Text& logText, sf::Text& resultText,
                         sf::Text& instructionText,
                         bool& rewardGiven, const sf::Sprite& finalBossSprite,
                         Floor* floor, std::string& rewardDescription);
 
