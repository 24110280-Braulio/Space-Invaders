#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <Animacion.hpp>
#include <Enemy.hpp>
#include <EnemyManager.hpp>
#include <Game.hpp>

// Suponiendo que tienes 3 spritesheets o imágenes para PEA, PEB y PEC
const std::string PEA_PATH = "assets/images/PEA.png";
const std::string PEB_PATH = "assets/images/PEB.png";
const std::string PEC_PATH = "assets/images/PEC.png";

int main() {
    Game game;
    game.run();
    return 0;
}