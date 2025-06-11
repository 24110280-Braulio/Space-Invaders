#include <SFML/Graphics.hpp>
#include <array>
#include <string>

int main()
{
    // Crear una ventana
    sf::RenderWindow window(sf::VideoMode(800, 800), "Sprite Animado");

    // Cargar las 4 texturas de los archivos Eshoot(1-4).png
    std::array<sf::Texture, 4> textures;
    for (int i = 0; i < 4; ++i) {
        std::string filename = "assets/images/Eshoot(" + std::to_string(i + 1) + ").png";
        if (!textures[i].loadFromFile(filename)) {
            return -1; // Manejo de error si no se puede cargar la imagen
        }
    }

    // Crear un sprite y asignarle la primera textura
    sf::Sprite sprite(textures[0]);
    sprite.setPosition(400, 400);

    sf::Clock clock;
    float frameTime = 0.1f; // Tiempo entre cada frame en segundos
    int currentFrame = 0;
    int numFrames = 4; // Número total de frames en la animación

    while (window.isOpen())
    {
        // Procesar eventos
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        // Actualizar la animación
        if (clock.getElapsedTime().asSeconds() >= frameTime)
        {
            currentFrame = (currentFrame + 1) % numFrames;
            sprite.setTexture(textures[currentFrame]);
            clock.restart();
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}
