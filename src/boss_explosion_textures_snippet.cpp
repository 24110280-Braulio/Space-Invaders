#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

int main() {
    sf::RenderWindow window(sf::VideoMode(256, 256), "Animación Explosión Boss");
    window.setFramerateLimit(60);

    // Cargar texturas de explosión
    std::vector<sf::Texture> explosionTextures(10);
    for (int i = 0; i < 10; ++i) {
        std::string filename = "assets/images/Explosion" + std::to_string(i + 1) + ".png";
        if (!explosionTextures[i].loadFromFile(filename)) {
            return -1;
        }
    }

    sf::Sprite explosionSprite;
    int currentFrame = 0;
    float frameTime = 0.08f; // segundos por frame
    float elapsed = 0.0f;
    sf::Clock clock;

    // Centrar sprite
    explosionSprite.setOrigin(64, 64);
    explosionSprite.setPosition(128, 128);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        elapsed += clock.restart().asSeconds();
        if (elapsed >= frameTime) {
            currentFrame = (currentFrame + 1) % 10;
            elapsed = 0.0f;
        }
        explosionSprite.setTexture(explosionTextures[currentFrame]);

        window.clear(sf::Color::Black);
        window.draw(explosionSprite);
        window.display();
    }
    return 0;
}
