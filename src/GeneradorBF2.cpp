#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>

struct GoalObj {
    sf::Sprite sprite;
    int x, y;
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "Generador de Goals");
    sf::Texture goalTexture;
    goalTexture.loadFromFile("assets/images/Boss2nd/Goal.png");
    std::vector<GoalObj> goals;
    sf::Clock spawnClock;
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Generar un nuevo Goal cada 2 segundos si hay menos de 5
        if (spawnClock.getElapsedTime().asSeconds() >= 2.0f && goals.size() < 5) {
            int x = 32 + std::rand() % (768 - 32 + 1);
            int y = 32 + std::rand() % (500 - 32 + 1);
            bool exists = false;
            for (const auto& g : goals) {
                if (g.x == x && g.y == y) {
                    exists = true;
                    break;
                }
            }
            if (!exists) {
                GoalObj obj;
                obj.sprite.setTexture(goalTexture);
                obj.sprite.setPosition(static_cast<float>(x), static_cast<float>(y));
                obj.x = x;
                obj.y = y;
                goals.push_back(obj);
                spawnClock.restart();
            }
        }

        window.clear(sf::Color::Black);
        for (const auto& g : goals)
            window.draw(g.sprite);
        window.display();
    }
    return 0;
}
