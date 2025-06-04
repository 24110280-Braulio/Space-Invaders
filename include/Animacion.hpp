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

    void setPosition(float x, float y) { sprite.setPosition(x, y); }

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

    void draw(sf::RenderWindow& window) { window.draw(sprite); }

    const sf::Sprite& getSprite() const { return sprite; }

private:
    sf::Sprite sprite;
    int frameWidth;
    int frameHeight;
    int numFrames;
    float frameTime;
    int currentFrame;
    float elapsedTime;
    int spacing;
};
