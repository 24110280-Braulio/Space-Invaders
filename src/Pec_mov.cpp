#include <SFML/Graphics.hpp>
#include <vector>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "Sprite Animado Pec");

    sf::Texture texture;
    if (!texture.loadFromFile("assets/images/PEC.png"))
        return -1;

    int frameWidth = 32;
    int frameHeight = 32;
    int numFrames = 2;
    int currentFrame = 0;
    float frameTime = .1f;
    float animTime = 0.0f;
    int numEnemies = 8;
    float velocity = 100.0f;
    std::vector<sf::Sprite> enemies;
    std::vector<sf::Vector2f> positions;
    std::vector<float> directions(numEnemies, 1.0f);
    for (int i = 0; i < numEnemies; ++i) {
        sf::Sprite enemy(texture);
        enemy.setPosition(60.0f + i * 90.0f, 0); // Pec arriba
        enemy.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
        enemies.push_back(enemy);
        positions.push_back(enemy.getPosition());
    }
    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        float deltaTime = clock.restart().asSeconds();
        bool bounceLeft = positions[0].x < 32;
        bool bounceRight = positions[numEnemies - 1].x > 768 - frameWidth;
        if (bounceLeft) {
            for (int i = 0; i < numEnemies; ++i) {
                positions[i].x = 32 + (i * (positions[1].x - positions[0].x));
                positions[i].y += 32;
                directions[i] = 1.0f;
            }
        } else if (bounceRight) {
            for (int i = 0; i < numEnemies; ++i) {
                positions[i].x = 768 - frameWidth - (numEnemies - 1 - i) * (positions[1].x - positions[0].x);
                positions[i].y += 32;
                directions[i] = -1.0f;
            }
        } else {
            for (int i = 0; i < numEnemies; ++i) {
                positions[i].x += velocity * directions[i] * deltaTime;
            }
        }
        for (int i = 0; i < numEnemies; ++i) {
            if (positions[i].y > 550) {
                velocity = 200.0f;
            }
            enemies[i].setPosition(positions[i]);
        }
        animTime += deltaTime;
        if (animTime >= frameTime) {
            currentFrame = (currentFrame + 1) % numFrames;
            int x = (currentFrame == 0) ? 0 : (frameWidth + 8);
            for (int i = 0; i < numEnemies; ++i) {
                enemies[i].setTextureRect(sf::IntRect(x, 0, frameWidth, frameHeight));
            }
            animTime = 0.0f;
        }
        window.clear();
        for (int i = 0; i < numEnemies; ++i) {
            window.draw(enemies[i]);
        }
        window.display();
    }
    return 0;
}
