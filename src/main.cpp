#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <array>
// normal libraries 

// nuestros headers
#include "Player.h"
#include "MainMenu.h"
#include "CharacterSelect.h"
#include "ScreenDoorSelect.h"
#include "ScreenCombat.h"
#include "GameLoop.h"
#include "ScreenOverlays.h"
#include "RelicTracker.h"


// main

int main() {
    srand(static_cast<unsigned int>(time(nullptr))); // seed para aleatoriedad
    //creacion de la ventana, ahora abre en fullscreen, pero se puede cambiar a 800x600 para pruebas
    sf::RenderWindow window(sf::VideoMode::getFullscreenModes()[0], "The Catacombs", sf::State::Fullscreen);
    window.setFramerateLimit(60);

    sf::Clock flashClock; // Reloj para animaciones de flashing

    //config de la vista para coordenadas 800x600, asi es mas facil posicionar los elementos en pantalla
    sf::View view(sf::FloatRect({0.f, 0.f}, {800.f, 600.f}));
    window.setView(view);

    //crear y cargar fuentes, texturas y sprites , y music
    sf::Font font; //cinzel, principal
    sf::Font fontSmall; //arial, para letras chiquitas
    sf::Font fontTitle; //MedievalSharp solo para titulo

    if (!font.openFromFile("assets/fonts/Cinzel-VariableFont_wght.ttf")) {
        std::cerr << "Error: no se pudo cargar la fuente\n";
        return 1;
    }

    if (!fontSmall.openFromFile("assets/fonts/Arial.ttf")) {
        std::cerr << "Error: no se pudo cargar la fuente\n";
        return 1;
    }

    if (!fontTitle.openFromFile("assets/fonts/MedievalSharp-Regular.ttf")) {
        std::cerr << "Error: no se pudo cargar la fuente\n";
        return 1;
    }

    sf::Music music;
    if (!music.openFromFile("assets/audio/PianoBG.wav")) {
        std::cerr << "Error: no se pudo cargar la musica\n";
     return 1;
    }
    music.setLooping(true);
    music.setVolume(50.f);
    music.play();

    sf::Texture bgTexture; //fondo del main menu
    if (!bgTexture.loadFromFile("assets/images/CTHomeScreen1.jpg")) {
        std::cerr << "Error: no se pudo cargar el fondo del menu\n";
        return 1;
    }

    sf::Texture csTexture; //fondo del character select
    if (!csTexture.loadFromFile("assets/images/CharacterSelect.jpeg")) {
        std::cerr << "Error: no se pudo cargar el fondo de seleccion\n";
        return 1;
    }

    sf::Texture combatTexture; //fondo del combate
    if (!combatTexture.loadFromFile("assets/images/CombatBG2.jpg")) {
        std::cerr << "Error: no se pudo cargar el fondo de combate\n";
        return 1;
    }

    sf::Texture eventTexture; //fondo de eventos
    if (!eventTexture.loadFromFile("assets/images/event_screen.jpg")) {
        std::cerr << "Error: no se pudo cargar el fondo de eventos\n";
        return 1;
    }

    sf::Texture rewardsTexture; // fondo de pantalla de recompensas
    if (!rewardsTexture.loadFromFile("assets/images/dungeon_hallway.jpg")) {
        std::cerr << "Error: no se pudo cargar el fondo de recompensas\n";
        return 1;
    }

    sf::Texture wall;
    if (!wall.loadFromFile("assets/images/3_doors.jpg")) {
        std::cerr << "Error: no se pudo cargar la imagen de la pared\n";
        return 1;
    }

    sf::Texture door;
    if (!door.loadFromFile("assets/images/door2.png")) {
        std::cerr << "Error: no se pudo cargar la imagen de la puerta\n";
        return 1;
    }

    sf::Texture doorC;
    if (!doorC.loadFromFile("assets/images/door2_c.png")) {
        std::cerr << "Error: no se pudo cargar la imagen de la puerta corrupta\n";
        return 1;
    }

    sf::Sprite bgSprite(bgTexture); 
    bgSprite.setScale({800.f / bgTexture.getSize().x, 600.f / bgTexture.getSize().y});

    sf::Sprite csSprite(csTexture);
    csSprite.setScale({800.f / csTexture.getSize().x, 600.f / csTexture.getSize().y});

    sf::Sprite combatSprite(combatTexture);
    combatSprite.setScale({800.f / combatTexture.getSize().x, 600.f / combatTexture.getSize().y});

    sf::Sprite eventSprite(eventTexture);
    eventSprite.setScale({800.f / eventTexture.getSize().x, 600.f / eventTexture.getSize().y});

    sf::Sprite rewardsSprite(rewardsTexture);
    rewardsSprite.setScale({800.f / rewardsTexture.getSize().x, 600.f / rewardsTexture.getSize().y});

    sf::Sprite wallSprite(wall);
    wallSprite.setScale({800.f / wall.getSize().x, 600.f / wall.getSize().y});

    sf::Sprite doorSprite(door);
    sf::Sprite doorCorruptSprite(doorC);

    // current game state
    GameScreen currentScreen = GameScreen::MAIN_MENU; //empezar en el main menu
    GameLoop gameLoop; //crear el game loop que encarga de pisos, enemigos, combate, etc
    Player* player = nullptr; //

    // rewardGiven se pasa por referencia a drawCombat, que lo pone en true
    // cuando entrega las recompensas. main.cpp lo usa para poder presionar ENTER.
    bool rewardGiven = false;
    bool showingMoves = false; // true = panel de moves, false = menu principal
    std::string combatRewardDescription;   // set by drawCombat when rewards are given

    // Transitions

    enum class TransitionPhase { NONE, FADE_OUT, FADE_IN };
    TransitionPhase transitionPhase = TransitionPhase::NONE;
    GameScreen pendingScreen = GameScreen::MAIN_MENU;
    float transitionAlpha = 0.f;
    const float TRANSITION_SPEED = 700.f;
    sf::RectangleShape transitionOverlay({800.f, 600.f});
    transitionOverlay.setFillColor(sf::Color(0, 0, 0, 0));
    sf::Clock dtClock;
    std::function<void()> onScreenFlip = nullptr; // for running code at the exact moment we actually switch screens during a transition




    //nuevas variables para evitar cin en level up y move select
    bool showLevelUp = false;
    bool showMoveSelect = false;

    // Track the next screen to transition to after level up from event door
    GameScreen nextScreenAfterLevelUp = GameScreen::MAIN_MENU;

    // Freeze door preview display while fading out to avoid showing updated previews
    bool freezeDoorPreview = false;
    std::array<std::string,3> frozenStatStr = {"", "", ""};

    // Main Menu — textos y botones
    sf::Text titleText(fontTitle, "THE CATACOMBS", 58);
    titleText.setFillColor(sf::Color(210, 160, 60));
    {
        sf::FloatRect b = titleText.getLocalBounds();
        titleText.setPosition({(800.f - b.size.x) / 2.f - b.position.x, 130.f});
    }

    sf::Text subtitleText(fontSmall, "- Roguelike Dungeon Crawler -", 17);
    subtitleText.setFillColor(sf::Color(160, 120, 50));
    {
        sf::FloatRect b = subtitleText.getLocalBounds();
        subtitleText.setPosition({(800.f - b.size.x) / 2.f - b.position.x, 205.f});
    }

    const float btnW = 220.f, btnH = 55.f;
    const float btnX = (800.f - btnW) / 2.f;
    const float playY = 250.f, relicsY = 330.f, quitY = 410.f; // por ahora no hace nada


    //crear array de structs para las cards de seleccion de personaje
    CharacterCard cards[4] = {
        //centerX, name, PlayerType, hp, atk, def, spAtk, spDef, spd
        { 117.f, "Warrior", PlayerType::WARRIOR, 1300, 30, 35, 10, 25, 10 }, // testing
        { 306.f, "Mage",    PlayerType::MAGE,    100, 10, 25, 35, 35, 15 },
        { 498.f, "Rogue",   PlayerType::ROGUE,   110, 40, 22, 15, 22, 40 },
        { 689.f, "Archer",  PlayerType::ARCHER,  100, 15, 30, 30, 25, 25 }
    };
    const float cardW = 172.f, cardH = 520.f, cardTop = 20.f;
    int selectedCard = -1;


    // Combat text

    sf::Text playerText(font, "", 20);
    playerText.setFillColor(sf::Color::Green);
    playerText.setPosition({50.f, 100.f});

    sf::Text enemyText(font, "", 20);
    enemyText.setFillColor(sf::Color::Red);
    enemyText.setPosition({500.f, 100.f});

    sf::Text logText(font, "", 16);
    logText.setFillColor(sf::Color(0, 0, 0, 0)); //ahora invisible porque el log lo maneja el nuevo panel
    logText.setPosition({50.f, 350.f});

    sf::Text instructionText(font, "SPACE: avanzar turno   |   P: usar pocion", 16);
    instructionText.setFillColor(sf::Color::White);
    instructionText.setPosition({160.f, 550.f});

    sf::Text resultText(font, "", 28);
    resultText.setFillColor(sf::Color::White);
    resultText.setPosition({250.f, 260.f});


    // main game while loop

    while (window.isOpen()) {

        float timePassed = flashClock.getElapsedTime().asSeconds();
        float dt = dtClock.restart().asSeconds();

        auto startTransition = [&](GameScreen target) {
            pendingScreen = target;
            transitionPhase = TransitionPhase::FADE_OUT;
            transitionAlpha = 0.f;
        };

        if (transitionPhase == TransitionPhase::FADE_OUT) {
            transitionAlpha += TRANSITION_SPEED * dt;
            if (transitionAlpha >= 255.f) {
                transitionAlpha = 255.f;
                currentScreen = pendingScreen;
                if (onScreenFlip) { 
                    onScreenFlip(); 
                    onScreenFlip = nullptr; 
                }
                transitionPhase = TransitionPhase::FADE_IN;
            }
        } else if (transitionPhase == TransitionPhase::FADE_IN) {
            transitionAlpha -= TRANSITION_SPEED * dt;
            if (transitionAlpha <= 0.f) {
                transitionAlpha = 0.f;
                transitionPhase = TransitionPhase::NONE;
            }
        }
        transitionOverlay.setFillColor(sf::Color(0, 0, 0, static_cast<uint8_t>(transitionAlpha)));

        while (const std::optional event = window.pollEvent()) {

            if (transitionPhase != TransitionPhase::NONE) {
                if (event->is<sf::Event::Closed>()) window.close();
                continue;
            }

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
                        if (mouse.x >= btnX && mouse.x <= btnX + btnW && mouse.y >= playY && mouse.y <= playY + btnH) {
                            onScreenFlip = [&] { music.stop(); };
                            startTransition(GameScreen::CHARACTER_SELECT); 
                        }

                            // boton de reliquias
                        if (mouse.x >= btnX && mouse.x <= btnX + btnW && mouse.y >= relicsY && mouse.y <= relicsY + btnH) {
                            onScreenFlip = [&] { RelicTracker::resetScroll(); };
                            startTransition(GameScreen::RELIC_TRACKER);
                        }

                        // Boton QUIT
                        if (mouse.x >= btnX && mouse.x <= btnX + btnW && mouse.y >= quitY && mouse.y <= quitY + btnH)
                            window.close();
                    }
                    else if (currentScreen == GameScreen::CHARACTER_SELECT) {
                        int clicked = handleCharacterSelectClick(mouse, cards, cardW, cardH, cardTop);
                        if (clicked != -1) selectedCard = clicked;
                    }

                    //Nueva, seleccion de move en combate, ahora con botones estilo pokemon
                    else if (currentScreen == GameScreen::COMBAT && player && gameLoop.getCombat() 
                            && !gameLoop.getCombat()->isBattleOver()) {

                        // medidas de los botones, igual que en drawCombat
                        const float bW = 170.f, bH = 58.f, bGap = 8.f;
                        const float btnAreaX = 390.f;
                        const float btnAreaW = 800.f - btnAreaX;
                        const float bStartX = btnAreaX + (btnAreaW - bW * 2.f - bGap) / 2.f;
                        const float panelY  = 440.f;
                        const float bRow1Y  = panelY + 20.f;
                        const float bRow2Y  = panelY + 20.f + bH + bGap;
                        float bx[4] = { bStartX, bStartX+bW+bGap, bStartX, bStartX+bW+bGap };
                        float by[4] = { bRow1Y, bRow1Y, bRow2Y, bRow2Y };

                        if (showingMoves) { //si se dio click en moves, ahora los 4 botones son los moves
                            
                            // click en BACK regresa al menu sin atacar, ahora primero por el overlay
                            const float backW = 100.f, backH = 30.f;
                            float backX = btnAreaX + (btnAreaW - backW) / 2.f;
                            float backY = panelY + 185.f - backH - 20.f;
                            if (mouse.x >= backX && mouse.x <= backX+backW &&
                                mouse.y >= backY && mouse.y <= backY+backH) {
                                showingMoves = false;
                            }
                            else {
                                // click en un move ataca con ese move
                                for (int i = 0; i < 4; i++) {
                                    if (mouse.x >= bx[i] && mouse.x <= bx[i]+bW &&
                                        mouse.y >= by[i] && mouse.y <= by[i]+bH) {
                                        gameLoop.advanceTurn(i);
                                        break;
                                    }
                                }
                            }
                        } 
                        else {
                            // click en MOVES abre el panel de moves
                            if (mouse.x >= bx[0] && mouse.x <= bx[0]+bW &&
                                mouse.y >= by[0] && mouse.y <= by[0]+bH) {
                                showingMoves = true;
                            }
                            // STATS/RELICS/POTIONS son solo hover, no necesitan click
                        }
                    }

                    // pop up LEVEL UP, click en uno de los 6 botones de stat
                    else if (showLevelUp && player) {
                        const float luBtnW = 260.f, luBtnH = 48.f;
                        const float luBtnX = (800.f - luBtnW) / 2.f;
                        float startYs[6] = {195.f, 258.f, 321.f, 384.f, 447.f, 510.f};

                        for (int i = 0; i < 6; i++) {
                            if (mouse.x >= luBtnX && mouse.x <= luBtnX + luBtnW &&
                                mouse.y >= startYs[i] && mouse.y <= startYs[i] + luBtnH) {
                                player->applyStatBoost(i + 1);
                                if (!player->needsStatBoost()) {
                                    showLevelUp = false;
                                    if (player->hasPendingMoveRelic())
                                        showMoveSelect = true;
                                    else if (currentScreen == GameScreen::COMBAT) {
                                        // Coming from combat, advance normally
                                        startTransition(gameLoop.onCombatFinished());
                                    } else {
                                        // Coming from event door, use stored next screen
                                        startTransition(nextScreenAfterLevelUp);
                                    }
                                }
                                break;
                            }
                        }
                    }
                    // new pop up MOVE SELECT: click en uno de los 4 slots
                    else if (showMoveSelect && player) {
                        const float msBtnW = 320.f, msBtnH = 55.f;
                        const float msBtnX = (800.f - msBtnW) / 2.f;
                        const float msStartY = 185.f;

                        for (int i = 0; i < 4; i++) {
                            float by = msStartY + i * (msBtnH + 18.f);
                            if (mouse.x >= msBtnX && mouse.x <= msBtnX + msBtnW &&
                                mouse.y >= by && mouse.y <= by + msBtnH) {
                                player->applyPendingMove(i);
                                showMoveSelect = false;
                                if (currentScreen == GameScreen::COMBAT) {
                                    // Coming from combat, advance normally
                                    startTransition(gameLoop.onCombatFinished());
                                } else {
                                    // Coming from event door, use stored next screen
                                    startTransition(nextScreenAfterLevelUp);
                                }
                                break;
                            }
                        }
                    } 
                } 
            }

            // Mouse scroll
            if (const auto* scroll = event->getIf<sf::Event::MouseWheelScrolled>()) {
                if (currentScreen == GameScreen::RELIC_TRACKER) {
                    RelicTracker::handleScroll(scroll->delta);
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
                        startTransition(GameScreen::DOOR_SELECT);
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
                        showingMoves = false; //resetear ambos
                            // Capture current preview stat strings so the UI doesn't
                            // immediately update when selectDoor mutates floors/previews. i couldnt use onScreenFlip
                            auto highestStatName = [&](const Enemy* e) -> std::string {
                                if (!e) return "ATK";
                                int h = e->getHighestStat();
                                if (h == e->getAtk()) return "ATK";
                                if (h == e->getDef()) return "DEF";
                                if (h == e->getSpAtk()) return "SP.ATK";
                                if (h == e->getSpDef()) return "SP.DEF";
                                return "SPD";
                            };

                            for (int i = 0; i < 3; ++i) {
                                Enemy* pe = gameLoop.getPreviewEnemy(i);
                                frozenStatStr[i] = (pe) ? highestStatName(pe) : "ATK";
                            }
                            freezeDoorPreview = true;

                            GameScreen result = gameLoop.selectDoor(doorIdx);

                            // Always clear the freeze at the actual screen flip.
                            if (result == GameScreen::EVENT) {
                                onScreenFlip = [&] {
                                    freezeDoorPreview = false;
                                    nextScreenAfterLevelUp = gameLoop.onEventDismissed();
                                };
                                startTransition(GameScreen::EVENT);
                                // level up / move select handled when player presses ENTER on EVENT screen
                            } else {
                                onScreenFlip = [&] { freezeDoorPreview = false; };
                                nextScreenAfterLevelUp = result;
                                if (player && player->needsStatBoost()) {
                                    showLevelUp = true;
                                } else {
                                    startTransition(result);
                                }
                            }
                    }
                }

                // main combat
                else if (currentScreen == GameScreen::COMBAT) {
                    //se borro el space para avanzar turno 
                    
                    if (keyEvent->code == sf::Keyboard::Key::P) {
                        gameLoop.usePotionInCombat();
                    }
                    else if (keyEvent->code == sf::Keyboard::Key::Enter) {
                        // Solo avanza si el combate ya termino y se entregaron recompensas
                        CombatEngine* combat = gameLoop.getCombat();
                        if (combat && combat->isBattleOver() && rewardGiven) {
                            if (player->needsStatBoost()) {
                                showLevelUp = true; //nuevo pop up para elegir stat boost
                                nextScreenAfterLevelUp = GameScreen::COMBAT; // Mark we came from combat
                            }
                            else if (player->hasPendingMoveRelic())
                                showMoveSelect = true; //new pop up para elegir que move boostear o reemplazar con MoveRelic
                            else
                                startTransition(gameLoop.onCombatFinished());
                            // Si el siguiente estado es DOOR_SELECT, rewardGiven
                            // se reseteara al elegir la siguiente puerta
                        }
                    }
                }

                else if (currentScreen == GameScreen::EVENT) {
                    if (keyEvent->code == sf::Keyboard::Key::Enter || keyEvent->code == sf::Keyboard::Key::Space) {
                        if (player->needsStatBoost()) {
                        showLevelUp = true;
                        // nextScreenAfterLevelUp already set when we entered EVENT
                        } else if (player->hasPendingMoveRelic()) {
                            showMoveSelect = true;
                        } else startTransition(gameLoop.onEventDismissed());
                    }
                }

                else if (currentScreen == GameScreen::REWARDS) {
                    if (keyEvent->code == sf::Keyboard::Key::Enter || keyEvent->code == sf::Keyboard::Key::Space) {
                        onScreenFlip = [&] { combatRewardDescription = ""; };
                        startTransition(gameLoop.onRewardsDismissed());
                    }
                }

                else if (currentScreen == GameScreen::RELIC_TRACKER) {
                    if (keyEvent->code == sf::Keyboard::Key::B) {
                        onScreenFlip = [&] { RelicTracker::resetScroll(); };
                        startTransition(GameScreen::MAIN_MENU);
                    }
                }

                // Game Over / Win / Final Boss  R para volver al menu
                else if (currentScreen == GameScreen::GAME_OVER || currentScreen == GameScreen::WIN) {
                    if (keyEvent->code == sf::Keyboard::Key::R) {
                        onScreenFlip = [&] {
                            gameLoop.reset();
                            delete player;
                            player = nullptr;
                            selectedCard = -1;
                            rewardGiven = false; 
                            showingMoves = false;
                            music.setLooping(true);
                            music.setVolume(50.f);
                            music.play(); 
                        };
                        startTransition(GameScreen::MAIN_MENU);
                    }
                }
            }
        }
  
        
        window.clear(sf::Color(10, 5, 0));

        //bloques de dibujo por pantalla

        if (currentScreen == GameScreen::MAIN_MENU) {
            sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            drawMainMenu(window, font, fontSmall, fontTitle, bgSprite, titleText, subtitleText, btnX, playY, relicsY, quitY, btnW, btnH, mouse);
        }
        
        else if (currentScreen == GameScreen::RELIC_TRACKER) {
            RelicTracker::drawRelicTracker(window, font, fontSmall, bgSprite);        }

        else if (currentScreen == GameScreen::CHARACTER_SELECT) {
            drawCharacterSelect(window, font, csSprite, cards, selectedCard, cardW, cardH, cardTop, timePassed);
        }   
        else if (currentScreen == GameScreen::DOOR_SELECT && gameLoop.getCurrentFloor() && player) {
            Enemy *previews[3] = { gameLoop.getPreviewEnemy(0), gameLoop.getPreviewEnemy(1), gameLoop.getPreviewEnemy(2) };
            if (freezeDoorPreview)
                drawDoorSelect(window, font, wallSprite, gameLoop.getCurrentFloor(), player, gameLoop.getFloorNumber(), previews, doorSprite, doorCorruptSprite, frozenStatStr);
            else
                drawDoorSelect(window, font, wallSprite, gameLoop.getCurrentFloor(), player, gameLoop.getFloorNumber(), previews, doorSprite, doorCorruptSprite);
            if (showLevelUp){
                // aqui iria el drawEvent o algo por el estilo
                sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                drawLevelUp(window, font, player, mouse);
            }
            else if (showMoveSelect) {
                sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                drawMoveSelect(window, font, player, mouse);
            }
        }
        else if (currentScreen == GameScreen::COMBAT && player && gameLoop.getCombat() && gameLoop.getCurrentFloor() && gameLoop.getCurrentEnemy()) {
            drawCombat(window, font, player, gameLoop.getCurrentEnemy(), gameLoop.getCombat(), playerText, enemyText, 
                        logText, resultText, instructionText, rewardGiven, combatSprite, 
                        gameLoop.getCurrentFloor(), gameLoop.getIsMiniBoss(), combatRewardDescription,
                        showingMoves, window.mapPixelToCoords(sf::Mouse::getPosition(window)));
            
            // Pop-ups encima del combate
            if (showLevelUp) {
                sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                drawLevelUp(window, font, player, mouse);
            }
            else if (showMoveSelect) {
                sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                drawMoveSelect(window, font, player, mouse);
            }
        }
        else if (currentScreen == GameScreen::EVENT && player) {
            window.draw(eventSprite); 
            drawEventScreen(window, font, gameLoop.getRewardDescription(), player, timePassed);
            if (showLevelUp) {
                sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                drawLevelUp(window, font, player, mouse);
            } else if (showMoveSelect) {
                sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                drawMoveSelect(window, font, player, mouse);
            }
        }

        else if (currentScreen == GameScreen::REWARDS && player) {
            window.draw(rewardsSprite);
            drawRewardScreen(window, font, combatRewardDescription, player, timePassed);
            // level-up / move-select overlays in case they leveled up mid-reward
            if (showLevelUp) {
                sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                drawLevelUp(window, font, player, mouse);
            } else if (showMoveSelect) {
                sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                drawMoveSelect(window, font, player, mouse);
            }
        }

        else if (currentScreen == GameScreen::GAME_OVER) {
            // Dibuja el ultimo frame de combate como fondo (si existe)
            if (gameLoop.getCurrentFloor() && gameLoop.getCurrentEnemy() && gameLoop.getCombat() && player) {
                drawCombat(window, font, player, gameLoop.getCurrentEnemy(), gameLoop.getCombat(), playerText, enemyText, logText, resultText,
                           instructionText, rewardGiven, combatSprite, gameLoop.getCurrentFloor(), gameLoop.getIsMiniBoss(), combatRewardDescription,
                           false, {0.f, 0.f}); //en esta pantalla no se muestra el menu
            }
            drawGameOver(window, font);
        }
        else if (currentScreen == GameScreen::WIN) {
            drawWin(window, font);
        }

        window.draw(transitionOverlay); // Dibuja la transition por encima de todo
        window.display();
    }

    // memory cleanup
    
    gameLoop.reset();   // destruye Floor, Enemy, CombatEngine
    delete player;      // main.cpp es el owner del Player

    return 0;
}