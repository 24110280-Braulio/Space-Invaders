#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include "../include/EnemyMovementManager.hpp"
#include "../include/EnemyHealthManager.hpp"
#include "../include/Enemy.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "Tabletop");

    // --- Fondo Espacial ---
    sf::Texture fondoTexture;
    if (!fondoTexture.loadFromFile("assets/images/Fondo Espacial.png"))
        return -1;
    sf::Sprite fondo(fondoTexture);
    fondo.setPosition(0, 550);
    // Ajustar tamaño si es necesario
    fondo.setScale(800.0f / fondoTexture.getSize().x, 250.0f / fondoTexture.getSize().y);

    // --- Barreras ---
    sf::Texture barreraTexture;
    if (!barreraTexture.loadFromFile("assets/images/Barrera.png"))
        return -1;
    const int barreraWidth = 64;
    const int numBarreras = 4;
    const int espacio = (800 - (numBarreras * barreraWidth)) / (numBarreras + 1);
    sf::Sprite barreras[numBarreras];
    for (int i = 0; i < numBarreras; ++i) {
        barreras[i].setTexture(barreraTexture);
        barreras[i].setPosition(espacio + i * (barreraWidth + espacio), 490);
    }

    // --- Player ---
    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("assets/images/space_Player.png"))
        return -1;
    sf::Sprite player(playerTexture);
    player.setPosition(400, 700);

    // --- Shootout (Bala) ---
    sf::Texture bulletTexture;
    if (!bulletTexture.loadFromFile("assets/images/DisparoNave.png"))
        return -1;
    std::vector<sf::Sprite> bullets;
    float bulletSpeed = 1.5f;

    // --- Pea (Enemigos animados tipo Space Invaders) ---
    sf::Texture peaTexture;
    if (!peaTexture.loadFromFile("assets/images/PEA.png"))
        return -1;
    int peaFrameWidth = 44;
    int peaFrameHeight = 32;
    int peaNumFrames = 2;
    int peaCurrentFrame = 0;
    float peaFrameTime = 0.1f;
    float peaAnimTime = 0.0f;
    int numEnemies = 8;
    float peaVelocity = 100.0f;
    std::vector<Enemy> peas;
    std::vector<float> peaVelocities(numEnemies, peaVelocity);
    std::vector<sf::Vector2f> peaPositions;
    std::vector<float> peaDirections(numEnemies, 1.0f);
    for (int i = 0; i < numEnemies; ++i) {
        peas.emplace_back(peaTexture, peaFrameWidth, peaFrameHeight, peaNumFrames, peaFrameTime, 8, 60.0f + i * 90.0f, 60);
        peaPositions.push_back(sf::Vector2f(60.0f + i * 90.0f, 60));
    }
    sf::Clock peaClock;
    std::vector<int> peaHP(numEnemies, 40); // Salud individual para Pea
    std::vector<int> pebHP(numEnemies, 40); // Salud individual para Peb
    std::vector<int> pecHP(numEnemies, 40); // Salud individual para Pec

    // --- Peb (Enemigos animados tipo Peb) ---
    sf::Texture pebTexture;
    if (!pebTexture.loadFromFile("assets/images/PEB.png"))
        return -1;
    int pebFrameWidth = 48;
    int pebFrameHeight = 32;
    int pebNumFrames = 2;
    int pebCurrentFrame = 0;
    float pebFrameTime = .1f;
    float pebAnimTime = 0.0f;
    std::vector<Enemy> pebs;
    std::vector<float> pebVelocities(numEnemies, peaVelocity);
    std::vector<sf::Vector2f> pebPositions;
    std::vector<float> pebDirections(numEnemies, 1.0f);
    for (int i = 0; i < numEnemies; ++i) {
        pebs.emplace_back(pebTexture, pebFrameWidth, pebFrameHeight, pebNumFrames, pebFrameTime, 8, 60.0f + i * 90.0f, 120);
        pebPositions.push_back(sf::Vector2f(60.0f + i * 90.0f, 120));
    }

    // --- Pec (Enemigos animados tipo Pec) ---
    sf::Texture pecTexture;
    if (!pecTexture.loadFromFile("assets/images/PEC.png"))
        return -1;
    int pecFrameWidth = 32;
    int pecFrameHeight = 32;
    int pecNumFrames = 2;
    int pecCurrentFrame = 0;
    float pecFrameTime = .1f;
    float pecAnimTime = 0.0f;
    std::vector<Enemy> pecs;
    std::vector<float> pecVelocities(numEnemies, peaVelocity);
    std::vector<sf::Vector2f> pecPositions;
    std::vector<float> pecDirections(numEnemies, 1.0f);
    for (int i = 0; i < numEnemies; ++i) {
        pecs.emplace_back(pecTexture, pecFrameWidth, pecFrameHeight, pecNumFrames, pecFrameTime, 8, 60.0f + i * 90.0f, 0);
        pecPositions.push_back(sf::Vector2f(60.0f + i * 90.0f, 0));
    }
    // --- Managers ---
    EnemyMovementManager movementManager;
    movementManager.addEnemyGroup(&peaPositions, &peaVelocities, &peaDirections, peaFrameWidth, 32, 768);
    movementManager.addEnemyGroup(&pebPositions, &pebVelocities, &pebDirections, pebFrameWidth, 32, 768);
    movementManager.addEnemyGroup(&pecPositions, &pecVelocities, &pecDirections, pecFrameWidth, 32, 768);
    EnemyHealthManager healthManager;
    healthManager.setHealths(std::vector<int>(numEnemies, 40), std::vector<int>(numEnemies, 40), std::vector<int>(numEnemies, 40));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        // Movimiento del jugador
        float velocidad = 0.5f;
        sf::Vector2f pos = player.getPosition();
        sf::Vector2u size = playerTexture.getSize();
        float minX = 0;
        float minY = 582;
        float maxX = 800 - size.x;
        float maxY = 800 - size.y;
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

        // Disparo (tecla Z)
        static bool canShoot = true;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
            if (canShoot) {
                sf::Sprite bullet(bulletTexture);
                bullet.setPosition(player.getPosition().x + size.x / 2 - bulletTexture.getSize().x / 2, player.getPosition().y - bulletTexture.getSize().y);
                bullets.push_back(bullet);
                canShoot = false;
            }
        } else {
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

        // --- Movimiento automático y animación de enemigos ---
        float peaDeltaTime = peaClock.restart().asSeconds();
        movementManager.update(peaDeltaTime);
        for (int i = 0; i < numEnemies; ++i) {
            peas[i].setPosition(peaPositions[i]);
            peas[i].update(peaDeltaTime);
        }
        for (int i = 0; i < numEnemies; ++i) {
            pebs[i].setPosition(pebPositions[i]);
            pebs[i].update(peaDeltaTime);
        }
        for (int i = 0; i < numEnemies; ++i) {
            pecs[i].setPosition(pecPositions[i]);
            pecs[i].update(peaDeltaTime);
        }

        // --- Colisión entre balas y enemigos ---
        for (int i = 0; i < numEnemies; ++i) {
            // Pea
            if (healthManager.getPeaHP()[i] > 0) {
                auto it = std::remove_if(bullets.begin(), bullets.end(), [&](const sf::Sprite& bullet) {
                    if (bullet.getGlobalBounds().intersects(peas[i].getGlobalBounds())) {
                        healthManager.damagePea(i, 20);
                        return true;
                    }
                    return false;
                });
                bullets.erase(it, bullets.end());
            }
            // Peb
            if (healthManager.getPebHP()[i] > 0) {
                auto it = std::remove_if(bullets.begin(), bullets.end(), [&](const sf::Sprite& bullet) {
                    if (bullet.getGlobalBounds().intersects(pebs[i].getGlobalBounds())) {
                        healthManager.damagePeb(i, 20);
                        return true;
                    }
                    return false;
                });
                bullets.erase(it, bullets.end());
            }
            // Pec
            if (healthManager.getPecHP()[i] > 0) {
                auto it = std::remove_if(bullets.begin(), bullets.end(), [&](const sf::Sprite& bullet) {
                    if (bullet.getGlobalBounds().intersects(pecs[i].getGlobalBounds())) {
                        healthManager.damagePec(i, 20);
                        return true;
                    }
                    return false;
                });
                bullets.erase(it, bullets.end());
            }
        }

        window.clear();
        // Dibuja el fondo espacial en la parte inferior
        window.draw(fondo);
        for (int i = 0; i < numBarreras; ++i)
            window.draw(barreras[i]);
        window.draw(player);
        for (const auto& bullet : bullets)
            window.draw(bullet);
        for (int i = 0; i < numEnemies; ++i)
            if (healthManager.getPeaHP()[i] > 0)
                peas[i].draw(window);
        for (int i = 0; i < numEnemies; ++i)
            if (healthManager.getPebHP()[i] > 0)
                pebs[i].draw(window);
        for (int i = 0; i < numEnemies; ++i)
            if (healthManager.getPecHP()[i] > 0)
                pecs[i].draw(window);
        window.display();
    }

    return 0;
}