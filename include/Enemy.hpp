#pragma once

#include <SFML/Graphics.hpp>
#include "Animacion.hpp"

class Enemy {
public:
    Enemy(sf::Texture& textura, int frameWidth, int frameHeight, int numFrames, float frameTime, int spacing, float x, float y)
        : animacion(textura, frameWidth, frameHeight, numFrames, frameTime, spacing)
    {
        animacion.setPosition(x, y);
    }

    void update(float deltaTime) {
        animacion.update(deltaTime);
    }

    void draw(sf::RenderWindow& window) {
        animacion.draw(window);
    }

    sf::Vector2f getPosition() const {
        return animacion.getSprite().getPosition();
    }

    void setPosition(const sf::Vector2f& pos) {
        animacion.setPosition(pos.x, pos.y);
    }

    sf::FloatRect getGlobalBounds() const {
        return animacion.getSprite().getGlobalBounds();
    }

private:
    Animacion animacion;
};
