#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "Tabletop");

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
        barreras[i].setPosition(espacio + i * (barreraWidth + espacio), 550);
    }

    // --- Player ---
    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("assets/images/space_Player.png"))
        return -1;
    sf::Sprite player(playerTexture);
    player.setPosition(400, 700);

    // --- Shootout (Bala) ---
    sf::Texture bulletTexture;
    if (!bulletTexture.loadFromFile("assets/images/Shootout.png"))
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
    std::vector<sf::Sprite> peas;
    std::vector<sf::Vector2f> peaPositions;
    std::vector<float> peaDirections(numEnemies, 1.0f);
    for (int i = 0; i < numEnemies; ++i) {
        sf::Sprite enemy(peaTexture);
        enemy.setPosition(60.0f + i * 90.0f, 0);
        enemy.setTextureRect(sf::IntRect(0, 0, peaFrameWidth, peaFrameHeight));
        peas.push_back(enemy);
        peaPositions.push_back(enemy.getPosition());
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
    float pebFrameTime = 1.0f;
    float pebAnimTime = 0.0f;
    std::vector<sf::Sprite> pebs;
    std::vector<sf::Vector2f> pebPositions;
    std::vector<float> pebDirections(numEnemies, 1.0f);
    for (int i = 0; i < numEnemies; ++i) {
        sf::Sprite enemy(pebTexture);
        enemy.setPosition(60.0f + i * 90.0f, 100);
        enemy.setTextureRect(sf::IntRect(0, 0, pebFrameWidth, pebFrameHeight));
        pebs.push_back(enemy);
        pebPositions.push_back(enemy.getPosition());
    }

    // --- Pec (Enemigos animados tipo Pec) ---
    sf::Texture pecTexture;
    if (!pecTexture.loadFromFile("assets/images/PEC.png"))
        return -1;
    int pecFrameWidth = 32;
    int pecFrameHeight = 32;
    int pecNumFrames = 2;
    int pecCurrentFrame = 0;
    float pecFrameTime = 1.0f;
    float pecAnimTime = 0.0f;
    std::vector<sf::Sprite> pecs;
    std::vector<sf::Vector2f> pecPositions;
    std::vector<float> pecDirections(numEnemies, 1.0f);
    for (int i = 0; i < numEnemies; ++i) {
        sf::Sprite enemy(pecTexture);
        enemy.setPosition(60.0f + i * 90.0f, 200);
        enemy.setTextureRect(sf::IntRect(0, 0, pecFrameWidth, pecFrameHeight));
        pecs.push_back(enemy);
        pecPositions.push_back(enemy.getPosition());
    }

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

        // --- Colisión entre balas y enemigos ---
        for (int i = 0; i < numEnemies; ++i) {
            // Pea
            if (peaHP[i] > 0) {
                auto it = std::remove_if(bullets.begin(), bullets.end(), [&](const sf::Sprite& bullet) {
                    if (bullet.getGlobalBounds().intersects(peas[i].getGlobalBounds())) {
                        peaHP[i] -= 20;
                        return true;
                    }
                    return false;
                });
                bullets.erase(it, bullets.end());
            }
            // Peb
            if (pebHP[i] > 0) {
                auto it = std::remove_if(bullets.begin(), bullets.end(), [&](const sf::Sprite& bullet) {
                    if (bullet.getGlobalBounds().intersects(pebs[i].getGlobalBounds())) {
                        pebHP[i] -= 20;
                        return true;
                    }
                    return false;
                });
                bullets.erase(it, bullets.end());
            }
            // Pec
            if (pecHP[i] > 0) {
                auto it = std::remove_if(bullets.begin(), bullets.end(), [&](const sf::Sprite& bullet) {
                    if (bullet.getGlobalBounds().intersects(pecs[i].getGlobalBounds())) {
                        pecHP[i] -= 20;
                        return true;
                    }
                    return false;
                });
                bullets.erase(it, bullets.end());
            }
        }

        // --- Movimiento automático y animación de Pea (múltiples enemigos) ---
        float peaDeltaTime = peaClock.restart().asSeconds();
        bool bounceLeft = peaPositions[0].x < 32;
        bool bounceRight = peaPositions[numEnemies - 1].x > 768 - peaFrameWidth;
        if (bounceLeft) {
            for (int i = 0; i < numEnemies; ++i) {
                peaPositions[i].x = 32 + (i * (peaPositions[1].x - peaPositions[0].x));
                peaPositions[i].y += 32;
                peaDirections[i] = 1.0f;
            }
        } else if (bounceRight) {
            for (int i = 0; i < numEnemies; ++i) {
                peaPositions[i].x = 768 - peaFrameWidth - (numEnemies - 1 - i) * (peaPositions[1].x - peaPositions[0].x);
                peaPositions[i].y += 32;
                peaDirections[i] = -1.0f;
            }
        } else {
            for (int i = 0; i < numEnemies; ++i) {
                peaPositions[i].x += peaVelocity * peaDirections[i] * peaDeltaTime;
            }
        }
        for (int i = 0; i < numEnemies; ++i) {
            if (peaPositions[i].y > 550) {
                peaVelocity = 200.0f;
            }
            peas[i].setPosition(peaPositions[i]);
        }
        peaAnimTime += peaDeltaTime;
        if (peaAnimTime >= peaFrameTime) {
            peaCurrentFrame = (peaCurrentFrame + 1) % peaNumFrames;
            int x = (peaCurrentFrame == 0) ? 0 : (peaFrameWidth + 8);
            for (int i = 0; i < numEnemies; ++i) {
                peas[i].setTextureRect(sf::IntRect(x, 0, peaFrameWidth, peaFrameHeight));
            }
            peaAnimTime = 0.0f;
        }
        // --- Movimiento automático y animación de Peb (múltiples enemigos) ---
        bool pebBounceLeft = pebPositions[0].x < 32;
        bool pebBounceRight = pebPositions[numEnemies - 1].x > 768 - pebFrameWidth;
        if (pebBounceLeft) {
            for (int i = 0; i < numEnemies; ++i) {
                pebPositions[i].x = 32 + (i * (pebPositions[1].x - pebPositions[0].x));
                pebPositions[i].y += 32;
                pebDirections[i] = 1.0f;
            }
        } else if (pebBounceRight) {
            for (int i = 0; i < numEnemies; ++i) {
                pebPositions[i].x = 768 - pebFrameWidth - (numEnemies - 1 - i) * (pebPositions[1].x - pebPositions[0].x);
                pebPositions[i].y += 32;
                pebDirections[i] = -1.0f;
            }
        } else {
            for (int i = 0; i < numEnemies; ++i) {
                pebPositions[i].x += peaVelocity * pebDirections[i] * peaDeltaTime;
            }
        }
        for (int i = 0; i < numEnemies; ++i) {
            if (pebPositions[i].y > 550) {
                peaVelocity = 200.0f;
            }
            pebs[i].setPosition(pebPositions[i]);
        }
        pebAnimTime += peaDeltaTime;
        if (pebAnimTime >= pebFrameTime) {
            pebCurrentFrame = (pebCurrentFrame + 1) % pebNumFrames;
            int x = (pebCurrentFrame == 0) ? 0 : (pebFrameWidth + 8);
            for (int i = 0; i < numEnemies; ++i) {
                pebs[i].setTextureRect(sf::IntRect(x, 0, pebFrameWidth, pebFrameHeight));
            }
            pebAnimTime = 0.0f;
        }
        // --- Movimiento automático y animación de Pec (múltiples enemigos) ---
        bool pecBounceLeft = pecPositions[0].x < 32;
        bool pecBounceRight = pecPositions[numEnemies - 1].x > 768 - pecFrameWidth;
        if (pecBounceLeft) {
            for (int i = 0; i < numEnemies; ++i) {
                pecPositions[i].x = 32 + (i * (pecPositions[1].x - pecPositions[0].x));
                pecPositions[i].y += 32;
                pecDirections[i] = 1.0f;
            }
        } else if (pecBounceRight) {
            for (int i = 0; i < numEnemies; ++i) {
                pecPositions[i].x = 768 - pecFrameWidth - (numEnemies - 1 - i) * (pecPositions[1].x - pecPositions[0].x);
                pecPositions[i].y += 32;
                pecDirections[i] = -1.0f;
            }
        } else {
            for (int i = 0; i < numEnemies; ++i) {
                pecPositions[i].x += peaVelocity * pecDirections[i] * peaDeltaTime;
            }
        }
        for (int i = 0; i < numEnemies; ++i) {
            if (pecPositions[i].y > 550) {
                peaVelocity = 200.0f;
            }
            pecs[i].setPosition(pecPositions[i]);
        }
        pecAnimTime += peaDeltaTime;
        if (pecAnimTime >= pecFrameTime) {
            pecCurrentFrame = (pecCurrentFrame + 1) % pecNumFrames;
            int x = (pecCurrentFrame == 0) ? 0 : (pecFrameWidth + 8);
            for (int i = 0; i < numEnemies; ++i) {
                pecs[i].setTextureRect(sf::IntRect(x, 0, pecFrameWidth, pecFrameHeight));
            }
            pecAnimTime = 0.0f;
        }

        window.clear();
        for (int i = 0; i < numBarreras; ++i)
            window.draw(barreras[i]);
        window.draw(player);
        for (const auto& bullet : bullets)
            window.draw(bullet);
        for (int i = 0; i < numEnemies; ++i)
            if (peaHP[i] > 0)
                window.draw(peas[i]);
        for (int i = 0; i < numEnemies; ++i)
            if (pebHP[i] > 0)
                window.draw(pebs[i]);
        for (int i = 0; i < numEnemies; ++i)
            if (pecHP[i] > 0)
                window.draw(pecs[i]);
        window.display();
    }

    return 0;
}