#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.h"
#include "Enemy.h"
#include "CombatEngine.h"
#include "RelicPool.h"
#include <cstdlib>
#include "MainMenu.h"
#include "CharacterSelect.h"
#include "ScreenCombat.h"

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
    float scaleX = 800.f / bgTexture.getSize().x;
    float scaleY = 600.f / bgTexture.getSize().y;
    bgSprite.setScale({scaleX, scaleY});
 
    //Segunda escala para la imagen de seleccion de personaje, que es 764x434
    float csScaleX = 800.f / csTexture.getSize().x;
    float csScaleY = 600.f / csTexture.getSize().y;
    csSprite.setScale({csScaleX, csScaleY});
 
    //empezar la pantalla actual en el menu principal
    GameScreen currentScreen = GameScreen::MAIN_MENU;
 
    //Main menu
 
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
 
    // Posicion y tamanio de los botones del main menu
    float btnW = 220.f, btnH = 55.f;
    float btnX = (800.f - btnW) / 2.f;
    float playY = 310.f, quitY = 390.f;
 
    //Seleccion de personaje 
 
    // Datos de los 4 personajes
    CharacterCard cards[4] = {
        {  90.f, "Warrior", PlayerType::WARRIOR, 130, 20, 15, 10, 10, 10 },
        { 290.f, "Mage",    PlayerType::MAGE,    100, 10, 10, 25, 20, 15 },
        { 490.f, "Rogue",   PlayerType::ROGUE,   110, 15, 10, 10, 10, 25 },
        { 690.f, "Archer",  PlayerType::ARCHER,  100, 10, 15, 25, 15, 25 }
    };
 
    // ancho y alto clickeable de cada carta
    float cardW = 160.f, cardH = 480.f, cardTop = 60.f;
 
    // indice del personaje seleccionado (-1 = ninguno)
    int selectedCard = -1;
 
    //Combate 
 
    // Pointers, se crean cuando el jugador elige personaje
    Player* player = nullptr;
    Enemy* enemy = nullptr;
    CombatEngine* combat = nullptr;
    bool rewardGiven = false;
 
    // Textos de combate
    sf::Text combatTitleText(font, "THE CATACOMBS", 36);
    combatTitleText.setFillColor(sf::Color::White);
    combatTitleText.setPosition({250.f, 20.f});
 
    sf::Text playerText(font, "", 20);
    playerText.setFillColor(sf::Color::Green);
    playerText.setPosition({50.f, 100.f});
 
    sf::Text enemyText(font, "", 20);
    enemyText.setFillColor(sf::Color::Red);
    enemyText.setPosition({500.f, 100.f});
 
    sf::Text logText(font, "", 16);
    logText.setFillColor(sf::Color::Yellow);
    logText.setPosition({50.f, 350.f});
 
    sf::Text instructionText(font, "SPACE: avanzar turno  |  P: usar pocion", 16);
    instructionText.setFillColor(sf::Color::White);
    instructionText.setPosition({200.f, 550.f});
 
    sf::Text resultText(font, "", 28);
    resultText.setFillColor(sf::Color::White);
    resultText.setPosition({250.f, 260.f});
 
    //Game loop
    while (window.isOpen()) {
 
        //eventos, como clicks o cerrar la ventana
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
 
            // Si se hizo click
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
 
                    // En seleccion de personaje, detectar click en cada carta
                    else if (currentScreen == GameScreen::CHARACTER_SELECT) {
                        int clicked = handleCharacterSelectClick(mouse, cards, cardW, cardH, cardTop);
                        if (clicked != -1) selectedCard = clicked;
                    }
                }
            }
 
            // Si se presiono una tecla
            if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
                if (currentScreen == GameScreen::CHARACTER_SELECT) {
                    if (handleCharacterSelectKey(keyEvent->code, selectedCard, cards, player, enemy, combat))
                        currentScreen = GameScreen::COMBAT;
                }
                else if (currentScreen == GameScreen::COMBAT) {
                    handleCombatKey(keyEvent->code, player, combat);
                }
            }
        }
 
        //Dibujar, lo primero queda atras, lo ultimo queda adelante
        window.clear(sf::Color(10, 5, 0));
 
        if (currentScreen == GameScreen::MAIN_MENU)
            drawMainMenu(window, font, bgSprite, titleText, subtitleText, btnX, playY, quitY, btnW, btnH);
 
        else if (currentScreen == GameScreen::CHARACTER_SELECT)
            drawCharacterSelect(window, font, csSprite, cards, selectedCard, cardW, cardH, cardTop);
 
        else if (currentScreen == GameScreen::COMBAT && player && combat)
            drawCombat(window, player, enemy, combat, playerText, enemyText,
                       logText, resultText, instructionText, combatTitleText, rewardGiven);
 
        window.display();
    }
 
    // Limpiar memoria de los pointers al cerrar
    delete combat;
    delete enemy;
    delete player;
 
    return 0;
}