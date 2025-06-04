#include <SFML/Graphics.hpp>

int main()
{
    // Crear una ventana
    sf::RenderWindow window(sf::VideoMode(800, 800), "Sprite Animado");

    // Cargar la imagen desde un archivo
    sf::Texture texture;
    if (!texture.loadFromFile("assets/images/PEC.png"))
    {
        // Manejo de error si no se puede cargar la imagen
        return -1;
    }

    // Crear un sprite y asignarle la textura
    sf::Sprite sprite(texture);
    sprite.setPosition(400, 400);

    sf::Clock clock;
    float frameTime = 1.0f; // Tiempo entre cada frame en segundos
    int currentFrame = 0;
    int numFrames = 2; // Número total de frames en la animación
    int frameWidth = 32;
    int frameHeight = 32;

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
            // Frame 0: (0,0,44,32), Frame 1: (52,0,44,32)
            int x = (currentFrame == 0) ? 0 : (frameWidth + 8);
            sprite.setTextureRect(sf::IntRect(x, 0, frameWidth, frameHeight));
            clock.restart();
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}
