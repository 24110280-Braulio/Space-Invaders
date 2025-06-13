#pragma once
#include <SFML/Graphics.hpp>

class PlayerHealth {
public:
    PlayerHealth(int maxHP = 100)
        : hp(maxHP), maxHp(maxHP), invulnerable(false) {}

    void setTextures(const sf::Texture& hp100, const sf::Texture& hp80, const sf::Texture& hp60, const sf::Texture& hp40, const sf::Texture& hp20, const sf::Texture& hp0, const sf::Texture& timeout) {
        hp100Texture = &hp100;
        hp80Texture = &hp80;
        hp60Texture = &hp60;
        hp40Texture = &hp40;
        hp20Texture = &hp20;
        hp0Texture = &hp0;
        timeoutTexture = &timeout;
        hpSprite.setTexture(hp100, true);
        hpSprite.setPosition(0, 0);
        hpSprite.setScale(68.0f / hp100.getSize().x, 28.0f / hp100.getSize().y);
    }

    void takeDamage(int dmg) {
        if (!invulnerable && isAlive()) {
            hp -= dmg;
            if (hp < 0) hp = 0;
            invulnerable = true;
            invulClock.restart();
        }
    }

    void update() {
        if (invulnerable && invulClock.getElapsedTime().asSeconds() >= 2.0f) {
            invulnerable = false;
        }
        updateSprite();
    }

    bool isAlive() const { return hp > 0; }
    int getHP() const { return hp; }
    bool isInvulnerable() const { return invulnerable; }
    sf::Sprite& getSprite() { return hpSprite; }

private:
    void updateSprite() {
        if (invulnerable && timeoutTexture) {
            hpSprite.setTexture(*timeoutTexture, true);
            hpSprite.setScale(68.0f / timeoutTexture->getSize().x, 28.0f / timeoutTexture->getSize().y);
        } else if (hp100Texture && hp80Texture && hp60Texture && hp40Texture && hp20Texture && hp0Texture) {
            switch (hp) {
                case 100:
                    hpSprite.setTexture(*hp100Texture, true);
                    hpSprite.setScale(68.0f / hp100Texture->getSize().x, 28.0f / hp100Texture->getSize().y);
                    break;
                case 80:
                    hpSprite.setTexture(*hp80Texture, true);
                    hpSprite.setScale(68.0f / hp80Texture->getSize().x, 28.0f / hp80Texture->getSize().y);
                    break;
                case 60:
                    hpSprite.setTexture(*hp60Texture, true);
                    hpSprite.setScale(68.0f / hp60Texture->getSize().x, 28.0f / hp60Texture->getSize().y);
                    break;
                case 40:
                    hpSprite.setTexture(*hp40Texture, true);
                    hpSprite.setScale(68.0f / hp40Texture->getSize().x, 28.0f / hp40Texture->getSize().y);
                    break;
                case 20:
                    hpSprite.setTexture(*hp20Texture, true);
                    hpSprite.setScale(68.0f / hp20Texture->getSize().x, 28.0f / hp20Texture->getSize().y);
                    break;
                case 0:
                    hpSprite.setTexture(*hp0Texture, true);
                    hpSprite.setScale(68.0f / hp0Texture->getSize().x, 28.0f / hp0Texture->getSize().y);
                    break;
                default:
                    break;
            }
        }
    }

    int hp;
    int maxHp;
    bool invulnerable;
    sf::Clock invulClock;
    sf::Sprite hpSprite;
    const sf::Texture *hp100Texture = nullptr, *hp80Texture = nullptr, *hp60Texture = nullptr, *hp40Texture = nullptr, *hp20Texture = nullptr, *hp0Texture = nullptr, *timeoutTexture = nullptr;
};
