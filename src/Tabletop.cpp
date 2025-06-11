#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include "../include/EnemyMovementManager.hpp"
#include "../include/EnemyHealthManager.hpp"
#include "../include/Enemy.hpp"
#include "../include/Barrier.hpp"
#include "../include/EnemyShooter.hpp"

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
    sf::Texture lowBarreraTexture;
    if (!lowBarreraTexture.loadFromFile("assets/images/Lowbarrier.png"))
        return -1;
    const int barreraWidth = 64;
    const int numBarreras = 4;
    const int espacio = (800 - (numBarreras * barreraWidth)) / (numBarreras + 1);
    std::vector<Barrier> barreras;
    for (int i = 0; i < numBarreras; ++i) {
        sf::Vector2f pos(espacio + i * (barreraWidth + espacio), 490);
        barreras.emplace_back(barreraTexture, lowBarreraTexture, pos, 5000); // Vida aumentada a 120
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
    std::vector<int> peaHP(numEnemies, 20); // Salud individual para Pea
    std::vector<int> pebHP(numEnemies, 20); // Salud individual para Peb
    std::vector<int> pecHP(numEnemies, 20); // Salud individual para Pec

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

    // --- Eshoot (Disparo enemigo animado) ---
    std::array<sf::Texture, 4> enemyBulletTextures;
    for (int i = 0; i < 4; ++i) {
        std::string filename = "assets/images/Eshoot(" + std::to_string(i + 1) + ").png";
        if (!enemyBulletTextures[i].loadFromFile(filename)) {
            return -1;
        }
    }
    EnemyShooter enemyShooter(enemyBulletTextures, 0.5f, 0.1f);
    float shootInterval = 0.5f;
    int shootChance = 8;
    sf::Clock shootClock;
    std::vector<sf::Clock> enemyShootClocks(numEnemies); // Delay individual de disparo

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

        // Disparo (tecla Space)
        static bool canShoot = true;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
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
        // Restricción: si hay al menos una barrera activa, los enemigos no pueden bajar de y=500
        bool barreraActiva = false;
        for (const auto& barrera : barreras) {
            if (barrera.isAlive()) {
                barreraActiva = true;
                break;
            }
        }
        float limiteY = barreraActiva ? 450.0f : 800.0f;
        for (int i = 0; i < numEnemies; ++i) {
            if (peaPositions[i].y > limiteY) peaPositions[i].y = limiteY;
            peas[i].setPosition(peaPositions[i]);
            peas[i].update(peaDeltaTime);
        }
        for (int i = 0; i < numEnemies; ++i) {
            if (pebPositions[i].y > limiteY) pebPositions[i].y = limiteY;
            pebs[i].setPosition(pebPositions[i]);
            pebs[i].update(peaDeltaTime);
        }
        for (int i = 0; i < numEnemies; ++i) {
            if (pecPositions[i].y > limiteY) pecPositions[i].y = limiteY;
            pecs[i].setPosition(pecPositions[i]);
            pecs[i].update(peaDeltaTime);
        }

        // --- Restricción: PEA debe estar al menos 16px arriba de PEB, y PEC al menos 16px arriba de PEA ---
        for (int i = 0; i < numEnemies; ++i) {
            // PEA vs PEB
            if (std::abs(peaPositions[i].x - pebPositions[i].x) < 30) {
                if (peaPositions[i].y > pebPositions[i].y - 16) {
                    peaPositions[i].y = pebPositions[i].y - 16;
                }
            }
        }
        for (int i = 0; i < numEnemies; ++i) {
            // PEC vs PEA
            if (std::abs(pecPositions[i].x - peaPositions[i].x) < 30) {
                if (pecPositions[i].y > peaPositions[i].y - 16) {
                    pecPositions[i].y = peaPositions[i].y - 16;
                }
            }
        }

        // --- Restricción: Si hay al menos un PEB, todos los PEA deben estar al menos 16px arriba del PEB más cercano en X ---
        bool hayPeb = false;
        for (int i = 0; i < numEnemies; ++i) {
            if (healthManager.getPebHP()[i] > 0) {
                hayPeb = true;
                break;
            }
        }
        if (hayPeb) {
            for (int i = 0; i < numEnemies; ++i) {
                float minDeltaX = 1e6;
                int idxPebCercano = -1;
                for (int j = 0; j < numEnemies; ++j) {
                    if (healthManager.getPebHP()[j] > 0) {
                        float deltaX = std::abs(peaPositions[i].x - pebPositions[j].x);
                        if (deltaX < minDeltaX) {
                            minDeltaX = deltaX;
                            idxPebCercano = j;
                        }
                    }
                }
                if (idxPebCercano != -1 && minDeltaX < 30) {
                    if (peaPositions[i].y > pebPositions[idxPebCercano].y - 40) {
                        peaPositions[i].y = pebPositions[idxPebCercano].y - 40;
                    }
                }
            }
        }
        // --- Lo mismo para PEC respecto a PEA ---
        bool hayPea = false;
        for (int i = 0; i < numEnemies; ++i) {
            if (healthManager.getPeaHP()[i] > 0) {
                hayPea = true;
                break;
            }
        }
        if (hayPea) {
            for (int i = 0; i < numEnemies; ++i) {
                float minDeltaX = 1e6;
                int idxPeaCercano = -1;
                for (int j = 0; j < numEnemies; ++j) {
                    if (healthManager.getPeaHP()[j] > 0) {
                        float deltaX = std::abs(pecPositions[i].x - peaPositions[j].x);
                        if (deltaX < minDeltaX) {
                            minDeltaX = deltaX;
                            idxPeaCercano = j;
                        }
                    }
                }
                if (idxPeaCercano != -1 && minDeltaX < 30) {
                    if (pecPositions[i].y > peaPositions[idxPeaCercano].y - 16) {
                        pecPositions[i].y = peaPositions[idxPeaCercano].y - 16;
                    }
                }
            }
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

        // --- Disparo enemigo automático (ejemplo: Pea dispara) ---
        for (int i = 0; i < numEnemies; ++i) {
            if (healthManager.getPeaHP()[i] > 0) {
                if (enemyShootClocks[i].getElapsedTime().asSeconds() > shootInterval) {
                    enemyShooter.tryShoot(peaPositions[i], peaFrameWidth, peaFrameHeight, shootChance);
                    enemyShootClocks[i].restart();
                }
            }
        }
        enemyShooter.updateBullets();
        // --- Colisión entre balas enemigas y barreras ---
        auto& peaBullets = enemyShooter.getBullets();
        peaBullets.erase(std::remove_if(peaBullets.begin(), peaBullets.end(), [&](const EnemyBullet& eb) {
            for (auto& barrera : barreras) {
                if (barrera.isAlive() && eb.sprite.getGlobalBounds().intersects(barrera.getSprite().getGlobalBounds())) {
                    barrera.takeDamage(20);
                    return true;
                }
            }
            if (eb.sprite.getPosition().y > 800) return true;
            if (eb.sprite.getGlobalBounds().intersects(player.getGlobalBounds())) {
                // Aquí podrías implementar daño al jugador si lo deseas
                return true;
            }
            return false;
        }), peaBullets.end());

        window.clear();
        // Dibuja el fondo espacial en la parte inferior
        window.draw(fondo);
        for (auto& barrera : barreras)
            if (barrera.isAlive())
                window.draw(barrera.getSprite());
        window.draw(player);
        for (const auto& bullet : bullets)
            window.draw(bullet);
        enemyShooter.drawBullets(window);
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