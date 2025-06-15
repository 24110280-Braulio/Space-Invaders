#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "Ghosts Sprite Demo");

    // Cargar texturas de cada fantasma
    sf::Texture blinkyLTexture, blinkyRTexture, inkyLTexture, inkyRTexture, pinkyLTexture, pinkyRTexture, claydLTexture, claydRTexture;
    if (!blinkyLTexture.loadFromFile("assets/images/Boss2nd/BlinkyL.png") ||
        !blinkyRTexture.loadFromFile("assets/images/Boss2nd/BlinkyR.png") ||
        !inkyLTexture.loadFromFile("assets/images/Boss2nd/InkyL.png") ||
        !inkyRTexture.loadFromFile("assets/images/Boss2nd/InkyR.png") ||
        !pinkyLTexture.loadFromFile("assets/images/Boss2nd/PinkyL.png") ||
        !pinkyRTexture.loadFromFile("assets/images/Boss2nd/PinkyR.png") ||
        !claydLTexture.loadFromFile("assets/images/Boss2nd/ClaydL.png") ||
        !claydRTexture.loadFromFile("assets/images/Boss2nd/ClaydR.png")) {
        std::cerr << "No se pudo cargar alguna textura de los fantasmas" << std::endl;
        return -1;
    }

    // Crear sprites y posiciones iniciales
    sf::Sprite blinkySprite(blinkyLTexture), inkySprite(inkyLTexture), pinkySprite(pinkyLTexture), claydSprite(claydLTexture);
    blinkySprite.setPosition(600, 200);
    inkySprite.setPosition(600, 300);
    pinkySprite.setPosition(600, 400);
    claydSprite.setPosition(600, 500);

    float velocitys[4] = {-200.0f, -200.0f, -200.0f, -200.0f};
    bool movingLefts[4] = {true, true, true, true};
    sf::Texture* leftTextures[4] = {&blinkyLTexture, &inkyLTexture, &pinkyLTexture, &claydLTexture};
    sf::Texture* rightTextures[4] = {&blinkyRTexture, &inkyRTexture, &pinkyRTexture, &claydRTexture};
    sf::Sprite* sprites[4] = {&blinkySprite, &inkySprite, &pinkySprite, &claydSprite};

    sf::Clock clock;
    float elapsed = 0.0f;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float deltaTime = clock.restart().asSeconds();
        elapsed += deltaTime;

        // Mover y alternar cada fantasma
        for (int i = 0; i < 4; ++i) {
            sprites[i]->move(velocitys[i] * deltaTime, 0);
        }
        if (elapsed >= 1.5f) {
            for (int i = 0; i < 4; ++i) {
                movingLefts[i] = !movingLefts[i];
                if (movingLefts[i]) {
                    sprites[i]->setTexture(*leftTextures[i]);
                    velocitys[i] = -200.0f;
                } else {
                    sprites[i]->setTexture(*rightTextures[i]);
                    velocitys[i] = 200.0f;
                }
            }
            elapsed = 0.0f;
        }

        window.clear(sf::Color::Black);
        for (int i = 0; i < 4; ++i) {
            window.draw(*sprites[i]);
        }
        window.display();
    }
    return 0;
}
