#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <string>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "Pantalla de Inicio");
    sf::Texture portadaTexture;
    if (!portadaTexture.loadFromFile("assets/images/Inicio.png")) {
        return -1;
    }
    sf::Sprite portada(portadaTexture);
    float scaleX = 800.0f / portadaTexture.getSize().x;
    float scaleY = 800.0f / portadaTexture.getSize().y;
    portada.setScale(scaleX, scaleY);
    portada.setPosition(0, 0);

    // Botones
    sf::Texture level1Texture, level2Texture;
    if (!level1Texture.loadFromFile("assets/images/Level (1).png") ||
        !level2Texture.loadFromFile("assets/images/Level (2).png")) {
        return -1;
    }
    sf::Sprite level1(level1Texture), level2(level2Texture);
    // Duplicar tamaño: scale 1.0f (original era 0.5f)
    level1.setScale(1.0f, 1.0f);
    level2.setScale(1.0f, 1.0f);
    // Centrar horizontalmente ambos botones
    float totalWidth = level1.getGlobalBounds().width + 80 + level2.getGlobalBounds().width; // 80 px de separación
    float startX = (800 - totalWidth) / 2.0f;
    float y = 300.0f;
    level1.setPosition(startX, y);
    level2.setPosition(startX + level1.getGlobalBounds().width + 80, y);

    // Fuente y texto de leyenda
    sf::Font font;
    if (!font.loadFromFile("assets/fonts/AngelicWar.ttf")) {
        return -1;
    }
    sf::Text leyenda;
    leyenda.setFont(font);
    leyenda.setCharacterSize(48);
    leyenda.setFillColor(sf::Color::White);
    leyenda.setStyle(sf::Text::Bold);
    leyenda.setPosition(250, 600);

    sf::Clock clock;
    bool mostrarPortada = true;
    bool enMenu = false;
    bool mostrandoLeyenda = false;
    std::string textoLeyenda;
    sf::Clock escClock;
    bool escapePresionado = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            // Si estamos mostrando una leyenda, permitir volver al menú con Escape
            if (mostrandoLeyenda) {
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                    escapePresionado = true;
                    escClock.restart();
                }
                if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape) {
                    escapePresionado = false;
                }
            }
            // Si estamos en el menú, detectar clics en los botones
            if (enMenu && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                if (level1.getGlobalBounds().contains(mousePos)) {
                    textoLeyenda = "Tabletop";
                    mostrandoLeyenda = true;
                    enMenu = false;
                } else if (level2.getGlobalBounds().contains(mousePos)) {
                    textoLeyenda = "Pacfight";
                    mostrandoLeyenda = true;
                    enMenu = false;
                }
            }
        }
        // Ocultar portada tras 2 segundos y mostrar menú
        if (mostrarPortada && clock.getElapsedTime().asSeconds() >= 2.0f) {
            mostrarPortada = false;
            enMenu = true;
        }
        // Si Escape se mantiene presionado por 0.3s, regresar al menú
        if (mostrandoLeyenda && escapePresionado && escClock.getElapsedTime().asSeconds() > 0.3f) {
            mostrandoLeyenda = false;
            enMenu = true;
            escapePresionado = false;
        }
        window.clear();
        if (mostrarPortada) {
            window.draw(portada);
        } else if (enMenu) {
            window.draw(level1);
            window.draw(level2);
        } else if (mostrandoLeyenda) {
            leyenda.setString(textoLeyenda);
            window.draw(leyenda);
        }
        window.display();
    }
    return 0;
}
