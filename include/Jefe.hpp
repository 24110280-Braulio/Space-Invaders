#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Boss {
public:
    Boss(const sf::Vector2f& startPos);
    void update(float deltaTime, const sf::Vector2f& playerPos);
    void draw(sf::RenderWindow& window);
    void appear();
    bool isActive() const;
    void reset();
    sf::FloatRect getBounds() const;
    void shoot();
    std::vector<sf::Sprite>& getLasers();

private:
    sf::Sprite spriteA;
    sf::Sprite spriteB;
    sf::Texture textureA;
    sf::Texture textureB;
    sf::Sprite laser1;
    sf::Sprite laser2;
    sf::Texture laserTexture1;
    sf::Texture laserTexture2;
    bool active;
    float animTime;
    float animSwitch;
    float moveTimer;
    sf::Vector2f originalPos;
    float moveRadiusX;
    float moveRadiusY;
    float moveSpeed;
    bool useA;
    std::vector<sf::Sprite> lasers;
    float laserCooldown; // tiempo entre disparos
    float laserTimer;    // temporizador

    // --- Ataque especial ---
    bool specialAttackActive = false;
    float specialAttackTimer = 0.0f;
    float specialAttackDuration = 6.0f;
    float specialAttackCooldown = 8.0f;
    float specialAttackElapsed = 0.0f;
    bool specialLaserOn = false;
    float specialLaserTimer = 0.0f;
    float specialLaserInterval = 2.0f;
};
