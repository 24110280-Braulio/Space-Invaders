#include <SFML/Graphics.hpp>
#include <string>

class PacSprite : public sf::Sprite {
public:
    PacSprite(const std::string& basePath) {
        // Cargar los 3 frames para la animación hacia abajo
        textureDown[0].loadFromFile(basePath + "PACmove (Base).png");
        textureDown[1].loadFromFile(basePath + "PACmove2 (Down).png");
        textureDown[2].loadFromFile(basePath + "PACmove3 (Down).png");
        // Cargar los 3 frames para la animación hacia arriba
        textureUp[0].loadFromFile(basePath + "PACmove (Base).png");
        textureUp[1].loadFromFile(basePath + "PACmove2 (Up).png");
        textureUp[2].loadFromFile(basePath + "PACmove3 (Up).png");
        setTexture(textureDown[0]);
        currentFrame = 0;
        frameTime = 0.15f;
        animClock.restart();
    }

    void animateDown() {
        if (animClock.getElapsedTime().asSeconds() > frameTime) {
            currentFrame = (currentFrame + 1) % 3;
            setTexture(textureDown[currentFrame]);
            animClock.restart();
        }
    }

    void animateUp() {
        if (animClock.getElapsedTime().asSeconds() > frameTime) {
            currentFrame = (currentFrame + 1) % 3;
            setTexture(textureUp[currentFrame]);
            animClock.restart();
        }
    }

    void setBaseFrame() {
        setTexture(textureDown[0]);
    }
private:
    sf::Texture textureDown[3];
    sf::Texture textureUp[3];
    int currentFrame;
    float frameTime;
    sf::Clock animClock;
};

int main() {
    sf::RenderWindow window(sf::VideoMode(400, 400), "PacSprite Arriba-Abajo");
    PacSprite pac("assets/images/Boss2nd/");
    pac.setPosition(200, 0);
    float speed = 200.f; // píxeles por segundo
    float direction = 1.f; // 1 para abajo, -1 para arriba

    sf::Clock clock;
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        // Animación según dirección
        if (direction == 1.f) {
            pac.animateDown();
        } else {
            pac.animateUp();
        }
        pac.move(0, speed * direction * deltaTime);
        if (pac.getPosition().y > window.getSize().y - pac.getGlobalBounds().height) {
            direction = -1.f;
            pac.setPosition(pac.getPosition().x, window.getSize().y - pac.getGlobalBounds().height);
        } else if (pac.getPosition().y < 0) {
            direction = 1.f;
            pac.setPosition(pac.getPosition().x, 0);
        }
        window.clear();
        window.draw(pac);
        window.display();
    }
    return 0;
}
