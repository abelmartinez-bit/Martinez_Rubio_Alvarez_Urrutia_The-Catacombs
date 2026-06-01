#pragma once
#include <SFML/Graphics.hpp>
#include "Floor.h"
#include "Player.h"
#include "Enemy.h"
#include "FinalBoss.h"
#include "CombatEngine.h"

void drawHpBar(sf::RenderWindow& window, float x, float y, float width, float height, int currentHp, int maxHp);
void drawPanelBtn(sf::RenderWindow& window, sf::Font& font, float x, float y, float width, float height, const std::string& label, bool hovered);
void drawMoveBtn(sf::RenderWindow& window, sf::Font& font, float x, float y, float width, float height, const Move& move, bool hovered);
void drawPopup(sf::RenderWindow& window, sf::Font& font, float x, float y, float width, float height, const std::string& title, const std::vector<std::string>& lines);

//Principal, con cambios para dibujar la pantalla de combate, tipo pokemon y entrega recompensa si termino
void drawCombat(sf::RenderWindow& window, sf::Font& font,
                Player* player, Enemy* enemy, CombatEngine* combat,
                sf::Text& playerText, sf::Text& enemyText,
                sf::Text& logText, sf::Text& resultText,
                sf::Text& instructionText,
                bool& rewardGiven, const sf::Sprite& combatSprite,
                Floor* floor, bool isMiniBoss, std::string& rewardDescription, 
                bool showingMoves, sf::Vector2f mousePos);
                //nuevos parametros
                //showingMoves para saber si estamos mostrando los 4 botones o los 4 moves
                //mousePos para saber donde se hace hover para los pop ups

void drawFinalBossCombat(sf::RenderWindow& window, sf::Font& font, Player* player, Enemy* enemy, CombatEngine* combat,
                         sf::Text& playerText, sf::Text& enemyText, sf::Text& logText, sf::Text& resultText,
                         sf::Text& instructionText, bool& rewardGiven, const sf::Sprite& finalBossBackground,
                         Floor* floor, std::string& rewardDescription, bool showingMoves, sf::Vector2f mousePos);
