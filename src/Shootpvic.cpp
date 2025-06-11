#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm> // Para std::remove_if
#include <random> // Para el generador aleatorio
#include <array>
#include "../include/EnemyShooter.hpp"
#include "../include/Barrier.hpp"

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
    int peaHP = 200; // NUEVO: Puntos de vida de Pea

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
        barreras.emplace_back(barreraTexture, lowBarreraTexture, pos);
    }

    // --- Eshoot (Disparo enemigo animado) ---
    std::array<sf::Texture, 4> enemyBulletTextures;
    for (int i = 0; i < 4; ++i) {
        std::string filename = "assets/images/Eshoot(" + std::to_string(i + 1) + ").png";
        if (!enemyBulletTextures[i].loadFromFile(filename)) {
            return -1;
        }
    }
    EnemyShooter peaShooter(enemyBulletTextures, 0.5f, 0.1f); // Línea 68: inicializa el disparador enemigo, pasando velocidad de disparo y frameTime de animación
    float enemyBulletSpeed = 0.00001f;           // Línea 69: controla la velocidad de movimiento vertical de las balas enemigas en pantalla
    float enemyBulletFrameTime = 0.1f;           // Línea 70: controla cada cuánto se actualiza el frame de animación de la bala enemiga
    int peaShootChance = 8; // Puedes ajustar este valor para cambiar la probabilidad
    sf::Clock peaShootClock;
    float shootInterval = 0.5f;
    PlayerHealth playerHP(60); // El jugador tiene 60 puntos de vida

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
        if (playerHP.isAlive()) { // Solo puede moverse si tiene vidas
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
        if (playerHP.isAlive()) { // Solo puede disparar si tiene vidas
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
        if (peaHP > 0 && peaShootClock.getElapsedTime().asSeconds() > shootInterval) {
            peaShooter.tryShoot(pea.getPosition(), peaFrameWidth, peaFrameHeight, peaShootChance);
            peaShootClock.restart();
        }
        peaShooter.updateBullets();

        // Mover y animar balas enemigas
        for (auto& eb : peaShooter.getBullets()) {
            eb.sprite.move(0, enemyBulletSpeed);
            eb.animTime += enemyBulletFrameTime;
            if (eb.animTime >= enemyBulletFrameTime) {
                eb.currentFrame = (eb.currentFrame + 1) % 4;
                eb.sprite.setTexture(enemyBulletTextures[eb.currentFrame]);
                eb.animTime = 0.0f;
            }
        }
        // --- Colisión entre balas enemigas y barreras ---
        for (auto& eb : peaShooter.getBullets()) {
            for (auto& barrera : barreras) {
                if (barrera.isAlive() && eb.sprite.getGlobalBounds().intersects(barrera.getSprite().getGlobalBounds())) {
                    barrera.takeDamage(20);
                }
            }
        }
        // Eliminar balas enemigas que colisionan con barreras o jugador
        auto& peaBullets = peaShooter.getBullets();
        peaBullets.erase(std::remove_if(peaBullets.begin(), peaBullets.end(), [&](const EnemyBullet& eb) {
            for (auto& barrera : barreras) {
                if (barrera.isAlive() && eb.sprite.getGlobalBounds().intersects(barrera.getSprite().getGlobalBounds())) {
                    return true;
                }
            }
            if (eb.sprite.getPosition().y > 800) return true;
            if (playerHP.isAlive() && eb.sprite.getGlobalBounds().intersects(player.getGlobalBounds())) {
                playerHP.takeDamage(20);
                return true;
            }
            return false;
        }), peaBullets.end());

        // Si el jugador muere, no cerrar la ventana, solo dejar de dibujarlo y de permitir acciones
        if (!playerHP.isAlive()) {
            // window.close();
        }

        window.clear();
        // Dibujar barreras si tienen vida
        for (auto& barrera : barreras) {
            if (barrera.isAlive())
                window.draw(barrera.getSprite());
        }
        if (playerHP.isAlive())
            window.draw(player);
        for (const auto& bullet : bullets)
            window.draw(bullet);
        // Dibujar balas enemigas
        peaShooter.drawBullets(window);
        if (peaHP > 0)
            window.draw(pea);
        window.display();
    }
    return 0;
}
