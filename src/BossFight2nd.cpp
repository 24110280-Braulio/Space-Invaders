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
    float blinkyInterval = 5.0f;

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
    float inkyInterval = 5.0f;

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
    float claydInterval = 5.0f;
    int claydState = 0; // 0: Blinky, 1: Inky
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    sf::Clock clock;
    bool canShoot = true;
    while (window.isOpen()) {
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
                // Espera 2 segundos antes de moverse
                if (inkyAppearTimer.getElapsedTime().asSeconds() >= 2.0f) {
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
                if (claydAppearTimer.getElapsedTime().asSeconds() < 2.0f) {
                    // Quieto 2 segundos
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

        // --- Renderizado ---
        window.clear();
        window.draw(fondo);
        window.draw(playerHP.getSprite());
        if (playerHP.isAlive())
            window.draw(player);
        for (const auto& bullet : bullets)
            window.draw(bullet);
        if (blinkyMoving) // Solo dibujar Blinky cuando está activo
            window.draw(blinkySprite);
        if (inkyMoving)
            window.draw(inkySprite);
        if (claydMoving)
            window.draw(claydSprite);
        window.display();
    }
    return 0;
}
