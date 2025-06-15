#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <array>
#include <functional>
#include "../include/PlayerHealth.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "BossFight2nd");

    // --- Fondo ---
    sf::Texture fondoTexture;
    fondoTexture.loadFromFile("assets/images/Fondo Espacial.png");
    sf::Sprite fondo(fondoTexture);
    fondo.setPosition(0, 550);
    fondo.setScale(800.0f / fondoTexture.getSize().x, 250.0f / fondoTexture.getSize().y);

    // --- Player ---
    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("assets/images/space_Player.png"))
        return -1;
    sf::Sprite player(playerTexture);
    player.setPosition(400, 700);
    PlayerHealth playerHP(100);

    // --- Sprites de vida del jugador ---
    sf::Texture hp100Texture, hp80Texture, hp60Texture, hp40Texture, hp20Texture, hp0Texture, timeoutTexture;
    hp100Texture.loadFromFile("assets/images/100 HP.png");
    hp80Texture.loadFromFile("assets/images/80 HP.png");
    hp60Texture.loadFromFile("assets/images/60 HP.png");
    hp40Texture.loadFromFile("assets/images/40 HP.png");
    hp20Texture.loadFromFile("assets/images/20 HP.png");
    hp0Texture.loadFromFile("assets/images/0 HP.png");
    timeoutTexture.loadFromFile("assets/images/Timeout.png");
    playerHP.setTextures(hp100Texture, hp80Texture, hp60Texture, hp40Texture, hp20Texture, hp0Texture, timeoutTexture);

    // --- Shootout (Bala) ---
    sf::Texture bulletTexture;
    if (!bulletTexture.loadFromFile("assets/images/DisparoNave.png"))
        return -1;
    std::vector<sf::Sprite> bullets;
    float bulletSpeed = 1.5f;

    // --- Blinky (Enemigo animado) ---
    sf::Texture blinkyLTexture, blinkyRTexture;
    blinkyLTexture.loadFromFile("assets/images/Boss2nd/BlinkyL.png");
    blinkyRTexture.loadFromFile("assets/images/Boss2nd/BlinkyR.png");
    sf::Sprite blinkySprite(blinkyRTexture);
    blinkySprite.setPosition(0, 550);
    float blinkySpeed = 200.0f;
    int blinkyDirectionFlag = 0; // 0: izq->der, 1: der->izq
    bool blinkyMoving = false;
    bool blinkyVisible = true;
    sf::Clock blinkyTimer;
    float blinkyInterval = 3.0f; // antes 8.0f, ahora más rápido

    // --- Inky (Enemigo animado) ---
    sf::Texture inkyLTexture, inkyRTexture;
    inkyLTexture.loadFromFile("assets/images/Boss2nd/InkyL.png");
    inkyRTexture.loadFromFile("assets/images/Boss2nd/InkyR.png");
    sf::Sprite inkySprite(inkyRTexture);
    inkySprite.setPosition(0, 612);
    float inkySpeed = 200.0f;
    int inkyDirectionFlag = 0; // 0: izq->der, 1: der->izq
    bool inkyMoving = false;
    int inkyState = 0; // 0: quieto, 1: moviéndose
    sf::Clock inkyTimer, inkyAppearTimer;
    float inkyInterval = 4.0f; // antes 10.0f, ahora más rápido

    // --- Clayd (Enemigo animado) ---
    sf::Texture claydLTexture, claydRTexture;
    claydLTexture.loadFromFile("assets/images/Boss2nd/ClaydL.png");
    claydRTexture.loadFromFile("assets/images/Boss2nd/ClaydR.png");
    sf::Sprite claydSprite(claydRTexture);
    claydSprite.setPosition(0, 736);
    float claydSpeed = 200.0f;
    int claydDirectionFlag = 0; // 0: izq->der, 1: der->izq
    bool claydMoving = false;
    sf::Clock claydTimer, claydAppearTimer;
    float claydInterval = 2.5f; // antes 7.0f, ahora más rápido
    int claydState = 0; // 0: Blinky, 1: Inky
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // --- Pinky (Enemigo animado, lógica propia) ---
    sf::Texture pinkyLTexture, pinkyRTexture;
    pinkyLTexture.loadFromFile("assets/images/Boss2nd/PinkyL.png");
    pinkyRTexture.loadFromFile("assets/images/Boss2nd/PinkyR.png");
    sf::Sprite pinkySprite(pinkyRTexture);
    pinkySprite.setPosition(0, 674); // Altura 674 Y
    float pinkySpeed = 200.0f;
    int pinkyState = 0;
    bool pinkyActive = false;
    sf::Clock pinkyDelayClock;
    sf::Clock pinkyPostCycleClock;
    bool pinkyPostCycleDelay = false;

    // --- Pac (Jefe, estado 0) ---
    sf::Texture pacLTexture, pacRTexture;
    pacLTexture.loadFromFile("assets/images/Boss2nd/PACmove2 (Left).png");
    pacRTexture.loadFromFile("assets/images/Boss2nd/PACmove2 (Right).png");
    std::vector<sf::Texture> pacRFrames(2), pacLFrames(2);
    pacRFrames[0].loadFromFile("assets/images/Boss2nd/PACmove2 (Right).png");
    pacRFrames[1].loadFromFile("assets/images/Boss2nd/PACmove3 (Right).png");
    pacLFrames[0].loadFromFile("assets/images/Boss2nd/PACmove2 (Left).png");
    pacLFrames[1].loadFromFile("assets/images/Boss2nd/PACmove3 (Left).png");
    // Sprites up/down para estado de hambre
    std::vector<sf::Texture> pacUFrames(2), pacDFrames(2);
    pacUFrames[0].loadFromFile("assets/images/Boss2nd/PACmove2 (Up).png");
    pacUFrames[1].loadFromFile("assets/images/Boss2nd/PACmove3 (Up).png");
    pacDFrames[0].loadFromFile("assets/images/Boss2nd/PACmove2 (Down).png");
    pacDFrames[1].loadFromFile("assets/images/Boss2nd/PACmove3 (Down).png");
    sf::Sprite pacSprite(pacRTexture);
    pacSprite.setPosition(400, 0);
    float pacSpeed = 600.0f;
    float pacHungrySpeed = 1400.0f; // antes 1000.0f
    int pacDirection = (std::rand() % 2 == 0) ? 0 : 1; // 0: derecha, 1: izquierda
    int pacState = 0;
    int pacAnimFrame = 0;
    float pacAnimTime = 0.0f;
    float pacAnimInterval = 0.15f;
    bool pacHungryDescending = false;
    int pacHP = 400; // Pacman inicia con 400 puntos de vida

    // --- Generador de Goals (mecánica de GeneradorBF2.cpp) ---
    sf::Texture goalTexture;
    goalTexture.loadFromFile("assets/images/Boss2nd/Goal.png");
    struct GoalObj {
        sf::Sprite sprite;
        int x, y;
        int hp = 20; // Cada Goal tiene 20 puntos de vida
    };
    std::vector<GoalObj> goals;
    sf::Clock goalSpawnClock;
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    sf::Clock clock;
    bool canShoot = true;
    while (window.isOpen()) {
        // --- Generar Goals cada 2 segundos si hay menos de 5 ---
        if (goalSpawnClock.getElapsedTime().asSeconds() >= 2.0f && goals.size() < 5) {
            int x = 32 + std::rand() % (768 - 32 + 1);
            int y = 32 + std::rand() % (500 - 32 + 1);
            bool exists = false;
            for (const auto& g : goals) {
                if (g.x == x && g.y == y) {
                    exists = true;
                    break;
                }
            }
            if (!exists) {
                GoalObj obj;
                obj.sprite.setTexture(goalTexture);
                obj.sprite.setPosition(static_cast<float>(x), static_cast<float>(y));
                obj.x = x;
                obj.y = y;
                goals.push_back(obj);
                goalSpawnClock.restart();
            }
        }

        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        // Movimiento del jugador
        float velocidad = 0.5f;
        sf::Vector2f pos = player.getPosition();
        sf::Vector2u size = playerTexture.getSize();
        float minX = 0;
        float minY = 550;
        float maxX = 800 - size.x;
        float maxY = 800 - size.y;
        if (playerHP.isAlive()) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                if (pos.y - velocidad >= minY)
                    player.move(0, -velocidad);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                if (pos.y + velocidad <= maxY)
                    player.move(0, velocidad);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                if (pos.x - velocidad >= minX)
                    player.move(-velocidad, 0);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                if (pos.x + velocidad <= maxX)
                    player.move(velocidad, 0);
        }
        // Disparo (tecla Space)
        if (playerHP.isAlive() && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            if (canShoot) {
                sf::Sprite bullet(bulletTexture);
                bullet.setPosition(player.getPosition().x + size.x / 2 - bulletTexture.getSize().x / 2, player.getPosition().y - bulletTexture.getSize().y);
                bullets.push_back(bullet);
                canShoot = false;
            }
        } else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            canShoot = true;
        }
        // Mover balas
        for (auto& bullet : bullets) {
            bullet.move(0, -bulletSpeed);
        }
        // Eliminar balas fuera de pantalla
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const sf::Sprite& b) {
            return b.getPosition().y + b.getTexture()->getSize().y < 0;
        }), bullets.end());

        float deltaTime = clock.restart().asSeconds();

        // --- Daño al jugador por colisión con enemigos (con invulnerabilidad Tabletop) ---
        static bool pacGoingDown = true;
        if (playerHP.isAlive() && !playerHP.isInvulnerable()) {
            bool damaged = false;
            sf::FloatRect playerBounds = player.getGlobalBounds();
            // Blinky
            if (blinkyMoving && blinkySprite.getGlobalBounds().intersects(playerBounds)) {
                playerHP.takeDamage(20);
                damaged = true;
            }
            // Inky
            if (inkyMoving && inkySprite.getGlobalBounds().intersects(playerBounds)) {
                playerHP.takeDamage(20);
                damaged = true;
            }
            // Clayd
            if (claydMoving && claydSprite.getGlobalBounds().intersects(playerBounds)) {
                playerHP.takeDamage(20);
                damaged = true;
            }
            // Pinky
            if (pinkyActive && !pinkyPostCycleDelay && pinkySprite.getGlobalBounds().intersects(playerBounds)) {
                playerHP.takeDamage(20);
                damaged = true;
            }
            // Pac (daño siempre que hay contacto)
            if (pacSprite.getGlobalBounds().intersects(playerBounds)) {
                playerHP.takeDamage(20);
                damaged = true;
            }
        }
        // --- Actualizar estado de invulnerabilidad y sprite de vida ---
        playerHP.update();

        // --- Blinky: lógica de temporizador y movimiento ---
        if (!blinkyMoving && blinkyTimer.getElapsedTime().asSeconds() >= blinkyInterval) {
            blinkyMoving = true;
            if (blinkyDirectionFlag == 0) {
                blinkySprite.setTexture(blinkyRTexture);
                blinkySprite.setPosition(0, 550);
            } else {
                blinkySprite.setTexture(blinkyLTexture);
                blinkySprite.setPosition(800 - blinkySprite.getGlobalBounds().width, 550);
            }
        }
        if (blinkyMoving) {
            float dir = (blinkyDirectionFlag == 0) ? 1.f : -1.f;
            blinkySprite.move(blinkySpeed * dir * deltaTime, 0);
            // Parar solo cuando Blinky haya salido completamente de la pantalla
            if (blinkyDirectionFlag == 0 && blinkySprite.getPosition().x > 800) {
                blinkyMoving = false;
                blinkyDirectionFlag = 1;
                blinkyTimer.restart();
            } else if (blinkyDirectionFlag == 1 && blinkySprite.getPosition().x + blinkySprite.getGlobalBounds().width < 0) {
                blinkyMoving = false;
                blinkyDirectionFlag = 0;
                blinkyTimer.restart();
            }
        }

        // --- Inky: lógica de aparición, espera y movimiento ---
        if (!inkyMoving && inkyTimer.getElapsedTime().asSeconds() >= inkyInterval) {
            inkyMoving = true;
            inkyState = 0;
            inkyAppearTimer.restart();
            if (inkyDirectionFlag == 0) {
                inkySprite.setTexture(inkyRTexture);
                inkySprite.setPosition(0, 612);
            } else {
                inkySprite.setTexture(inkyLTexture);
                inkySprite.setPosition(800 - inkySprite.getGlobalBounds().width, 612);
            }
        }
        if (inkyMoving) {
            if (inkyState == 0) {
                // Espera 0.5 segundos antes de moverse (antes 2.0f)
                if (inkyAppearTimer.getElapsedTime().asSeconds() >= 0.5f) {
                    inkyState = 1;
                }
            } else if (inkyState == 1) {
                float fastSpeed = 400.0f; // velocidad mayor
                float dir = (inkyDirectionFlag == 0) ? 1.f : -1.f;
                inkySprite.move(fastSpeed * dir * deltaTime, 0);
                if (inkyDirectionFlag == 0 && inkySprite.getPosition().x > 800) {
                    inkyMoving = false;
                    inkyDirectionFlag = 1;
                    inkyTimer.restart();
                } else if (inkyDirectionFlag == 1 && inkySprite.getPosition().x + inkySprite.getGlobalBounds().width < 0) {
                    inkyMoving = false;
                    inkyDirectionFlag = 0;
                    inkyTimer.restart();
                }
            }
        }

        // --- Clayd: lógica de temporizador y movimiento aleatorio ---
        if (!claydMoving && claydTimer.getElapsedTime().asSeconds() >= claydInterval) {
            claydMoving = true;
            claydState = std::rand() % 2; // 0 o 1
            if (claydState == 0) { // Blinky
                if (claydDirectionFlag == 0) {
                    claydSprite.setTexture(claydRTexture);
                    claydSprite.setPosition(0, 736);
                } else {
                    claydSprite.setTexture(claydLTexture);
                    claydSprite.setPosition(800 - claydSprite.getGlobalBounds().width, 736);
                }
            } else { // Inky
                claydAppearTimer.restart();
                if (claydDirectionFlag == 0) {
                    claydSprite.setTexture(claydRTexture);
                    claydSprite.setPosition(0, 736);
                } else {
                    claydSprite.setTexture(claydLTexture);
                    claydSprite.setPosition(800 - claydSprite.getGlobalBounds().width, 736);
                }
            }
        }
        if (claydMoving) {
            if (claydState == 0) { // Blinky
                float dir = (claydDirectionFlag == 0) ? 1.f : -1.f;
                claydSprite.move(claydSpeed * dir * deltaTime, 0);
                if (claydDirectionFlag == 0 && claydSprite.getPosition().x > 800) {
                    claydMoving = false;
                    claydDirectionFlag = 1;
                    claydTimer.restart();
                } else if (claydDirectionFlag == 1 && claydSprite.getPosition().x + claydSprite.getGlobalBounds().width < 0) {
                    claydMoving = false;
                    claydDirectionFlag = 0;
                    claydTimer.restart();
                }
            } else if (claydState == 1) { // Inky
                if (claydAppearTimer.getElapsedTime().asSeconds() < 0.5f) { // antes 2.0f
                    // Quieto 0.5 segundos
                } else {
                    float fastSpeed = 400.0f;
                    float dir = (claydDirectionFlag == 0) ? 1.f : -1.f;
                    claydSprite.move(fastSpeed * dir * deltaTime, 0);
                    if (claydDirectionFlag == 0 && claydSprite.getPosition().x > 800) {
                        claydMoving = false;
                        claydDirectionFlag = 1;
                        claydTimer.restart();
                    } else if (claydDirectionFlag == 1 && claydSprite.getPosition().x + claydSprite.getGlobalBounds().width < 0) {
                        claydMoving = false;
                        claydDirectionFlag = 0;
                        claydTimer.restart();
                    }
                }
            }
        }

        // --- Pinky: lógica de movimiento por estados personalizados (de Pinky.cpp) ---
        if (!pinkyActive) {
            if (pinkyDelayClock.getElapsedTime().asSeconds() >= 2.0f) { // antes 6.0f
                pinkyActive = true;
            }
        }
        if (pinkyActive && !pinkyPostCycleDelay) {
            switch (pinkyState) {
                case 0: // Izquierda a derecha hasta X=300
                    pinkySprite.setTexture(pinkyRTexture);
                    pinkySprite.move(pinkySpeed * deltaTime, 0);
                    pinkySprite.setPosition(pinkySprite.getPosition().x, 674);
                    if (pinkySprite.getPosition().x >= 300) {
                        pinkySprite.setPosition(300, 674);
                        pinkyState = 1;
                    }
                    break;
                case 1: // Derecha a izquierda desde 300 hasta salir
                    pinkySprite.setTexture(pinkyLTexture);
                    pinkySprite.move(-pinkySpeed * deltaTime, 0);
                    pinkySprite.setPosition(pinkySprite.getPosition().x, 674);
                    if (pinkySprite.getPosition().x + pinkySprite.getGlobalBounds().width < 0) {
                        pinkySprite.setPosition(800 - pinkySprite.getGlobalBounds().width, 674);
                        pinkyState = 2;
                    }
                    break;
                case 2: // Derecha a izquierda hasta X=500
                    pinkySprite.setTexture(pinkyLTexture);
                    pinkySprite.move(-pinkySpeed * deltaTime, 0);
                    pinkySprite.setPosition(pinkySprite.getPosition().x, 674);
                    if (pinkySprite.getPosition().x <= 500) {
                        pinkySprite.setPosition(500, 674);
                        pinkyState = 3;
                    }
                    break;
                case 3: // Izquierda a derecha desde 500 hasta salir
                    pinkySprite.setTexture(pinkyRTexture);
                    pinkySprite.move(pinkySpeed * deltaTime, 0);
                    pinkySprite.setPosition(pinkySprite.getPosition().x, 674);
                    if (pinkySprite.getPosition().x > 800) {
                        pinkySprite.setPosition(0, 674);
                        pinkyState = 0;
                        pinkyPostCycleDelay = true;
                        pinkyPostCycleClock.restart();
                    }
                    break;
            }
        }
        if (pinkyPostCycleDelay && pinkyPostCycleClock.getElapsedTime().asSeconds() >= 1.0f) { // antes 3.0f
            pinkyPostCycleDelay = false;
            pinkyDelayClock.restart();
        }

        // --- Pac: lógica de movimiento y animación estado 0 y modo hambriento (1) ---
        pacAnimTime += deltaTime;
        if (pacAnimTime >= pacAnimInterval) {
            pacAnimFrame = (pacAnimFrame + 1) % 2;
            pacAnimTime = 0.0f;
        }
        // Detener completamente a Pac si su HP llega a 0
        if (pacHP <= 0) {
            // No mover ni animar Pac
            // Opcional: podrías cambiar el sprite a uno de "derrota" aquí
        } else {
            // Colisión Pac-Goal: activa modo hambriento y elimina el Goal tocado
            if (pacState == 0) {
                for (auto it = goals.begin(); it != goals.end(); ) {
                    if (pacGoingDown && pacSprite.getGlobalBounds().intersects(it->sprite.getGlobalBounds())) {
                        pacState = 1;
                        pacHungryDescending = false;
                        pacHP += 20;
                        if (pacHP > 400) pacHP = 400;
                        it = goals.erase(it);
                        break;
                    } else {
                        ++it;
                    }
                }
            }
            if (pacState == 0) {
                // Determinar si Pac debe cambiar a modo hambriento
                // (Ejemplo: aquí puedes poner una condición para activar el modo hambriento)
                // if (algunaCondicion) pacState = 1;
                if (pacGoingDown) {
                    if (pacSprite.getPosition().y < 550) {
                        // Baja
                        if (pacDirection == 0) {
                            pacSprite.setTexture(pacRFrames[pacAnimFrame]);
                            pacSprite.move(pacSpeed * deltaTime, 0);
                            if (pacSprite.getPosition().x >= 800 - pacSprite.getGlobalBounds().width) {
                                pacSprite.setPosition(800 - pacSprite.getGlobalBounds().width, pacSprite.getPosition().y + 40);
                                pacDirection = 1;
                            }
                        } else {
                            pacSprite.setTexture(pacLFrames[pacAnimFrame]);
                            pacSprite.move(-pacSpeed * deltaTime, 0);
                            if (pacSprite.getPosition().x <= 0) {
                                pacSprite.setPosition(0, pacSprite.getPosition().y + 40);
                                pacDirection = 0;
                            }
                        }
                        if (pacSprite.getPosition().y >= 550) {
                            pacSprite.setPosition(pacSprite.getPosition().x, 550);
                            pacGoingDown = false;
                        }
                    }
                } else {
                    // Sube
                    if (pacDirection == 0) {
                        pacSprite.setTexture(pacRFrames[pacAnimFrame]);
                        pacSprite.move(pacSpeed * deltaTime, 0);
                        if (pacSprite.getPosition().x >= 800 - pacSprite.getGlobalBounds().width) {
                            pacSprite.setPosition(800 - pacSprite.getGlobalBounds().width, pacSprite.getPosition().y - 40);
                            pacDirection = 1;
                        }
                    } else {
                        pacSprite.setTexture(pacLFrames[pacAnimFrame]);
                        pacSprite.move(-pacSpeed * deltaTime, 0);
                        if (pacSprite.getPosition().x <= 0) {
                            pacSprite.setPosition(0, pacSprite.getPosition().y - 40);
                            pacDirection = 0;
                        }
                    }
                    if (pacSprite.getPosition().y <= 0) {
                        pacSprite.setPosition(pacSprite.getPosition().x, 0);
                        pacGoingDown = true;
                    }
                }
            } else if (pacState == 1) {
                // Modo hambriento: usar sprites up/down
                if (!pacHungryDescending) {
                    // Baja rápidamente hasta 800 Y
                    pacSprite.setTexture(pacDFrames[pacAnimFrame]);
                    pacSprite.move(0, pacHungrySpeed * deltaTime);
                    if (pacSprite.getPosition().y >= 800) {
                        pacSprite.setPosition(pacSprite.getPosition().x, 800);
                        pacHungryDescending = true;
                        // Dirección aleatoria al llegar abajo
                        pacDirection = (std::rand() % 2 == 0) ? 0 : 1;
                    }
                } else {
                    // Rebote izquierda/derecha en la base (más lento)
                    float hungryBounceSpeed = 400.0f; // antes pacHungrySpeed, ahora más lento
                    if (pacDirection == 0) {
                        pacSprite.setTexture(pacRFrames[pacAnimFrame]);
                        pacSprite.move(hungryBounceSpeed * deltaTime, 0);
                        if (pacSprite.getPosition().x >= 800 - pacSprite.getGlobalBounds().width) {
                            pacSprite.setPosition(800 - pacSprite.getGlobalBounds().width, pacSprite.getPosition().y);
                            pacDirection = 1;
                        }
                    } else {
                        pacSprite.setTexture(pacLFrames[pacAnimFrame]);
                        pacSprite.move(-hungryBounceSpeed * deltaTime, 0);
                        if (pacSprite.getPosition().x <= 0) {
                            pacSprite.setPosition(0, pacSprite.getPosition().y);
                            pacDirection = 0;
                        }
                    }
                    // Sube si llega a 0 Y
                    if (pacSprite.getPosition().y > 0) {
                        pacSprite.setTexture(pacUFrames[pacAnimFrame]);
                        pacSprite.move(0, -pacHungrySpeed * deltaTime * 0.5f);
                        if (pacSprite.getPosition().y <= 0) {
                            pacSprite.setPosition(pacSprite.getPosition().x, 0);
                            pacState = 0;
                            pacHungryDescending = false;
                            pacGoingDown = true;
                        }
                    }
                }
            }
        }

        // --- Daño de balas a Pac y Goals ---
        for (auto bulletIt = bullets.begin(); bulletIt != bullets.end(); ) {
            bool bulletRemoved = false;
            sf::FloatRect bulletBounds = bulletIt->getGlobalBounds();
            // Daño a Pac
            if (pacSprite.getGlobalBounds().intersects(bulletBounds) && pacHP > 0) {
                pacHP -= 20;
                bulletIt = bullets.erase(bulletIt);
                continue;
            }
            // Daño a Goals
            for (auto goalIt = goals.begin(); goalIt != goals.end(); ++goalIt) {
                if (goalIt->sprite.getGlobalBounds().intersects(bulletBounds) && goalIt->hp > 0) {
                    goalIt->hp -= 20;
                    bulletIt = bullets.erase(bulletIt);
                    if (goalIt->hp <= 0) {
                        goals.erase(goalIt);
                    }
                    bulletRemoved = true;
                    break;
                }
            }
            if (!bulletRemoved) ++bulletIt;
        }

        // --- Renderizado ---
        window.clear();
        window.draw(fondo);
        window.draw(playerHP.getSprite());
        if (playerHP.isAlive())
            window.draw(player);
        for (const auto& bullet : bullets)
            window.draw(bullet);
        if (blinkyMoving)
            window.draw(blinkySprite);
        if (inkyMoving)
            window.draw(inkySprite);
        if (claydMoving)
            window.draw(claydSprite);
        if (pinkyActive && !pinkyPostCycleDelay)
            window.draw(pinkySprite);
        window.draw(pacSprite);
        for (const auto& g : goals) {
            window.draw(g.sprite);
            // DEBUG: dibujar el hitbox de cada goal
            sf::RectangleShape rect;
            rect.setPosition(g.sprite.getGlobalBounds().left, g.sprite.getGlobalBounds().top);
            rect.setSize(sf::Vector2f(g.sprite.getGlobalBounds().width, g.sprite.getGlobalBounds().height));
            rect.setFillColor(sf::Color::Transparent);
            rect.setOutlineColor(sf::Color::Red);
            rect.setOutlineThickness(1);
            window.draw(rect);
        }
        // DEBUG: dibujar el hitbox de Pac
        sf::RectangleShape pacRect;
        pacRect.setPosition(pacSprite.getGlobalBounds().left, pacSprite.getGlobalBounds().top);
        pacRect.setSize(sf::Vector2f(pacSprite.getGlobalBounds().width, pacSprite.getGlobalBounds().height));
        pacRect.setFillColor(sf::Color::Transparent);
        pacRect.setOutlineColor(sf::Color::Green);
        pacRect.setOutlineThickness(1);
        window.draw(pacRect);
        // DEBUG: dibujar el hitbox de los fantasmas
        if (blinkyMoving) {
            sf::RectangleShape blinkyRect;
            blinkyRect.setPosition(blinkySprite.getGlobalBounds().left, blinkySprite.getGlobalBounds().top);
            blinkyRect.setSize(sf::Vector2f(blinkySprite.getGlobalBounds().width, blinkySprite.getGlobalBounds().height));
            blinkyRect.setFillColor(sf::Color::Transparent);
            blinkyRect.setOutlineColor(sf::Color::Magenta);
            blinkyRect.setOutlineThickness(1);
            window.draw(blinkyRect);
        }
        if (inkyMoving) {
            sf::RectangleShape inkyRect;
            inkyRect.setPosition(inkySprite.getGlobalBounds().left, inkySprite.getGlobalBounds().top);
            inkyRect.setSize(sf::Vector2f(inkySprite.getGlobalBounds().width, inkySprite.getGlobalBounds().height));
            inkyRect.setFillColor(sf::Color::Transparent);
            inkyRect.setOutlineColor(sf::Color::Cyan);
            inkyRect.setOutlineThickness(1);
            window.draw(inkyRect);
        }
        if (claydMoving) {
            sf::RectangleShape claydRect;
            claydRect.setPosition(claydSprite.getGlobalBounds().left, claydSprite.getGlobalBounds().top);
            claydRect.setSize(sf::Vector2f(claydSprite.getGlobalBounds().width, claydSprite.getGlobalBounds().height));
            claydRect.setFillColor(sf::Color::Transparent);
            claydRect.setOutlineColor(sf::Color(255, 165, 0)); // Naranja
            claydRect.setOutlineThickness(1);
            window.draw(claydRect);
        }
        if (pinkyActive && !pinkyPostCycleDelay) {
            sf::RectangleShape pinkyRect;
            pinkyRect.setPosition(pinkySprite.getGlobalBounds().left, pinkySprite.getGlobalBounds().top);
            pinkyRect.setSize(sf::Vector2f(pinkySprite.getGlobalBounds().width, pinkySprite.getGlobalBounds().height));
            pinkyRect.setFillColor(sf::Color::Transparent);
            pinkyRect.setOutlineColor(sf::Color::White);
            pinkyRect.setOutlineThickness(1);
            window.draw(pinkyRect);
        }
        // Mostrar HP de Pacman
        static sf::Font font;
        static bool fontLoaded = false;
        if (!fontLoaded) {
            font.loadFromFile("assets/fonts/AngelicWar.ttf");
            fontLoaded = true;
        }
        sf::Text pacHpText;
        pacHpText.setFont(font);
        pacHpText.setString("PAC HP: " + std::to_string(pacHP));
        pacHpText.setCharacterSize(28);
        pacHpText.setFillColor(sf::Color::Yellow);
        pacHpText.setOutlineColor(sf::Color::Black);
        pacHpText.setOutlineThickness(2);
        pacHpText.setPosition(20, 20);
        window.draw(pacHpText);
        window.display();
    }
    return 0;
}
