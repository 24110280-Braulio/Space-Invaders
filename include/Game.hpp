#pragma once
#include <SFML/Graphics.hpp>
#include <Animacion.hpp>
#include <Enemy.hpp>
#include <EnemyManager.hpp>
#include <string>
#include <vector>

class Game {
public:
    Game()
        : window(sf::VideoMode(800, 800), "Animaciones PEA, PEB, PEC")
    {
        peaTexture.loadFromFile(PEA_PATH);
        pebTexture.loadFromFile(PEB_PATH);
        pecTexture.loadFromFile(PEC_PATH);
        // Agregar PEA
        enemyManager.addEnemies(peaTexture, 44, 32, 2, 0.2f, 8, 8, 104, 250, 66);
        // Agregar PEB
        enemyManager.addEnemies(pebTexture, 48, 32, 2, 0.2f, 8, 8, 100, 300, 66);
        // Agregar PEC
        enemyManager.addEnemies(pecTexture, 32, 32, 2, 0.2f, 8, 8, 108, 400, 66);
    }
    void run() {
        while (window.isOpen()) {
            processEvents();
            float deltaTime = clock.restart().asSeconds();
            update(deltaTime);
            render();
        }
    }
private:
    void processEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }
    void update(float deltaTime) {
        enemyManager.update(deltaTime);
    }
    void render() {
        window.clear();
        enemyManager.draw(window);
        window.display();
    }
    sf::RenderWindow window;
    EnemyManager enemyManager;
    sf::Texture peaTexture, pebTexture, pecTexture;
    const std::string PEA_PATH = "assets/images/PEA.png";
    const std::string PEB_PATH = "assets/images/PEB.png";
    const std::string PEC_PATH = "assets/images/PEC.png";
    sf::Clock clock;
};
