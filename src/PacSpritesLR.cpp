#include <SFML/Graphics.hpp>
#include <string>

class PacSprite : public sf::Sprite {
public:
    PacSprite(const std::string& basePath) {
        // Cargar los 3 frames para la animación hacia la izquierda
        textureLeft[0].loadFromFile(basePath + "PACmove (Base).png");
        textureLeft[1].loadFromFile(basePath + "PACmove2 (Left).png");
        textureLeft[2].loadFromFile(basePath + "PACmove3 (Left).png");
        // Cargar los 3 frames para la animación hacia la derecha
        textureRight[0].loadFromFile(basePath + "PACmove (Base).png");
        textureRight[1].loadFromFile(basePath + "PACmove2 (Right).png");
        textureRight[2].loadFromFile(basePath + "PACmove3 (Right).png");
        setTexture(textureRight[0]);
        currentFrame = 0;
        frameTime = 0.15f;
        animClock.restart();
    }

    void animateLeft() {
        if (animClock.getElapsedTime().asSeconds() > frameTime) {
            currentFrame = (currentFrame + 1) % 3;
            setTexture(textureLeft[currentFrame]);
            animClock.restart();
        }
    }
    void animateRight() {
        if (animClock.getElapsedTime().asSeconds() > frameTime) {
            currentFrame = (currentFrame + 1) % 3;
            setTexture(textureRight[currentFrame]);
            animClock.restart();
        }
    }
private:
    sf::Texture textureLeft[3];
    sf::Texture textureRight[3];
    int currentFrame;
    float frameTime;
    sf::Clock animClock;
};

int main() {
    sf::RenderWindow window(sf::VideoMode(400, 400), "PacSprite Izquierda-Derecha");
    PacSprite pac("assets/images/Boss2nd/");
    pac.setPosition(window.getSize().x - 50, 200);
    float speed = 200.f; // píxeles por segundo
    float direction = -1.f; // -1 para izquierda, 1 para derecha

    sf::Clock clock;
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        // Animación según dirección
        if (direction == -1.f) {
            pac.animateLeft();
        } else {
            pac.animateRight();
        }
        pac.move(speed * direction * deltaTime, 0);
        // Cambia de dirección al tocar los bordes
        if (pac.getPosition().x < -pac.getGlobalBounds().width) {
            direction = 1.f;
            pac.setPosition(-pac.getGlobalBounds().width, 200);
        } else if (pac.getPosition().x > window.getSize().x) {
            direction = -1.f;
            pac.setPosition(window.getSize().x, 200);
        }
        window.clear();
        window.draw(pac);
        window.display();
    }
    return 0;
}
