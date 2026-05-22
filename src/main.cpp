#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
// normal libraries 

// our headers
#include "Player.h"
#include "MainMenu.h"
#include "CharacterSelect.h"
#include "ScreenDoorSelect.h"
#include "ScreenCombat.h"
#include "GameLoop.h"
#include "RelicTracker.h"

// gameover screen (podemos mover esto)

static void drawGameOver(sf::RenderWindow& window, sf::Font& font) {
    // Overlay oscuro semitransparente
    sf::RectangleShape overlay({800.f, 600.f});
    overlay.setFillColor(sf::Color(0, 0, 0, 200));
    window.draw(overlay);

    sf::Text title(font, "GAME OVER", 72);
    title.setFillColor(sf::Color(200, 40, 40));
    sf::FloatRect tb = title.getLocalBounds();
    title.setPosition({(800.f - tb.size.x) / 2.f - tb.position.x, 180.f});
    window.draw(title);

    sf::Text sub(font, "The Catacombs claimed another soul.", 22);
    sub.setFillColor(sf::Color(180, 130, 130));
    sf::FloatRect sb = sub.getLocalBounds();
    sub.setPosition({(800.f - sb.size.x) / 2.f - sb.position.x, 290.f});
    window.draw(sub);

    sf::Text hint(font, "Press  R  to return to the main menu.", 19);
    hint.setFillColor(sf::Color(150, 150, 150));
    sf::FloatRect hb = hint.getLocalBounds();
    hint.setPosition({(800.f - hb.size.x) / 2.f - hb.position.x, 380.f});
    window.draw(hint);
}

static void drawWin(sf::RenderWindow& window, sf::Font& font) {
    sf::RectangleShape overlay({800.f, 600.f});
    overlay.setFillColor(sf::Color(0, 0, 0, 190));
    window.draw(overlay);

    sf::Text title(font, "YOU ESCAPED!", 66);
    title.setFillColor(sf::Color(210, 160, 60));
    sf::FloatRect tb = title.getLocalBounds();
    title.setPosition({(800.f - tb.size.x) / 2.f - tb.position.x, 170.f});
    window.draw(title);

    sf::Text sub(font, "You conquered all 5 floors of The Catacombs.", 20);
    sub.setFillColor(sf::Color(180, 160, 100));
    sf::FloatRect sb = sub.getLocalBounds();
    sub.setPosition({(800.f - sb.size.x) / 2.f - sb.position.x, 285.f});
    window.draw(sub);

    sf::Text hint(font, "Press  R  to return to the main menu.", 19);
    hint.setFillColor(sf::Color(150, 150, 150));
    sf::FloatRect hb = hint.getLocalBounds();
    hint.setPosition({(800.f - hb.size.x) / 2.f - hb.position.x, 375.f});
    window.draw(hint);
}


// main

