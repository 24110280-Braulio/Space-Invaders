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

// --- NUEVO: variables para ataque especial y alternancia ---
bool specialAttackActive = false;
float specialAttackTimer = 0.0f;
float specialAttackDuration = 30.0f; // Duración total del ataque especial
float specialAttackCooldown = 20.0f; // Tiempo entre ataques especiales (ataque normal)
float specialAttackElapsed = 0.0f;
bool specialLaserOn = false;
float specialLaserTimer = 0.0f;
float specialLaserInterval = 2.0f; // Cada 2 segundos se apaga/prende
bool inNormalAttack = true;

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

void Boss::update(float deltaTime, const sf::Vector2f& playerPos) {
    if (!active) return;
    animTime += deltaTime;
    moveTimer += deltaTime * moveSpeed;
    // Animación entre A y B
    if (animTime > animSwitch) {
        useA = !useA;
        animTime = 0;
    }
    // Movimiento mejorado: jefe siempre visible y recorre toda la pantalla
    float bossWidth = spriteA.getGlobalBounds().width;
    float minX = 0;
    float maxX = 800 - bossWidth;
    float x = minX + (maxX - minX) * (0.5f + 0.5f * sin(moveTimer));
    float y = 60 + 40 * sin(moveTimer * 0.7f) + 20 * cos(moveTimer * 1.3f);
    if (y < 20) y = 20;
    if (y > 200) y = 200;
    spriteA.setPosition(x, y);
    spriteB.setPosition(x, y);
    laser1.setPosition(x + 30, y + 80);
    laser2.setPosition(x + 80, y + 80);

    // --- Alternancia entre ataques ---
    if (inNormalAttack) {
        specialAttackElapsed += deltaTime;
        if (specialAttackElapsed >= specialAttackCooldown) {
            inNormalAttack = false;
            specialAttackActive = true;
            specialAttackTimer = 0.0f;
            specialLaserOn = true;
            specialLaserTimer = 0.0f;
            specialAttackElapsed = 0.0f;
            lasers.clear();
        }
    }
    if (specialAttackActive) {
        specialAttackTimer += deltaTime;
        specialLaserTimer += deltaTime;
        // Apagar/prender columna cada 2 segundos
        if (specialLaserTimer >= specialLaserInterval) {
            specialLaserOn = !specialLaserOn;
            specialLaserTimer = 0.0f;
        }
        // Movimiento normal por el mapa (NO seguir al jugador)
        float bossWidth = spriteA.getGlobalBounds().width;
        float minX = 0;
        float maxX = 800 - bossWidth;
        float x = minX + (maxX - minX) * (0.5f + 0.5f * sin(moveTimer));
        float y = 60 + 40 * sin(moveTimer * 0.7f) + 20 * cos(moveTimer * 1.3f);
        if (y < 20) y = 20;
        if (y > 200) y = 200;
        spriteA.setPosition(x, y);
        spriteB.setPosition(x, y);
        // Columna de laser justo debajo del jefe
        if (specialLaserOn) {
            float laserX = x + bossWidth / 2 - laserTexture1.getSize().x / 2;
            float laserY = y + spriteA.getGlobalBounds().height;
            lasers.clear();
            sf::Sprite l1(laserTexture1);
            l1.setPosition(laserX, laserY);
            lasers.push_back(l1);
            float laser2Y = laserY + laserTexture1.getSize().y;
            while (laser2Y < 800) {
                sf::Sprite l2(laserTexture2);
                l2.setPosition(laserX, laser2Y);
                lasers.push_back(l2);
                laser2Y += laserTexture2.getSize().y;
            }
        } else {
            lasers.clear(); // Apagar columna
        }
        if (specialAttackTimer >= specialAttackDuration) {
            specialAttackActive = false;
            inNormalAttack = true;
            specialAttackElapsed = 0.0f;
            lasers.clear();
        }
        return; // No disparar normal durante ataque especial
    }
    // --- Disparo normal del jefe (como enemigos normales) ---
    laserTimer += deltaTime;
    if (laserTimer >= laserCooldown) {
        shoot();
        laserTimer = 0;
    }
    // Animación de los lasers activos
    for (auto& l : lasers) {
        float anim = fmod(animTime, 0.3f);
        if (anim < 0.15f) {
            l.setTexture(laserTexture1);
        } else {
            l.setTexture(laserTexture2);
        }
        l.move(0, 3.0f);
    }
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
