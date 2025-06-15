#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <array>
#include "../include/Barrier.hpp"
#include "../include/PlayerHealth.hpp"

int main() {
    // Tamaño de ventana igual que Tabletop
    sf::RenderWindow window(sf::VideoMode(800, 800), "Boss Mode");

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
        barreras.emplace_back(barreraTexture, lowBarreraTexture, pos, 100); // Vida 100
    }

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

    sf::Clock clock;
    bool canShoot = true;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
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

        // --- Renderizado ---
        window.clear();
        window.draw(playerHP.getSprite());
        for (auto& barrera : barreras)
            if (barrera.isAlive())
                window.draw(barrera.getSprite());
        if (playerHP.isAlive())
            window.draw(player);
        for (const auto& bullet : bullets)
            window.draw(bullet);
        window.display();
    }
    return 0;
}
