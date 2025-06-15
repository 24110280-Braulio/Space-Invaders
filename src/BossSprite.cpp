#include <SFML/Graphics.hpp>
#include <array>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "Boss Sprite");
    std::array<sf::Texture, 2> bossTextures;
    if (!bossTextures[0].loadFromFile("assets/images/Boss1_A.png"))
        return -1;
    if (!bossTextures[1].loadFromFile("assets/images/Boss1_B.png"))
        return -1;
    sf::Sprite bossSprite(bossTextures[0]);
    bossSprite.setPosition(400 - bossTextures[0].getSize().x / 2, 200);

    int currentFrame = 0;
    float frameTime = 0.5f;
    float animTime = 0.0f;
    sf::Clock clock;

    sf::Texture laser1Texture, laser2Texture;
    if (!laser1Texture.loadFromFile("assets/images/Laser1.png"))
        return -1;
    if (!laser2Texture.loadFromFile("assets/images/Laser2.png"))
        return -1;
    sf::Sprite laser1Sprite(laser1Texture);
    sf::Sprite laser2Sprite(laser2Texture);
    bool showLasers = false;
    sf::Clock laserClock;
    float laserInterval = 3.0f;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        animTime += clock.restart().asSeconds();
        if (animTime >= frameTime) {
            currentFrame = (currentFrame + 1) % 2;
            bossSprite.setTexture(bossTextures[currentFrame]);
            animTime = 0.0f;
        }
        // Mostrar lasers cada 3 segundos
        if (laserClock.getElapsedTime().asSeconds() >= laserInterval) {
            showLasers = true;
            laserClock.restart();
        }
        window.clear();
        window.draw(bossSprite);
        if (showLasers) {
            // Posicionar Laser1 justo debajo del boss
            sf::FloatRect bossBounds = bossSprite.getGlobalBounds();
            float laser1X = bossBounds.left + bossBounds.width / 2 - laser1Texture.getSize().x / 2;
            float laser1Y = bossBounds.top + bossBounds.height;
            laser1Sprite.setPosition(laser1X, laser1Y);
            window.draw(laser1Sprite);
            // Laser2 debajo de Laser1, repetir hasta Y >= 800
            float laser2X = laser1X;
            float laser2Y = laser1Y + laser1Texture.getSize().y;
            while (laser2Y < 800) {
                laser2Sprite.setPosition(laser2X, laser2Y);
                window.draw(laser2Sprite);
                laser2Y += laser2Texture.getSize().y;
            }
        }
        window.display();
    }
    return 0;
}
