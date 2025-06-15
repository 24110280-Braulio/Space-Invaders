#include <cmath>
#include "../include/Jefe.hpp"

Boss::Boss(const sf::Vector2f& startPos)
    : active(false), animTime(0), animSwitch(0.3f), moveTimer(0), originalPos(startPos), moveRadiusX(40), moveRadiusY(20), moveSpeed(1.2f), useA(true), laserCooldown(2.0f), laserTimer(0)
{
    textureA.loadFromFile("assets/images/Boss1_A.png");
    textureB.loadFromFile("assets/images/Boss1_B.png");
    spriteA.setTexture(textureA);
    spriteB.setTexture(textureB);
    spriteA.setPosition(startPos);
    spriteB.setPosition(startPos);
    laserTexture1.loadFromFile("assets/images/Laser1.png");
    laserTexture2.loadFromFile("assets/images/Laser2.png");
    laser1.setTexture(laserTexture1);
    laser2.setTexture(laserTexture2);
    // Posición inicial de los lasers (ajustar según sprite)
    laser1.setPosition(startPos.x + 30, startPos.y + 80);
    laser2.setPosition(startPos.x + 80, startPos.y + 80);
}

void Boss::appear() {
    active = true;
}

void Boss::reset() {
    active = false;
    animTime = 0;
    moveTimer = 0;
    useA = true;
    spriteA.setPosition(originalPos);
    spriteB.setPosition(originalPos);
}

bool Boss::isActive() const {
    return active;
}

void Boss::update(float deltaTime) {
    if (!active) return;
    animTime += deltaTime;
    moveTimer += deltaTime * moveSpeed;
    // Animación entre A y B
    if (animTime > animSwitch) {
        useA = !useA;
        animTime = 0;
    }
    // Movimiento suave en X e Y (oscilatorio)
    float x = originalPos.x + moveRadiusX * sin(moveTimer);
    float y = originalPos.y + moveRadiusY * cos(moveTimer * 0.7f);
    spriteA.setPosition(x, y);
    spriteB.setPosition(x, y);
    laser1.setPosition(x + 30, y + 80);
    laser2.setPosition(x + 80, y + 80);
    // --- Disparo del jefe ---
    laserTimer += deltaTime;
    if (laserTimer >= laserCooldown) {
        shoot();
        laserTimer = 0;
    }
    // Animación de los lasers activos
    for (auto& l : lasers) {
        // Efecto de parpadeo/animación usando Laser1 y Laser2
        float anim = fmod(animTime, 0.3f);
        if (anim < 0.15f) {
            l.setTexture(laserTexture1);
        } else {
            l.setTexture(laserTexture2);
        }
        l.move(0, 3.0f); // velocidad hacia abajo
    }
    // Eliminar lasers fuera de pantalla
    lasers.erase(std::remove_if(lasers.begin(), lasers.end(), [](const sf::Sprite& l){
        return l.getPosition().y > 800;
    }), lasers.end());
}

void Boss::shoot() {
    float x = useA ? spriteA.getPosition().x : spriteB.getPosition().x;
    float y = useA ? spriteA.getPosition().y : spriteB.getPosition().y;
    // Laser principal (Laser1) desde el boss
    sf::Sprite l1(laserTexture1);
    l1.setPosition(x + 30, y + 80);
    lasers.push_back(l1);
    // Laser continuo (Laser2) desde el final de Laser1 hasta el fondo de la pantalla
    float laser2Y = y + 80 + laserTexture1.getSize().y;
    while (laser2Y < 800) {
        sf::Sprite l2(laserTexture2);
        l2.setPosition(x + 30, laser2Y);
        lasers.push_back(l2);
        laser2Y += laserTexture2.getSize().y;
    }
}

std::vector<sf::Sprite>& Boss::getLasers() {
    return lasers;
}

void Boss::draw(sf::RenderWindow& window) {
    if (!active) return;
    if (useA)
        window.draw(spriteA);
    else
        window.draw(spriteB);
    // Solo dibujar lasers activos (no los estáticos)
    for (auto& l : lasers) window.draw(l);
}

sf::FloatRect Boss::getBounds() const {
    return useA ? spriteA.getGlobalBounds() : spriteB.getGlobalBounds();
}
