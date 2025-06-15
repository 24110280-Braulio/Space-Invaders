#include <SFML/Graphics.hpp>
#include <vector>
#include "../include/Jefe.hpp"

#pragma once
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

    Animacion peaSprite(peaTexture, 44, 32, 2, 0.2f, 8); // espaciado de 8 para PEA
    Animacion pebSprite(pebTexture, 48, 32, 2, 0.2f, 8); // espaciado de 8 para PEB
    Animacion pecSprite(pecTexture, 32, 32, 2, 0.2f, 8); // espaciado de 8 para PEC

    // Posiciona cada animación en la ventana
    peaSprite.setPosition(100, 250);
    pebSprite.setPosition(350, 250);
    pecSprite.setPosition(600, 250);

    Boss boss({380, 60}); // Posición superior central
    bool bossAppeared = false;

    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float deltaTime = clock.restart().asSeconds();

        // Actualiza las animaciones
        peaSprite.update(deltaTime);
        pebSprite.update(deltaTime);
        pecSprite.update(deltaTime);

        // Lógica para detectar si todos los enemigos han sido eliminados
        // Aquí simplemente simulamos que desaparecen tras 10 segundos
        static float enemyTimer = 0;
        if (!bossAppeared) {
            enemyTimer += deltaTime;
            if (enemyTimer > 10.0f) { // Simulación: tras 10 segundos
                boss.appear();
                bossAppeared = true;
            }
        }
        boss.update(deltaTime);

        window.clear();
        if (!bossAppeared) {
            peaSprite.draw(window);
            pebSprite.draw(window);
            pecSprite.draw(window);
        } else {
            boss.draw(window);
        }
        window.display();
    }
    return 0;
}