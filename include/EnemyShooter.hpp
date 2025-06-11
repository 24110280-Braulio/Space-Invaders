#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <random>
#include <vector>

struct EnemyBullet {
    sf::Sprite sprite;
    int currentFrame;
    float animTime;
};

class EnemyShooter {
public:
    EnemyShooter(std::array<sf::Texture, 4>& bulletTextures, float bulletSpeed, float frameTime)
        : bulletTextures(bulletTextures), bulletSpeed(bulletSpeed), frameTime(frameTime), rng(std::random_device{}()) {}

    // Llama esto para intentar disparar
    bool tryShoot(sf::Vector2f pos, int frameWidth, int frameHeight, int shootChance) {
        std::uniform_int_distribution<int> dist(1, 20);
        int randomValue = dist(rng);
        if (randomValue < shootChance) {
            EnemyBullet eb;
            eb.sprite.setTexture(bulletTextures[0]);
            eb.sprite.setPosition(pos.x + frameWidth / 2 - bulletTextures[0].getSize().x / 2, pos.y + frameHeight);
            eb.currentFrame = 0;
            eb.animTime = 0.0f;
            bullets.push_back(eb);
            return true;
        }
        return false;
    }

    void updateBullets() {
        for (auto& eb : bullets) {
            eb.sprite.move(0, bulletSpeed);
            eb.animTime += frameTime;
            if (eb.animTime >= frameTime) {
                eb.currentFrame = (eb.currentFrame + 1) % 4;
                eb.sprite.setTexture(bulletTextures[eb.currentFrame]);
                eb.animTime = 0.0f;
            }
        }
    }

    void drawBullets(sf::RenderWindow& window) {
        for (const auto& eb : bullets)
            window.draw(eb.sprite);
    }

    std::vector<EnemyBullet>& getBullets() { return bullets; }

private:
    std::array<sf::Texture, 4>& bulletTextures;
    float bulletSpeed;
    float frameTime;
    std::vector<EnemyBullet> bullets;
    std::mt19937 rng;
};
