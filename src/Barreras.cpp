#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "Barreras");

    sf::Texture barreraTexture;
    if (!barreraTexture.loadFromFile("assets/images/Barrera.png"))
        return -1;

    sf::Texture lowBarreraTexture;
    if (!lowBarreraTexture.loadFromFile("assets/images/Lowbarrier.png"))
        return -1;

    const int barreraWidth = 64;
    const int numBarreras = 4;
    const int espacio = (800 - (numBarreras * barreraWidth)) / (numBarreras + 1);

    sf::Sprite barreras[numBarreras];
    for (int i = 0; i < numBarreras; ++i) {
        barreras[i].setTexture(barreraTexture);
        barreras[i].setPosition(espacio + i * (barreraWidth + espacio), 550);
    }

    // Contador que parte de 100
    int contador = 100;
    sf::Clock clock;
    float tiempoAcumulado = 0.0f;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        // Ciclo que desciende rápidamente el contador
        float delta = clock.restart().asSeconds();
        tiempoAcumulado += delta;
        while (tiempoAcumulado > 0.03f && contador > 0) {
            contador--;
            tiempoAcumulado -= 0.03f;
        }

        // Cambia el sprite si el contador está por debajo de 30
        if (contador < 30 && contador > 0) {
            for (int i = 0; i < numBarreras; ++i) {
                barreras[i].setTexture(lowBarreraTexture);
            }
        }

        window.clear();
        // Si el contador es mayor que 0, dibuja las barreras
        if (contador > 0) {
            for (int i = 0; i < numBarreras; ++i)
                window.draw(barreras[i]);
        }
        window.display();
    }
    return 0;
}