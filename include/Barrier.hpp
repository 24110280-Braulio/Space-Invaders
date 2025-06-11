#pragma once
#include <SFML/Graphics.hpp>

class Barrier {
public:
    Barrier(const sf::Texture& normalTex, const sf::Texture& lowTex, sf::Vector2f pos, int maxHP = 60)
        : normalTexture(normalTex), lowTexture(lowTex), hp(maxHP), maxHp(maxHP) {
        sprite.setTexture(normalTexture);
        sprite.setPosition(pos);
    }

    void takeDamage(int dmg) {
        if (hp <= 0) return;
        hp -= dmg;
        if (hp <= 20 && hp > 0) {
            sprite.setTexture(lowTexture);
        }
        if (hp < 0) hp = 0;
    }

    bool isAlive() const { return hp > 0; }
    sf::Sprite& getSprite() { return sprite; }
    int getHP() const { return hp; }

private:
    sf::Sprite sprite;
    const sf::Texture& normalTexture;
    const sf::Texture& lowTexture;
    int hp;
    int maxHp;
};

class PlayerHealth {
public:
    PlayerHealth(int maxHP = 60) : hp(maxHP), maxHp(maxHP) {}
    void takeDamage(int dmg) { if (hp > 0) hp -= dmg; if (hp < 0) hp = 0; }
    bool isAlive() const { return hp > 0; }
    int getHP() const { return hp; }
private:
    int hp;
    int maxHp;
};
