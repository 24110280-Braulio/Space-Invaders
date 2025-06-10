#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm> // Para std::remove_if

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
    if (!bulletTexture.loadFromFile("assets/images/Shootout.png"))
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
    bool peaAlive = true; // NUEVO: Pea está vivo

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

        // --- Colisión entre balas y Pea ---
        if (peaAlive) {
            auto it = std::remove_if(bullets.begin(), bullets.end(), [&](const sf::Sprite& bullet) {
                return bullet.getGlobalBounds().intersects(pea.getGlobalBounds());
            });
            if (it != bullets.end()) {
                peaAlive = false; // Pea muere si hay colisión
                bullets.erase(it, bullets.end());
            }
        }

        // --- Movimiento automático y animación de Pea ---
        if (peaAlive) {
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

        window.clear();
        window.draw(player);
        for (const auto& bullet : bullets)
            window.draw(bullet);
        if (peaAlive)
            window.draw(pea);
        window.display();
    }
    return 0;
}
