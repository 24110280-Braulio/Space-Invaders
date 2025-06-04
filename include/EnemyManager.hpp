#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Enemy.hpp"

class EnemyManager {
public:
    void addEnemies(sf::Texture& textura, int frameWidth, int frameHeight, int numFrames, float frameTime, int spacing,
                   int cantidad, float startX, float startY, float spacingX)
    {
        for (int i = 0; i < cantidad; ++i) {
            enemies.emplace_back(textura, frameWidth, frameHeight, numFrames, frameTime, spacing,
                                startX + i * spacingX, startY);
        }
    }

    void update(float deltaTime) {
        for (auto& enemy : enemies) {
            enemy.update(deltaTime);
        }
    }

    void draw(sf::RenderWindow& window) {
        for (auto& enemy : enemies) {
            enemy.draw(window);
        }
    }

    void moveEnemiesDown(float offsetY) {
        for (auto& enemy : enemies) {
            sf::Vector2f pos = enemy.getPosition();
            pos.y += offsetY;
            enemy.setPosition(pos);
        }
    }

private:
    std::vector<Enemy> enemies;
};
