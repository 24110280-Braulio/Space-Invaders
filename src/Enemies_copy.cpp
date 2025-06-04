#include <SFML/Graphics.hpp>
#include <vector>

#include <SFML/Graphics.hpp>

class Animacion {
public:
    Animacion(sf::Texture& textura, int frameWidth, int frameHeight, int numFrames, float frameTime, int spacing = 0)
        : sprite(textura), frameWidth(frameWidth), frameHeight(frameHeight),
          numFrames(numFrames), frameTime(frameTime), currentFrame(0), elapsedTime(0.0f), spacing(spacing)
    {
        sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
    }

    void setPosition(float x, float y) {
        sprite.setPosition(x, y);
    }

    void update(float deltaTime) {
        elapsedTime += deltaTime;
        if (elapsedTime >= frameTime) {
            currentFrame = (currentFrame + 1) % numFrames;
            sprite.setTextureRect(
                sf::IntRect(currentFrame * (frameWidth + spacing), 0, frameWidth, frameHeight)
            );
            elapsedTime = 0.0f;
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(sprite);
    }

private:
    sf::Sprite sprite;
    int frameWidth;
    int frameHeight;
    int numFrames;
    float frameTime;
    int currentFrame;
    float elapsedTime;
    int spacing; // <-- nuevo atributo para el espaciado
};

// Suponiendo que tienes 3 spritesheets o imágenes para PEA, PEB y PEC
const std::string PEA_PATH = "assets/images/PEA.png";
const std::string PEB_PATH = "assets/images/PEB.png";
const std::string PEC_PATH = "assets/images/PEC.png";

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "Animaciones PEA, PEB, PEC");

    sf::Texture peaTexture, pebTexture, pecTexture;
    peaTexture.loadFromFile(PEA_PATH);
    pebTexture.loadFromFile(PEB_PATH);
    pecTexture.loadFromFile(PEC_PATH);

    // Crear múltiples copias de PEA en serie en X
    std::vector<Animacion> peaSprites;
    int numPea = 8;
    int peaStartX = 104;
    int peaSpacingX = 66; // Más separación para PEA
    for (int i = 0; i < numPea; ++i) {
        Animacion pea(peaTexture, 44, 32, 2, 0.2f, 8);
        pea.setPosition(peaStartX + i * peaSpacingX, 250);
        peaSprites.push_back(pea);
    }

    // Crear múltiples copias de PEB en serie en X
    std::vector<Animacion> pebSprites;
    int numPeb = 8;
    int pebStartX = 100;
    int pebSpacingX = 66; // Más separación para PEB
    for (int i = 0; i < numPeb; ++i) {
        Animacion peb(pebTexture, 48, 32, 2, 0.2f, 8);
        peb.setPosition(pebStartX + i * pebSpacingX, 300);
        pebSprites.push_back(peb);
    }

    // Crear múltiples copias de PEC en serie en X
    std::vector<Animacion> pecSprites;
    int numPec = 8; // Número de copias
    int pecStartX = 108; // X inicial
    int pecSpacingX = 66; // Espacio entre copias

    for (int i = 0; i < numPec; ++i) {
        Animacion pec(pecTexture, 32, 32, 2, 0.2f, 8);
        pec.setPosition(pecStartX + i * pecSpacingX, 400); // Y fijo, X en serie
        pecSprites.push_back(pec);
    }

    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float deltaTime = clock.restart().asSeconds();

        // Actualiza las animaciones
        for (auto& pea : peaSprites) {
            pea.update(deltaTime);
        }
        for (auto& peb : pebSprites) {
            peb.update(deltaTime);
        }
        for (auto& pec : pecSprites) {
            pec.update(deltaTime);
        }

        window.clear();
        for (auto& pea : peaSprites) {
            pea.draw(window);
        }
        for (auto& peb : pebSprites) {
            peb.draw(window);
        }
        for (auto& pec : pecSprites) {
            pec.draw(window);
        }
        window.display();
    }
    return 0;
}