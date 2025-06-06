#include <SFML/Graphics.hpp>

int main()
{
    // Crear una ventana
    sf::RenderWindow window(sf::VideoMode(800, 800), "Sprite Animado");

    // Cargar la imagen desde un archivo
    sf::Texture texture;
    if (!texture.loadFromFile("assets/images/PEA.png"))
    {
        // Manejo de error si no se puede cargar la imagen
        return -1;
    }

    // Crear un sprite y asignarle la textura
    sf::Sprite sprite(texture);
    sprite.setPosition(0, 0); // Posición inicial del sprite

    sf::Clock clock;
    float frameTime = 0.1f; // Tiempo entre cada frame en segundos
    int currentFrame = 0;
    int numFrames = 2; // Número total de frames en la animación
    int frameWidth = 44;
    int frameHeight = 32;

    // Variables para el movimiento
    float velocity = 500.0f; // pixeles por segundo
    float direction = 1.0f;  // 1: derecha, -1: izquierda

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

        // Movimiento automático
        float deltaTime = clock.restart().asSeconds();
        sf::Vector2f pos = sprite.getPosition();
        pos.x += velocity * direction * deltaTime;

        // Rebotar en los bordes
        if (pos.x < 0)
        {
            pos.x = 0;
            pos.y += 32; // Bajar 32 píxeles en Y al tocar el borde izquierdo
            direction = 1.0f;
        }
        else if (pos.x > 800 - frameWidth)
        {
            pos.x = 800 - frameWidth;
            pos.y += 32; // Bajar 32 píxeles en Y al tocar el borde derecho
            direction = -1.0f;
        }

        // Aumentar velocidad si pasa Y=550
        if (pos.y > 550)
        {
            velocity = 700.0f; // Nueva velocidad (ajusta el valor si lo deseas)
        }

        sprite.setPosition(pos);

        // Actualizar la animación
        static float animTime = 0.0f;
        animTime += deltaTime;
        if (animTime >= frameTime)
        {
            currentFrame = (currentFrame + 1) % numFrames;
            int x = (currentFrame == 0) ? 0 : (frameWidth + 8);
            sprite.setTextureRect(sf::IntRect(x, 0, frameWidth, frameHeight));
            animTime = 0.0f;
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}
