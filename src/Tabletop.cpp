#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "Tabletop");

    // --- Barreras ---
    sf::Texture barreraTexture;
    if (!barreraTexture.loadFromFile("assets/images/Barrera.png"))
        return -1;
    const int barreraWidth = 64;
    const int numBarreras = 4;
    const int espacio = (800 - (numBarreras * barreraWidth)) / (numBarreras + 1);
    sf::Sprite barreras[numBarreras];
    for (int i = 0; i < numBarreras; ++i) {
        barreras[i].setTexture(barreraTexture);
        barreras[i].setPosition(espacio + i * (barreraWidth + espacio), 550);
    }

    // --- Player ---
    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("assets/images/space_Player.png"))
        return -1;
    sf::Sprite player(playerTexture);
    player.setPosition(400, 700);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        // Movimiento del jugador
        float velocidad = 0.5f;
        sf::Vector2f pos = player.getPosition();
        sf::Vector2u size = playerTexture.getSize();
        float minX = 0;
        float minY = 582;
        float maxX = 800 - size.x;
        float maxY = 800 - size.y;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            if (pos.y - velocidad >= minY)
                player.move(0, -velocidad);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            if (pos.y + velocidad <= maxY)
                player.move(0, velocidad);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            if (pos.x - velocidad >= minX)
                player.move(-velocidad, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            if (pos.x + velocidad <= maxX)
                player.move(velocidad, 0);

        window.clear();
        for (int i = 0; i < numBarreras; ++i)
            window.draw(barreras[i]);
        window.draw(player);
        window.display();
    }
    return 0;
}
