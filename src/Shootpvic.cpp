#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm> // Para std::remove_if
#include <random> // Para el generador aleatorio
#include <array>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "Shootp - Disparo Jugador");

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

    // --- Pea (Sprite animado) ---
    sf::Texture peaTexture;
    if (!peaTexture.loadFromFile("assets/images/PEA.png"))
        return -1;
    sf::Sprite pea(peaTexture);
    pea.setPosition(0, 0); // Posición inicial del sprite
    int peaFrameWidth = 44;
    int peaFrameHeight = 32;
    int peaNumFrames = 2;
    int peaCurrentFrame = 0;
    float peaFrameTime = 0.1f;
    float peaAnimTime = 0.0f;
    float peaVelocity = 500.0f;
    float peaDirection = 1.0f;
    sf::Clock peaClock;
    // bool peaAlive = true; // NUEVO: Pea está vivo
    int peaHP = 200; // NUEVO: Puntos de vida de Pea

    // --- Eshoot (Disparo enemigo animado) ---
    std::array<sf::Texture, 4> enemyBulletTextures;
    for (int i = 0; i < 4; ++i) {
        std::string filename = "assets/images/Eshoot(" + std::to_string(i + 1) + ").png";
        if (!enemyBulletTextures[i].loadFromFile(filename)) {
            return -1;
        }
    }
    struct EnemyBullet {
        sf::Sprite sprite;
        int currentFrame;
        float animTime;
    };
    std::vector<EnemyBullet> enemyBullets;
    float enemyBulletSpeed = 1.5f;
    float enemyBulletFrameTime = 0.1f;
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(1, 20);
    bool canEnemyShoot = true;
    sf::Clock enemyShootClock;
    int playerHP = 60; // El jugador tiene 60 puntos de vida

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
    sf::Sprite barreras[numBarreras];
    int barreraHP[numBarreras] = {60, 60, 60, 60};
    for (int i = 0; i < numBarreras; ++i) {
        barreras[i].setTexture(barreraTexture);
        barreras[i].setPosition(espacio + i * (barreraWidth + espacio), 490);
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Movimiento del jugador
        float velocidad = 0.5f;
        sf::Vector2f pos = player.getPosition();
        sf::Vector2u size = playerTexture.getSize();
        float minX = 0;
        float minY = 550;
        float maxX = 800 - size.x;
        float maxY = 800 - size.y;
        if (playerHP > 0) { // Solo puede moverse si tiene vidas
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

        // Disparo (tecla Z)
        static bool canShoot = true;
        if (playerHP > 0) { // Solo puede disparar si tiene vidas
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
        }

        // Mover balas
        for (auto& bullet : bullets) {
            bullet.move(0, -bulletSpeed);
        }
        // Eliminar balas fuera de pantalla
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const sf::Sprite& b) {
            return b.getPosition().y + b.getTexture()->getSize().y < 0;
        }), bullets.end());

        // --- Colisión entre balas y Pea ---
        if (peaHP > 0) {
            auto it = std::remove_if(bullets.begin(), bullets.end(), [&](const sf::Sprite& bullet) {
                if (bullet.getGlobalBounds().intersects(pea.getGlobalBounds())) {
                    peaHP -= 20; // Cada disparo causa 20 puntos de daño
                    return true; // Eliminar la bala
                }
                return false;
            });
            bullets.erase(it, bullets.end());
        }

        // --- Movimiento automático y animación de Pea ---
        if (peaHP > 0) {
            float peaDeltaTime = peaClock.restart().asSeconds();
            sf::Vector2f peaPos = pea.getPosition();
            peaPos.x += peaVelocity * peaDirection * peaDeltaTime;
            if (peaPos.x < 0) {
                peaPos.x = 0;
                peaPos.y += 32;
                peaDirection = 1.0f;
            } else if (peaPos.x > 800 - peaFrameWidth) {
                peaPos.x = 800 - peaFrameWidth;
                peaPos.y += 32;
                peaDirection = -1.0f;
            }
            if (peaPos.y > 550) {
                peaVelocity = 700.0f;
            }
            pea.setPosition(peaPos);
            peaAnimTime += peaDeltaTime;
            if (peaAnimTime >= peaFrameTime) {
                peaCurrentFrame = (peaCurrentFrame + 1) % peaNumFrames;
                int x = (peaCurrentFrame == 0) ? 0 : (peaFrameWidth + 8);
                pea.setTextureRect(sf::IntRect(x, 0, peaFrameWidth, peaFrameHeight));
                peaAnimTime = 0.0f;
            }
        }

        // --- Disparo enemigo (Pea) ---
        if (peaHP > 0) {
            if (canEnemyShoot) {
                int randomValue = dist(rng);
                if (randomValue < 8) {
                    EnemyBullet eb;
                    eb.sprite.setTexture(enemyBulletTextures[0]);
                    eb.sprite.setPosition(pea.getPosition().x + peaFrameWidth / 2 - enemyBulletTextures[0].getSize().x / 2, pea.getPosition().y + peaFrameHeight);
                    eb.currentFrame = 0;
                    eb.animTime = 0.0f;
                    enemyBullets.push_back(eb);
                    canEnemyShoot = false;
                    enemyShootClock.restart();
                }
            } else {
                if (enemyShootClock.getElapsedTime().asSeconds() > 0.5f) {
                    canEnemyShoot = true;
                }
            }
        }
        // Mover y animar balas enemigas
        for (auto& eb : enemyBullets) {
            eb.sprite.move(0, enemyBulletSpeed);
            eb.animTime += enemyBulletFrameTime;
            if (eb.animTime >= enemyBulletFrameTime) {
                eb.currentFrame = (eb.currentFrame + 1) % 4;
                eb.sprite.setTexture(enemyBulletTextures[eb.currentFrame]);
                eb.animTime = 0.0f;
            }
        }
        // --- Colisión entre balas enemigas y barreras ---
        for (auto& eb : enemyBullets) {
            for (int i = 0; i < numBarreras; ++i) {
                if (barreraHP[i] > 0 && eb.sprite.getGlobalBounds().intersects(barreras[i].getGlobalBounds())) {
                    barreraHP[i] -= 20;
                    if (barreraHP[i] <= 20 && barreraHP[i] > 0) {
                        barreras[i].setTexture(lowBarreraTexture);
                    }
                }
            }
        }
        // Eliminar balas enemigas que colisionan con barreras
        enemyBullets.erase(std::remove_if(enemyBullets.begin(), enemyBullets.end(), [&](const EnemyBullet& eb) {
            for (int i = 0; i < numBarreras; ++i) {
                if (barreraHP[i] > 0 && eb.sprite.getGlobalBounds().intersects(barreras[i].getGlobalBounds())) {
                    return true;
                }
            }
            if (eb.sprite.getPosition().y > 800) return true;
            if (playerHP > 0 && eb.sprite.getGlobalBounds().intersects(player.getGlobalBounds())) {
                playerHP -= 20;
                return true;
            }
            return false;
        }), enemyBullets.end());

        // Si el jugador muere, no cerrar la ventana, solo dejar de dibujarlo y de permitir acciones
        if (playerHP <= 0) {
            // window.close();
        }

        window.clear();
        // Dibujar barreras si tienen vida
        for (int i = 0; i < numBarreras; ++i) {
            if (barreraHP[i] > 0)
                window.draw(barreras[i]);
        }
        if (playerHP > 0)
            window.draw(player);
        for (const auto& bullet : bullets)
            window.draw(bullet);
        // Dibujar balas enemigas
        for (const auto& eb : enemyBullets)
            window.draw(eb.sprite);
        if (peaHP > 0)
            window.draw(pea);
        window.display();
    }
    return 0;
}