int main() {
    srand(static_cast<unsigned int>(time(nullptr))); // seed para aleatoriedad
    //creacion de la ventana, ahora abre en fullscreen con la resolucion del monitor, pero se puede cambiar a 800x600 para pruebas
    sf::RenderWindow window(sf::VideoMode({800, 600}), "The Catacombs");
    window.setFramerateLimit(60);

    //config de la vista para coordenadas 800x600, asi es mas facil posicionar los elementos en pantalla
    sf::View view(sf::FloatRect({0.f, 0.f}, {800.f, 600.f}));
    window.setView(view);

    sf::Font font;
    if (!font.openFromFile("assets/fonts/Arial.ttf")) {
        std::cerr << "Error: no se pudo cargar la fuente\n";
        return 1;
    }

    sf::Texture bgTexture;
    if (!bgTexture.loadFromFile("assets/images/CTHomeScreen1.jpg")) {
        std::cerr << "Error: no se pudo cargar el fondo del menu\n";
        return 1;
    }

    sf::Texture csTexture;
    if (!csTexture.loadFromFile("assets/images/CharacterSelect.jpeg")) {
        std::cerr << "Error: no se pudo cargar el fondo de seleccion\n";
        return 1;
    }

    sf::Texture combatTexture;
    if (!combatTexture.loadFromFile("assets/images/Corridor.jpeg")) {
        std::cerr << "Error: no se pudo cargar el fondo de combate\n";
        return 1;
    }

    sf::Sprite bgSprite(bgTexture);
    bgSprite.setScale({800.f / bgTexture.getSize().x, 600.f / bgTexture.getSize().y});

    sf::Sprite csSprite(csTexture);
    csSprite.setScale({800.f / csTexture.getSize().x, 600.f / csTexture.getSize().y});

    sf::Sprite combatSprite(combatTexture);
    combatSprite.setScale({800.f / combatTexture.getSize().x, 600.f / combatTexture.getSize().y});


    // current game state
    GameScreen currentScreen = GameScreen::MAIN_MENU;
    GameLoop gameLoop;
    Player* player = nullptr;

    // rewardGiven se pasa por referencia a drawCombat, que lo pone en true
    // cuando entrega las recompensas. main.cpp lo usa para poder presionar ENTER.
    bool rewardGiven = false;


    // Main Menu — textos y botones

    sf::Text titleText(font, "THE CATACOMBS", 58);
    titleText.setFillColor(sf::Color(210, 160, 60));
    {
        sf::FloatRect b = titleText.getLocalBounds();
        titleText.setPosition({(800.f - b.size.x) / 2.f - b.position.x, 130.f});
    }

    sf::Text subtitleText(font, "- Roguelike Dungeon Crawler -", 17);
    subtitleText.setFillColor(sf::Color(160, 120, 50));
    {
        sf::FloatRect b = subtitleText.getLocalBounds();
        subtitleText.setPosition({(800.f - b.size.x) / 2.f - b.position.x, 205.f});
    }

    const float btnW = 220.f, btnH = 55.f;
    const float btnX = (800.f - btnW) / 2.f;
    const float playY = 250.f, relicsY = 330.f, quitY = 410.f; // por ahora no hace nada


    // Character Select, char data (needs tweaking too weak)

    CharacterCard cards[4] = {
        {  117.f, "Warrior", PlayerType::WARRIOR, 1300, 30, 35, 10, 25, 10 }, // testing
        { 306.f, "Mage",    PlayerType::MAGE,    100, 10, 25, 30, 35, 15 },
        { 498.f, "Rogue",   PlayerType::ROGUE,   110, 40, 22, 15, 32, 40 },
        { 689.f, "Archer",  PlayerType::ARCHER,  100, 15, 30, 30, 25, 25 }
    };
    const float cardW = 160.f, cardH = 480.f, cardTop = 60.f;
    int selectedCard = -1;


    // Combat text

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

    sf::Text instructionText(font, "SPACE: avanzar turno   |   P: usar pocion", 16);
    instructionText.setFillColor(sf::Color::White);
    instructionText.setPosition({160.f, 550.f});

    sf::Text resultText(font, "", 28);
    resultText.setFillColor(sf::Color::White);
    resultText.setPosition({250.f, 260.f});


    // main game while loop

    while (window.isOpen()) {

        while (const std::optional event = window.pollEvent()) {

            // Cerrar ventana
            if (event->is<sf::Event::Closed>())
                window.close();

            // Mouse clicks
            if (const auto* click = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (click->button == sf::Mouse::Button::Left) {//click izquierdo
                    //mapPixelToCoords toma el pixel de la pantalla y lo convierte a la coordenada
                    sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                    if (currentScreen == GameScreen::MAIN_MENU) {
                        // Boton PLAY
                        if (mouse.x >= btnX && mouse.x <= btnX + btnW && mouse.y >= playY && mouse.y <= playY + btnH)
                            currentScreen = GameScreen::CHARACTER_SELECT;

                            // boton de reliquias
                        if (mouse.x >= btnX && mouse.x <= btnX + btnW && mouse.y >= relicsY && mouse.y <= relicsY + btnH)
                            currentScreen = GameScreen::RELIC_TRACKER;

                        // Boton QUIT
                        if (mouse.x >= btnX && mouse.x <= btnX + btnW && mouse.y >= quitY && mouse.y <= quitY + btnH)
                            window.close();
                    }
                    else if (currentScreen == GameScreen::CHARACTER_SELECT) {
                        int clicked = handleCharacterSelectClick(mouse, cards, cardW, cardH, cardTop);
                        if (clicked != -1) selectedCard = clicked;
                    }
                }
            }

            // Teclas
            if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
                //escape cierra el juego desde cualquier pantalla
                if (keyEvent->code == sf::Keyboard::Key::Escape)
                    window.close();

                // char select
                if (currentScreen == GameScreen::CHARACTER_SELECT) {
                    if (handleCharacterSelectKey(keyEvent->code, selectedCard, cards, player)) {
                        // Jugador creado: iniciamos partida en piso 1
                        gameLoop.startRun(player);
                        rewardGiven = false;
                        currentScreen = GameScreen::DOOR_SELECT;
                    }
                }

                // simple door select con 1, 2, 3. needs visual tweaking
                else if (currentScreen == GameScreen::DOOR_SELECT) {
                    int doorIdx = -1;
                    if (keyEvent->code == sf::Keyboard::Key::Num1) doorIdx = 0;
                    else if (keyEvent->code == sf::Keyboard::Key::Num2) doorIdx = 1;
                    else if (keyEvent->code == sf::Keyboard::Key::Num3) doorIdx = 2;

                    if (doorIdx != -1) {
                        rewardGiven = false;
                        currentScreen = gameLoop.selectDoor(doorIdx);
                        // Si fue evento, selectDoor ya genero el siguiente piso
                        // y devolvio DOOR_SELECT (o WIN/FINAL BOSS si era el ultimo piso)
                    }
                }

                // main combat
                else if (currentScreen == GameScreen::COMBAT) {
                    if (keyEvent->code == sf::Keyboard::Key::Space) {
                        gameLoop.advanceTurn();
                    }
                    else if (keyEvent->code == sf::Keyboard::Key::P) {
                        gameLoop.usePotionInCombat();
                    }
                    else if (keyEvent->code == sf::Keyboard::Key::Enter) {
                        // Solo avanza si el combate ya termino y se entregaron recompensas
                        CombatEngine* combat = gameLoop.getCombat();
                        if (combat && combat->isBattleOver() && rewardGiven) {
                            currentScreen = gameLoop.onCombatFinished();
                            // Si el siguiente estado es DOOR_SELECT, rewardGiven
                            // se reseteara al elegir la siguiente puerta
                        }
                    }
                }

                else if (currentScreen == GameScreen::RELIC_TRACKER) {
                    if (keyEvent->code == sf::Keyboard::Key::B || keyEvent->code == sf::Keyboard::Key::R) {
                          currentScreen = GameScreen::MAIN_MENU;
    }
}

                // Game Over / Win / Final Boss  R para volver al menu
                else if (currentScreen == GameScreen::GAME_OVER || currentScreen == GameScreen::WIN) {
                    if (keyEvent->code == sf::Keyboard::Key::R) {
                        gameLoop.reset();
                        delete player;
                        player = nullptr;
                        selectedCard = -1;
                        rewardGiven = false;
                        currentScreen = GameScreen::MAIN_MENU;
                    }
                }
            }
        }
  
        
        window.clear(sf::Color(10, 5, 0));

        if (currentScreen == GameScreen::MAIN_MENU) {
            drawMainMenu(window, font, bgSprite, titleText, subtitleText, btnX, playY, relicsY, quitY, btnW, btnH);
        }
        
        else if (currentScreen == GameScreen::RELIC_TRACKER) {
            RelicTracker::drawRelicTracker(window, font, bgSprite);        }

        else if (currentScreen == GameScreen::CHARACTER_SELECT) {
            drawCharacterSelect(window, font, csSprite, cards, selectedCard, cardW, cardH, cardTop);
        }   
        else if (currentScreen == GameScreen::DOOR_SELECT && gameLoop.getCurrentFloor() && player) {
            drawDoorSelect(window, font, bgSprite, gameLoop.getCurrentFloor(), player, gameLoop.getFloorNumber());
        }
        else if (currentScreen == GameScreen::COMBAT && player && gameLoop.getCombat() && gameLoop.getCurrentFloor() && gameLoop.getCurrentEnemy()) {
            drawCombat(window, player, gameLoop.getCurrentEnemy(), gameLoop.getCombat(), playerText, enemyText, logText, resultText, instructionText, combatTitleText,
                       rewardGiven, combatSprite, gameLoop.getCurrentFloor(), gameLoop.getIsMiniBoss());
        }
        else if (currentScreen == GameScreen::GAME_OVER) {
            // Dibuja el ultimo frame de combate como fondo (si existe)
            if (gameLoop.getCurrentFloor() && gameLoop.getCurrentEnemy() && gameLoop.getCombat() && player) {
                drawCombat(window, player, gameLoop.getCurrentEnemy(), gameLoop.getCombat(), playerText, enemyText, logText, resultText,
                           instructionText, combatTitleText, rewardGiven, combatSprite, gameLoop.getCurrentFloor(), gameLoop.getIsMiniBoss());
            }
            drawGameOver(window, font);
        }
        else if (currentScreen == GameScreen::WIN) {
            drawWin(window, font);
        }

        window.display();
    }

    // memory cleanup
    
    gameLoop.reset();   // destruye Floor, Enemy, CombatEngine
    delete player;      // main.cpp es el owner del Player

    return 0;
}