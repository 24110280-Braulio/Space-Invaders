#include <SFML/Graphics.hpp>
#include <vector>

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

    // Crear 8 enemigos
    int numEnemies = 8;
    std::vector<sf::Sprite> enemies;
    std::vector<sf::Vector2f> positions;
    std::vector<float> directions(numEnemies, 1.0f); // 1: derecha, -1: izquierda

    for (int i = 0; i < numEnemies; ++i) {
        sf::Sprite enemy(texture);
        enemy.setPosition(60.0f + i * 90.0f, 100); // Pea en el medio
        enemy.setTextureRect(sf::IntRect(0, 0, 44, 32));
        enemies.push_back(enemy);
        positions.push_back(enemy.getPosition());
    }

    sf::Clock clock;
    float frameTime = 0.1f; // Tiempo entre cada frame en segundos
    int currentFrame = 0;
    int numFrames = 2; // Número total de frames en la animación
    int frameWidth = 44;
    int frameHeight = 32;

    // Variables para el movimiento
    float velocity = 100.0f; // pixeles por segundo
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

        // Determinar si hay rebote en los bordes para toda la fila
        bool bounceLeft = positions[0].x < 32;
        bool bounceRight = positions[numEnemies - 1].x > 768 - frameWidth;

        // Si hay rebote, bajar todos y cambiar dirección
        if (bounceLeft) {
            for (int i = 0; i < numEnemies; ++i) {
                positions[i].x = 32 + (i * (positions[1].x - positions[0].x)); // mantener espaciado
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
            // Movimiento normal
            for (int i = 0; i < numEnemies; ++i) {
                positions[i].x += velocity * directions[i] * deltaTime;
            }
        }

        // Aumentar velocidad si alguno pasa Y=550
        for (int i = 0; i < numEnemies; ++i) {
            if (positions[i].y > 550) {
                velocity = 200.0f;
            }
            enemies[i].setPosition(positions[i]);
        }

        // Actualizar la animación
        static float animTime = 0.0f;
        animTime += deltaTime;
        if (animTime >= frameTime)
        {
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
