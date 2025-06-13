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
        barreras.emplace_back(barreraTexture, lowBarreraTexture, pos, 60); // Vida cambiada a 60
    }

    // --- Player ---
    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("assets/images/space_Player.png"))
        return -1;
    sf::Sprite player(playerTexture);
    player.setPosition(400, 700);
    PlayerHealth playerHP(100); // Usar PlayerHealth de Barrier.hpp

    // --- Invencibilidad del jugador ---
    sf::Clock playerInvincibleClock;
    bool playerInvincible = false;

    // --- Sprites de vida del jugador ---
    sf::Texture hp100Texture, hp80Texture, hp60Texture, hp40Texture, hp20Texture, hp0Texture;
    hp100Texture.loadFromFile("assets/images/100 HP.png");
    hp80Texture.loadFromFile("assets/images/80 HP.png");
    hp60Texture.loadFromFile("assets/images/60 HP.png");
    hp40Texture.loadFromFile("assets/images/40 HP.png");
    hp20Texture.loadFromFile("assets/images/20 HP.png");
    hp0Texture.loadFromFile("assets/images/0 HP.png");
    sf::Texture timeoutTexture;
    timeoutTexture.loadFromFile("assets/images/Timeout.png");
    sf::Sprite hpSprite(hp100Texture);
    hpSprite.setPosition(0, 0);
    hpSprite.setScale(68.0f / hp100Texture.getSize().x, 28.0f / hp100Texture.getSize().y);

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
    float peaVelocity = 50.0f; // Reducido a la mitad
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
    healthManager.setHealths(std::vector<int>(numEnemies, 20), std::vector<int>(numEnemies, 20), std::vector<int>(numEnemies, 20));

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
    int totalEnemiesDefeated = 0;
    int lastShootChanceUpdate = 0;
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
        static bool canShoot = true;
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
            // Condición: Si cualquier enemigo llega a Y >= 800, daña al jugador y elimina al enemigo
            if (peaPositions[i].y >= 800 && healthManager.getPeaHP()[i] > 0) {
                if (!playerInvincible && playerHP.isAlive()) {
                    playerHP.takeDamage(100);
                    playerInvincible = true;
                    playerInvincibleClock.restart();
                }
                healthManager.damagePea(i, healthManager.getPeaHP()[i]); // Eliminar enemigo
            }
            if (pebPositions[i].y >= 800 && healthManager.getPebHP()[i] > 0) {
                if (!playerInvincible && playerHP.isAlive()) {
                    playerHP.takeDamage(100);
                    playerInvincible = true;
                    playerInvincibleClock.restart();
                }
                healthManager.damagePeb(i, healthManager.getPebHP()[i]);
            }
            if (pecPositions[i].y >= 800 && healthManager.getPecHP()[i] > 0) {
                if (!playerInvincible && playerHP.isAlive()) {
                    playerHP.takeDamage(100);
                    playerInvincible = true;
                    playerInvincibleClock.restart();
                }
                healthManager.damagePec(i, healthManager.getPecHP()[i]);
            }
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

        // --- Restricciones de altura entre enemigos ---
        auto ajustarAltura = [](std::vector<sf::Vector2f>& arriba, std::vector<int>& arribaHP,
                                const std::vector<sf::Vector2f>& abajo, const std::vector<int>& abajoHP,
                                float distanciaX, float distanciaY) {
            int n = arriba.size();
            int m = abajo.size();
            for (int i = 0; i < n; ++i) {
                if (arribaHP[i] <= 0) continue;
                for (int j = 0; j < m; ++j) {
                    if (abajoHP[j] > 0 && std::abs(arriba[i].x - abajo[j].x) < distanciaX) {
                        if (arriba[i].y > abajo[j].y - distanciaY) {
                            arriba[i].y = abajo[j].y - distanciaY;
                        }
                    }
                }
            }
        };

        ajustarAltura(peaPositions, healthManager.getPeaHP(), pebPositions, healthManager.getPebHP(), 30, 30); // PEA sobre PEB
        ajustarAltura(pecPositions, healthManager.getPecHP(), peaPositions, healthManager.getPeaHP(), 30, 30); // PEC sobre PEA

        // --- Restricción: Si hay al menos un PEB, todos los PEA deben estar al menos 40px arriba del PEB más cercano en X ---
        bool hayPeb = std::any_of(healthManager.getPebHP().begin(), healthManager.getPebHP().end(), [](int hp){ return hp > 0; });
        if (hayPeb) {
            for (int i = 0; i < numEnemies; ++i) {
                if (healthManager.getPeaHP()[i] <= 0) continue;
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
        bool hayPea = std::any_of(healthManager.getPeaHP().begin(), healthManager.getPeaHP().end(), [](int hp){ return hp > 0; });
        if (hayPea) {
            for (int i = 0; i < numEnemies; ++i) {
                if (healthManager.getPecHP()[i] <= 0) continue;
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
        int enemiesDefeatedThisFrame = 0;
        for (int i = 0; i < numEnemies; ++i) {
            // Pea
            if (healthManager.getPeaHP()[i] > 0) {
                auto it = std::remove_if(bullets.begin(), bullets.end(), [&](const sf::Sprite& bullet) {
                    if (bullet.getGlobalBounds().intersects(peas[i].getGlobalBounds())) {
                        healthManager.damagePea(i, 20);
                        if (healthManager.getPeaHP()[i] <= 0) enemiesDefeatedThisFrame++;
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
                        if (healthManager.getPebHP()[i] <= 0) enemiesDefeatedThisFrame++;
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
                        if (healthManager.getPecHP()[i] <= 0) enemiesDefeatedThisFrame++;
                        return true;
                    }
                    return false;
                });
                bullets.erase(it, bullets.end());
            }
        }
        totalEnemiesDefeated += enemiesDefeatedThisFrame;
        // Cada 2 enemigos derrotados, aumenta shootChance en 1
        if ((totalEnemiesDefeated / 2) > lastShootChanceUpdate) {
            shootChance++;
            lastShootChanceUpdate = totalEnemiesDefeated / 2;
        }
        // --- Disparo enemigo automático (Pea, Peb y Pec disparan) ---
        for (int i = 0; i < numEnemies; ++i) {
            if (healthManager.getPeaHP()[i] > 0) {
                if (enemyShootClocks[i].getElapsedTime().asSeconds() > shootInterval) {
                    enemyShooter.tryShoot(peaPositions[i], peaFrameWidth, peaFrameHeight, shootChance);
                    enemyShootClocks[i].restart();
                }
            }
            if (healthManager.getPebHP()[i] > 0) {
                if (enemyShootClocks[i].getElapsedTime().asSeconds() > shootInterval) {
                    enemyShooter.tryShoot(pebPositions[i], pebFrameWidth, pebFrameHeight, shootChance);
                    enemyShootClocks[i].restart();
                }
            }
            if (healthManager.getPecHP()[i] > 0) {
                if (enemyShootClocks[i].getElapsedTime().asSeconds() > shootInterval) {
                    enemyShooter.tryShoot(pecPositions[i], pecFrameWidth, pecFrameHeight, shootChance);
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
                if (!playerInvincible && playerHP.isAlive()) {
                    playerHP.takeDamage(20); // Usar método de PlayerHealth
                    playerInvincible = true;
                    playerInvincibleClock.restart();
                }
                // Aquí podrías agregar lógica de game over si !playerHP.isAlive()
                return true;
            }
            return false;
        }), peaBullets.end());

        // --- Daño por enemigos que llegan al fondo ---
        for (int i = 0; i < numEnemies; ++i) {
            // Pea
            if (peaPositions[i].y >= 800 && healthManager.getPeaHP()[i] > 0) {
                if (!playerInvincible && playerHP.isAlive()) {
                    playerHP.takeDamage(100);
                    playerInvincible = true;
                    playerInvincibleClock.restart();
                }
                healthManager.damagePea(i, healthManager.getPeaHP()[i]); // Eliminar enemigo
            }
            // Peb
            if (pebPositions[i].y >= 800 && healthManager.getPebHP()[i] > 0) {
                if (!playerInvincible && playerHP.isAlive()) {
                    playerHP.takeDamage(100);
                    playerInvincible = true;
                    playerInvincibleClock.restart();
                }
                healthManager.damagePeb(i, healthManager.getPebHP()[i]);
            }
            // Pec
            if (pecPositions[i].y >= 800 && healthManager.getPecHP()[i] > 0) {
                if (!playerInvincible && playerHP.isAlive()) {
                    playerHP.takeDamage(100);
                    playerInvincible = true;
                    playerInvincibleClock.restart();
                }
                healthManager.damagePec(i, healthManager.getPecHP()[i]);
            }
        }

        // --- Actualizar estado de invencibilidad ---
        if (playerInvincible && playerInvincibleClock.getElapsedTime().asSeconds() >= 2.0f) {
            playerInvincible = false;
        }

        // --- Actualizar sprite de vida según el HP exacto o invencibilidad ---
        if (playerInvincible) {
            hpSprite.setTexture(timeoutTexture, true);
            // Mantener el mismo tamaño y posición
            hpSprite.setScale(68.0f / timeoutTexture.getSize().x, 28.0f / timeoutTexture.getSize().y);
        } else {
            switch (playerHP.getHP()) {
                case 100:
                    hpSprite.setTexture(hp100Texture, true);
                    hpSprite.setScale(68.0f / hp100Texture.getSize().x, 28.0f / hp100Texture.getSize().y);
                    break;
                case 80:
                    hpSprite.setTexture(hp80Texture, true);
                    hpSprite.setScale(68.0f / hp80Texture.getSize().x, 28.0f / hp80Texture.getSize().y);
                    break;
                case 60:
                    hpSprite.setTexture(hp60Texture, true);
                    hpSprite.setScale(68.0f / hp60Texture.getSize().x, 28.0f / hp60Texture.getSize().y);
                    break;
                case 40:
                    hpSprite.setTexture(hp40Texture, true);
                    hpSprite.setScale(68.0f / hp40Texture.getSize().x, 28.0f / hp40Texture.getSize().y);
                    break;
                case 20:
                    hpSprite.setTexture(hp20Texture, true);
                    hpSprite.setScale(68.0f / hp20Texture.getSize().x, 28.0f / hp20Texture.getSize().y);
                    break;
                case 0:
                    hpSprite.setTexture(hp0Texture, true);
                    hpSprite.setScale(68.0f / hp0Texture.getSize().x, 28.0f / hp0Texture.getSize().y);
                    break;
                default:
                    break;
            }
        }

        window.clear();
        // Dibuja el fondo espacial en la parte inferior
        window.draw(fondo);
        window.draw(hpSprite); // Dibuja el sprite de vida del jugador
        for (auto& barrera : barreras)
            if (barrera.isAlive())
                window.draw(barrera.getSprite());
        if (playerHP.isAlive())
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