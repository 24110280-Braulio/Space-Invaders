#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "Pinky Sprite Demo");

    // Cargar texturas de Pinky
    sf::Texture pinkyLTexture, pinkyRTexture;
    if (!pinkyLTexture.loadFromFile("assets/images/Boss2nd/PinkyL.png") ||
        !pinkyRTexture.loadFromFile("assets/images/Boss2nd/PinkyR.png")) {
        std::cerr << "No se pudo cargar alguna textura de Pinky" << std::endl;
        return -1;
    }

    // Crear sprite y posición inicial
    sf::Sprite pinkySprite(pinkyLTexture);
    pinkySprite.setPosition(0, 400); // Inicia en la izquierda
    float pinkySpeed = 200.0f;

    // Estados de Pinky: 0=izq->der hasta 300, 1=der->izq hasta salir, 2=der->izq hasta 500, 3=izq->der hasta salir
    int pinkyState = 0;
    bool running = true;
    sf::Clock clock;
    sf::Clock pinkyDelayClock;
    bool pinkyActive = false;
    bool pinkyPostCycleDelay = false;
    sf::Clock pinkyPostCycleClock;

    while (window.isOpen() && running) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        float deltaTime = clock.restart().asSeconds();
        if (!pinkyActive) {
            if (pinkyDelayClock.getElapsedTime().asSeconds() >= 6.0f) {
                pinkyActive = true;
            } else {
                window.clear(sf::Color::Black);
                window.display();
                continue;
            }
        }
        if (pinkyPostCycleDelay) {
            if (pinkyPostCycleClock.getElapsedTime().asSeconds() >= 6.0f) {
                pinkyPostCycleDelay = false;
            } else {
                window.clear(sf::Color::Black);
                window.display();
                continue;
            }
        }
        // Lógica de estados
        switch (pinkyState) {
            case 0: // Izquierda a derecha hasta X=300
                pinkySprite.setTexture(pinkyRTexture);
                pinkySprite.move(pinkySpeed * deltaTime, 0);
                if (pinkySprite.getPosition().x >= 300) {
                    pinkySprite.setPosition(300, 400);
                    pinkyState = 1;
                }
                break;
            case 1: // Derecha a izquierda desde 300 hasta salir
                pinkySprite.setTexture(pinkyLTexture);
                pinkySprite.move(-pinkySpeed * deltaTime, 0);
                if (pinkySprite.getPosition().x + pinkySprite.getGlobalBounds().width < 0) {
                    pinkySprite.setPosition(800 - pinkySprite.getGlobalBounds().width, 400);
                    pinkyState = 2;
                }
                break;
            case 2: // Derecha a izquierda hasta X=500
                pinkySprite.setTexture(pinkyLTexture);
                pinkySprite.move(-pinkySpeed * deltaTime, 0);
                if (pinkySprite.getPosition().x <= 500) {
                    pinkySprite.setPosition(500, 400);
                    pinkyState = 3;
                }
                break;
            case 3: // Izquierda a derecha desde 500 hasta salir
                pinkySprite.setTexture(pinkyRTexture);
                pinkySprite.move(pinkySpeed * deltaTime, 0);
                if (pinkySprite.getPosition().x > 800) {
                    pinkySprite.setPosition(0, 400);
                    pinkyState = 0;
                    pinkyPostCycleDelay = true;
                    pinkyPostCycleClock.restart();
                }
                break;
        }
        window.clear(sf::Color::Black);
        window.draw(pinkySprite);
        window.display();
    }
    return 0;
}
