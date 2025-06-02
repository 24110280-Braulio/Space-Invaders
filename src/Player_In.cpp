#include <SFML/Graphics.hpp>

int main()
{
    // Crear la ventana
    sf::RenderWindow window(sf::VideoMode(800, 800), "Space 0006");

    // Cargar la imagen desde un archivo
    sf::Texture texture;
    if (!texture.loadFromFile("assets/images/space_Player.png"))
    {
        // Manejo de error si no se puede cargar la imagen
        return -1;
    }
    sf::Sprite sprite(texture);
    //sprite.setScale(4.f, 4.f); Aumenta el tamaño del jugador (Opción Descartada)
    sprite.setPosition(400, 700);

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

        // Agregado con Copilot
        // Movimiento del jugador con el teclado
        float velocidad = 0.5f;
        sf::Vector2f pos = sprite.getPosition();
        sf::Vector2u size = texture.getSize();
        // Limites de la ventana
        float minX = 0;
        float minY = 550;
        float maxX = 800 - size.x;
        float maxY = 800 - size.y;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            if (pos.y - velocidad >= minY)
                sprite.move(0, -velocidad);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            if (pos.y + velocidad <= maxY)
                sprite.move(0, velocidad);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            if (pos.x - velocidad >= minX)
                sprite.move(-velocidad, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            if (pos.x + velocidad <= maxX)
                sprite.move(velocidad, 0);
        }

        // Limpiar la ventana
        window.clear();

        // Dibujar el sprite en la ventana
        window.draw(sprite);

        // Mostrar la ventana
        window.display();
    }

    return 0;
}