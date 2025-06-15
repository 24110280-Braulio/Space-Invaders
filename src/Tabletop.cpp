#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <array>
#include <functional> // <-- Added for std::function
#include "../include/EnemyMovementManager.hpp"
#include "../include/EnemyHealthManager.hpp"
#include "../include/Enemy.hpp"
#include "../include/Barrier.hpp"
#include "../include/PlayerHealth.hpp"
#include "../include/EnemyShooter.hpp"
#include "../include/Jefe.hpp" // <-- Added include for Jefe
#include <iostream> // <-- Added for debug output

// --- Forward declarations for functions used in main ---
void drawBarriers(sf::RenderWindow& window, std::vector<Barrier>& barreras);
void drawPlayer(sf::RenderWindow& window, const sf::Sprite& player, const PlayerHealth& playerHP);

// --- Invincibility globals ---
bool playerInvincible = false;
sf::Clock playerInvincibleClock;

// --- Estructura para agrupar datos de enemigos ---
struct EnemyGroup {
    std::vector<Enemy>* enemies;
    std::vector<float>* velocities;
    std::vector<sf::Vector2f>* positions;
    std::vector<float>* directions;
    int frameWidth;
    int frameHeight;
    int numFrames;
    float frameTime;
    int yBase;
    std::function<std::vector<int>&()> getHP;
    std::function<void(int,int)> damage;
};

// --- BulletManager para balas del jugador ---
class BulletManager {
public:
    BulletManager(const sf::Texture& texture, float speed) : bulletTexture(texture), bulletSpeed(speed) {}
    void shoot(const sf::Vector2f& pos) {
        sf::Sprite bullet(bulletTexture);
        bullet.setPosition(pos);
        bullets.push_back(bullet);
    }
    void update() {
        for (auto& bullet : bullets) bullet.move(0, -bulletSpeed);
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [&](const sf::Sprite& b) {
            return b.getPosition().y + b.getTexture()->getSize().y < 0;
        }), bullets.end());
    }
    std::vector<sf::Sprite>& getBullets() { return bullets; }
    void draw(sf::RenderWindow& window) {
        for (const auto& bullet : bullets) window.draw(bullet);
    }
private:
    std::vector<sf::Sprite> bullets;
    const sf::Texture& bulletTexture;
    float bulletSpeed;
};

enum class GameState { Playing, GameOver };

int main()
{
    GameState gameState = GameState::Playing;
    sf::RenderWindow window(sf::VideoMode(800, 800), "Tabletop");

    // --- Fondo Espacial ---
    sf::Texture fondoTexture;
    if (!fondoTexture.loadFromFile("assets/images/Fondo Espacial.png"))
        return -1;
    sf::Sprite fondo(fondoTexture);
    fondo.setPosition(0, 550);
    // Ajustar tamaño si es necesario
    fondo.setScale(800.0f / fondoTexture.getSize().x, 250.0f / fondoTexture.getSize().y);

    // --- Barreras ---
    sf::Texture barreraTexture;
    if (!barreraTexture.loadFromFile("assets/images/Barrera.png"))
        return -1;
    sf::Texture lowBarreraTexture;
    if (!lowBarreraTexture.loadFromFile("assets/images/Lowbarrier.png"))
        return -1;
    const int barreraWidth = 64;
    const int numBarreras = 4;
    const int espacio = (800 - (numBarreras * barreraWidth)) / (numBarreras + 1);
    std::vector<Barrier> barreras;
    for (int i = 0; i < numBarreras; ++i) {
        sf::Vector2f pos(espacio + i * (barreraWidth + espacio), 490);
        barreras.emplace_back(barreraTexture, lowBarreraTexture, pos, 100); // Vida cambiada a 100
    }

    // --- Player ---
    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("assets/images/space_Player.png"))
        return -1;
    sf::Sprite player(playerTexture);
    player.setPosition(400, 700);
    PlayerHealth playerHP(100);
    // --- Sprites de vida del jugador ---
    sf::Texture hp100Texture, hp80Texture, hp60Texture, hp40Texture, hp20Texture, hp0Texture, timeoutTexture;
    hp100Texture.loadFromFile("assets/images/100 HP.png");
    hp80Texture.loadFromFile("assets/images/80 HP.png");
    hp60Texture.loadFromFile("assets/images/60 HP.png");
    hp40Texture.loadFromFile("assets/images/40 HP.png");
    hp20Texture.loadFromFile("assets/images/20 HP.png");
    hp0Texture.loadFromFile("assets/images/0 HP.png");
    timeoutTexture.loadFromFile("assets/images/Timeout.png");
    playerHP.setTextures(hp100Texture, hp80Texture, hp60Texture, hp40Texture, hp20Texture, hp0Texture, timeoutTexture);
    // --- Invencibilidad del jugador ---
    // (Ahora gestionada por PlayerHealth)

    // --- Shootout (Bala) ---
    sf::Texture bulletTexture;
    if (!bulletTexture.loadFromFile("assets/images/DisparoNave.png"))
        return -1;
    BulletManager bulletManager(bulletTexture, 1.5f);

    // --- Pea (Enemigos animados tipo Space Invaders) ---
    sf::Texture peaTexture;
    if (!peaTexture.loadFromFile("assets/images/PEA.png"))
        return -1;
    int peaFrameWidth = 44;
    int peaFrameHeight = 32;
    int peaNumFrames = 2;
    int peaCurrentFrame = 0;
    float peaFrameTime = 0.1f;
    float peaAnimTime = 0.0f;
    int numEnemies = 8;
    float peaVelocity = 40.0f; // Reducido 10.0f
    std::vector<Enemy> peas;
    std::vector<float> peaVelocities(numEnemies, peaVelocity);
    std::vector<sf::Vector2f> peaPositions;
    std::vector<float> peaDirections(numEnemies, 1.0f);
    for (int i = 0; i < numEnemies; ++i) {
        peas.emplace_back(peaTexture, peaFrameWidth, peaFrameHeight, peaNumFrames, peaFrameTime, 8, 60.0f + i * 90.0f, 60);
        peaPositions.push_back(sf::Vector2f(60.0f + i * 90.0f, 60));
    }
    sf::Clock peaClock;
    std::vector<int> peaHP(numEnemies, 20); // Salud individual para Pea
    std::vector<int> pebHP(numEnemies, 20); // Salud individual para Peb
    std::vector<int> pecHP(numEnemies, 20); // Salud individual para Pec

    // --- Peb (Enemigos animados tipo Peb) ---
    sf::Texture pebTexture;
    if (!pebTexture.loadFromFile("assets/images/PEB.png"))
        return -1;
    int pebFrameWidth = 48;
    int pebFrameHeight = 32;
    int pebNumFrames = 2;
    int pebCurrentFrame = 0;
    float pebFrameTime = .1f;
    float pebAnimTime = 0.0f;
    std::vector<Enemy> pebs;
    std::vector<float> pebVelocities(numEnemies, peaVelocity);
    std::vector<sf::Vector2f> pebPositions;
    std::vector<float> pebDirections(numEnemies, 1.0f);
    for (int i = 0; i < numEnemies; ++i) {
        pebs.emplace_back(pebTexture, pebFrameWidth, pebFrameHeight, pebNumFrames, pebFrameTime, 8, 60.0f + i * 90.0f, 120);
        pebPositions.push_back(sf::Vector2f(60.0f + i * 90.0f, 120));
    }

    // --- Pec (Enemigos animados tipo Pec) ---
    sf::Texture pecTexture;
    if (!pecTexture.loadFromFile("assets/images/PEC.png"))
        return -1;
    int pecFrameWidth = 32;
    int pecFrameHeight = 32;
    int pecNumFrames = 2;
    int pecCurrentFrame = 0;
    float pecFrameTime = .1f;
    float pecAnimTime = 0.0f;
    std::vector<Enemy> pecs;
    std::vector<float> pecVelocities(numEnemies, peaVelocity);
    std::vector<sf::Vector2f> pecPositions;
    std::vector<float> pecDirections(numEnemies, 1.0f);
    for (int i = 0; i < numEnemies; ++i) {
        pecs.emplace_back(pecTexture, pecFrameWidth, pecFrameHeight, pecNumFrames, pecFrameTime, 8, 60.0f + i * 90.0f, 0);
        pecPositions.push_back(sf::Vector2f(60.0f + i * 90.0f, 0));
    }
    // --- Managers ---
    EnemyMovementManager movementManager;
    movementManager.addEnemyGroup(&peaPositions, &peaVelocities, &peaDirections, peaFrameWidth, 32, 768);
    movementManager.addEnemyGroup(&pebPositions, &pebVelocities, &pebDirections, pebFrameWidth, 32, 768);
    movementManager.addEnemyGroup(&pecPositions, &pecVelocities, &pecDirections, pecFrameWidth, 32, 768);
    EnemyHealthManager healthManager;
    healthManager.setHealths(std::vector<int>(numEnemies, 20), std::vector<int>(numEnemies, 20), std::vector<int>(numEnemies, 20));

    // --- Eshoot (Disparo enemigo animado) ---
    std::array<sf::Texture, 4> enemyBulletTextures;
    for (int i = 0; i < 4; ++i) {
        std::string filename = "assets/images/Eshoot(" + std::to_string(i + 1) + ").png";
        if (!enemyBulletTextures[i].loadFromFile(filename)) {
            return -1;
        }
    }
    EnemyShooter enemyShooter(enemyBulletTextures, 0.3f, 0.1f);
    float shootInterval = 0.5f;
    int shootChance = 6;
    int totalEnemiesDefeated = 0;
    int lastShootChanceUpdate = 0;
    sf::Clock shootClock;
    std::vector<sf::Clock> enemyShootClocks(numEnemies); // Delay individual de disparo

    // --- Cargar fuente para Game Over ---
    sf::Font gameOverFont;
    if (!gameOverFont.loadFromFile("assets/fonts/AngelicWar.ttf")) {
        return -1;
    }
    sf::Text gameOverText;
    gameOverText.setFont(gameOverFont);
    gameOverText.setString("GAME OVER");
    gameOverText.setCharacterSize(90);
    gameOverText.setFillColor(sf::Color::Red);
    // Centrar el texto
    sf::FloatRect textRect = gameOverText.getLocalBounds();
    gameOverText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    gameOverText.setPosition(400, 400);

    Boss boss({380, 60}); // Posición superior central
    bool bossAppeared = false;
    sf::Clock bossClock; // Reloj independiente para el boss
    sf::Clock bossDelayClock; // Reloj para el retardo de aparición
    bool bossReadyToAppear = false;

    // --- Texto de aparición del boss ---
    sf::Text bossText;
    bossText.setFont(gameOverFont);
    bossText.setString("BOSS!");
    bossText.setCharacterSize(80);
    bossText.setFillColor(sf::Color::Yellow);
    bossText.setStyle(sf::Text::Bold);
    bossText.setOutlineColor(sf::Color::Red);
    bossText.setOutlineThickness(4);
    bossText.setOrigin(bossText.getLocalBounds().width / 2, bossText.getLocalBounds().height / 2);
    bossText.setPosition(400, 200);
    bool showBossText = false;

    sf::Font bossFont;
    if (!bossFont.loadFromFile("assets/fonts/AngelicWar.ttf")) {
        // Si falla, usar la fuente por defecto
    }
    sf::Text bossTimerText;
    bossTimerText.setFont(bossFont);
    bossTimerText.setCharacterSize(60);
    bossTimerText.setFillColor(sf::Color::Yellow);
    bossTimerText.setStyle(sf::Text::Bold);
    bossTimerText.setPosition(200, 350);
    bool showBossTimer = false;
    float bossCountdown = 0;

    bool allEnemiesDefeated = false;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        if (gameState == GameState::GameOver) {
            window.clear();
            window.draw(fondo);
            window.draw(gameOverText);
            window.display();
            continue;
        }

        // Movimiento del jugador
        float velocidad = 0.5f;
        sf::Vector2f pos = player.getPosition();
        sf::Vector2u size = playerTexture.getSize();
        float minX = 0;
        float minY = 582;
        float maxX = 800 - size.x;
        float maxY = 800 - size.y;
        if (playerHP.isAlive()) {
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
        }

        // Disparo (tecla Space)
        static bool canShoot = true;
        if (playerHP.isAlive() && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            if (canShoot) {
                bulletManager.shoot(sf::Vector2f(player.getPosition().x + playerTexture.getSize().x / 2 - bulletTexture.getSize().x / 2, player.getPosition().y - bulletTexture.getSize().y));
                canShoot = false;
            }
        } else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            canShoot = true;
        }
        bulletManager.update();

        // --- Movimiento automático y animación de enemigos ---
        float peaDeltaTime = peaClock.restart().asSeconds();
        movementManager.update(peaDeltaTime);
        // Restricción: si hay al menos una barrera activa, los enemigos no pueden bajar de y=500
        bool barreraActiva = false;
        for (const auto& barrera : barreras) {
            if (barrera.isAlive()) {
                barreraActiva = true;
                break;
            }
        }
        float limiteY = barreraActiva ? 450.0f : 800.0f;
        for (int i = 0; i < numEnemies; ++i) {
            // Condición: Si cualquier enemigo llega a Y >= 800, daña al jugador y elimina al enemigo
            if (peaPositions[i].y >= 800 && healthManager.getPeaHP()[i] > 0) {
                if (!playerInvincible && playerHP.isAlive()) {
                    playerHP.takeDamage(100);
                    playerInvincible = true;
                    playerInvincibleClock.restart();
                }
                healthManager.damagePea(i, healthManager.getPeaHP()[i]); // Eliminar enemigo
            }
            if (pebPositions[i].y >= 800 && healthManager.getPebHP()[i] > 0) {
                if (!playerInvincible && playerHP.isAlive()) {
                    playerHP.takeDamage(100);
                    playerInvincible = true;
                    playerInvincibleClock.restart();
                }
                healthManager.damagePeb(i, healthManager.getPebHP()[i]);
            }
            if (pecPositions[i].y >= 800 && healthManager.getPecHP()[i] > 0) {
                if (!playerInvincible && playerHP.isAlive()) {
                    playerHP.takeDamage(100);
                    playerInvincible = true;
                    playerInvincibleClock.restart();
                }
                healthManager.damagePec(i, healthManager.getPecHP()[i]);
            }
            if (peaPositions[i].y > limiteY) peaPositions[i].y = limiteY;
            peas[i].setPosition(peaPositions[i]);
            peas[i].update(peaDeltaTime);
        }
        for (int i = 0; i < numEnemies; ++i) {
            if (pebPositions[i].y > limiteY) pebPositions[i].y = limiteY;
            pebs[i].setPosition(pebPositions[i]);
            pebs[i].update(peaDeltaTime);
        }
        for (int i = 0; i < numEnemies; ++i) {
            if (pecPositions[i].y > limiteY) pecPositions[i].y = limiteY;
            pecs[i].setPosition(pecPositions[i]);
            pecs[i].update(peaDeltaTime);
        }

        // --- Restricciones de altura entre enemigos ---
        auto ajustarAltura = [](std::vector<sf::Vector2f>& arriba, std::vector<int>& arribaHP,
                                const std::vector<sf::Vector2f>& abajo, const std::vector<int>& abajoHP,
                                float distanciaX, float distanciaY) {
            int n = arriba.size();
            int m = abajo.size();
            for (int i = 0; i < n; ++i) {
                if (arribaHP[i] <= 0) continue;
                for (int j = 0; j < m; ++j) {
                    if (abajoHP[j] > 0 && std::abs(arriba[i].x - abajo[j].x) < distanciaX) {
                        if (arriba[i].y > abajo[j].y - distanciaY) {
                            arriba[i].y = abajo[j].y - distanciaY;
                        }
                    }
                }
            }
        };

        ajustarAltura(peaPositions, healthManager.getPeaHP(), pebPositions, healthManager.getPebHP(), 30, 30); // PEA sobre PEB
        ajustarAltura(pecPositions, healthManager.getPecHP(), peaPositions, healthManager.getPeaHP(), 30, 30); // PEC sobre PEA

        // --- Restricción: Si hay al menos un PEB, todos los PEA deben estar al menos 40px arriba del PEB más cercano en X ---
        bool hayPeb = std::any_of(healthManager.getPebHP().begin(), healthManager.getPebHP().end(), [](int hp){ return hp > 0; });
        if (hayPeb) {
            for (int i = 0; i < numEnemies; ++i) {
                if (healthManager.getPeaHP()[i] <= 0) continue;
                float minDeltaX = 1e6;
                int idxPebCercano = -1;
                for (int j = 0; j < numEnemies; ++j) {
                    if (healthManager.getPebHP()[j] > 0) {
                        float deltaX = std::abs(peaPositions[i].x - pebPositions[j].x);
                        if (deltaX < minDeltaX) {
                            minDeltaX = deltaX;
                            idxPebCercano = j;
                        }
                    }
                }
                if (idxPebCercano != -1 && minDeltaX < 30) {
                    if (peaPositions[i].y > pebPositions[idxPebCercano].y - 40) {
                        peaPositions[i].y = pebPositions[idxPebCercano].y - 40;
                    }
                }
            }
        }
        // --- Lo mismo para PEC respecto a PEA ---
        bool hayPea = std::any_of(healthManager.getPeaHP().begin(), healthManager.getPeaHP().end(), [](int hp){ return hp > 0; });
        if (hayPea) {
            for (int i = 0; i < numEnemies; ++i) {
                if (healthManager.getPecHP()[i] <= 0) continue;
                float minDeltaX = 1e6;
                int idxPeaCercano = -1;
                for (int j = 0; j < numEnemies; ++j) {
                    if (healthManager.getPeaHP()[j] > 0) {
                        float deltaX = std::abs(pecPositions[i].x - peaPositions[j].x);
                        if (deltaX < minDeltaX) {
                            minDeltaX = deltaX;
                            idxPeaCercano = j;
                        }
                    }
                }
                if (idxPeaCercano != -1 && minDeltaX < 30) {
                    if (pecPositions[i].y > peaPositions[idxPeaCercano].y - 16) {
                        pecPositions[i].y = peaPositions[idxPeaCercano].y - 16;
                    }
                }
            }
        }

        // --- Colisión entre balas y enemigos ---
        int enemiesDefeatedThisFrame = 0;
        for (int i = 0; i < numEnemies; ++i) {
            // Pea
            if (healthManager.getPeaHP()[i] > 0) {
                auto it = std::remove_if(bulletManager.getBullets().begin(), bulletManager.getBullets().end(), [&](const sf::Sprite& bullet) {
                    if (bullet.getGlobalBounds().intersects(peas[i].getGlobalBounds())) {
                        healthManager.damagePea(i, 20);
                        if (healthManager.getPeaHP()[i] <= 0) enemiesDefeatedThisFrame++;
                        return true;
                    }
                    return false;
                });
                bulletManager.getBullets().erase(it, bulletManager.getBullets().end());
            }
            // Peb
            if (healthManager.getPebHP()[i] > 0) {
                auto it = std::remove_if(bulletManager.getBullets().begin(), bulletManager.getBullets().end(), [&](const sf::Sprite& bullet) {
                    if (bullet.getGlobalBounds().intersects(pebs[i].getGlobalBounds())) {
                        healthManager.damagePeb(i, 20);
                        if (healthManager.getPebHP()[i] <= 0) enemiesDefeatedThisFrame++;
                        return true;
                    }
                    return false;
                });
                bulletManager.getBullets().erase(it, bulletManager.getBullets().end());
            }
            // Pec
            if (healthManager.getPecHP()[i] > 0) {
                auto it = std::remove_if(bulletManager.getBullets().begin(), bulletManager.getBullets().end(), [&](const sf::Sprite& bullet) {
                    if (bullet.getGlobalBounds().intersects(pecs[i].getGlobalBounds())) {
                        healthManager.damagePec(i, 20);
                        if (healthManager.getPecHP()[i] <= 0) enemiesDefeatedThisFrame++;
                        return true;
                    }
                    return false;
                });
                bulletManager.getBullets().erase(it, bulletManager.getBullets().end());
            }
        }
        totalEnemiesDefeated += enemiesDefeatedThisFrame;
        // Cada 2 enemigos derrotados, aumenta shootChance en 1
        if ((totalEnemiesDefeated / 2) > lastShootChanceUpdate) {
            shootChance++;
            lastShootChanceUpdate = totalEnemiesDefeated / 2;
        }
        // --- Disparo enemigo automático (Pea, Peb y Pec disparan) ---
        for (int i = 0; i < numEnemies; ++i) {
            if (healthManager.getPeaHP()[i] > 0) {
                if (enemyShootClocks[i].getElapsedTime().asSeconds() > shootInterval) {
                    enemyShooter.tryShoot(peaPositions[i], peaFrameWidth, peaFrameHeight, shootChance);
                    enemyShootClocks[i].restart();
                }
            }
            if (healthManager.getPebHP()[i] > 0) {
                if (enemyShootClocks[i].getElapsedTime().asSeconds() > shootInterval) {
                    enemyShooter.tryShoot(pebPositions[i], pebFrameWidth, pebFrameHeight, shootChance);
                    enemyShootClocks[i].restart();
                }
            }
            if (healthManager.getPecHP()[i] > 0) {
                if (enemyShootClocks[i].getElapsedTime().asSeconds() > shootInterval) {
                    enemyShooter.tryShoot(pecPositions[i], pecFrameWidth, pecFrameHeight, shootChance);
                    enemyShootClocks[i].restart();
                }
            }
        }
        enemyShooter.updateBullets();
        // --- Colisión entre balas enemigas y barreras ---
        if (!bossAppeared) {
            auto& peaBullets = enemyShooter.getBullets();
            peaBullets.erase(std::remove_if(peaBullets.begin(), peaBullets.end(), [&](const EnemyBullet& eb) {
                for (auto& barrera : barreras) {
                    if (barrera.isAlive() && eb.sprite.getGlobalBounds().intersects(barrera.getSprite().getGlobalBounds())) {
                        barrera.takeDamage(20);
                        return true;
                    }
                }
                if (eb.sprite.getPosition().y > 800) return true;
                if (eb.sprite.getGlobalBounds().intersects(player.getGlobalBounds())) {
                    playerHP.takeDamage(20);
                    return true;
                }
                return false;
            }), peaBullets.end());
        } else {
            // Si el boss está en pantalla, eliminar todas las balas de enemigos normales
            enemyShooter.getBullets().clear();
        }
        // --- Lógica de daño y movimiento de lasers del boss ---
        if (bossAppeared && boss.isActive()) {
            auto& bossLasers = boss.getLasers();
            for (auto& laser : bossLasers) {
                if (playerHP.isAlive() && laser.getGlobalBounds().intersects(player.getGlobalBounds())) {
                    playerHP.takeDamage(40); // Daño mayor por laser del boss
                    // Opcional: puedes hacer que el láser desaparezca tras impactar
                    laser.setPosition(-100, -100); // Lo mueve fuera de pantalla
                }
            }
        }

        // --- Daño por enemigos que llegan al fondo ---
        for (int i = 0; i < numEnemies; ++i) {
            if (peaPositions[i].y >= 800 && healthManager.getPeaHP()[i] > 0) {
                playerHP.takeDamage(100);
                healthManager.damagePea(i, healthManager.getPeaHP()[i]);
            }
            if (pebPositions[i].y >= 800 && healthManager.getPebHP()[i] > 0) {
                playerHP.takeDamage(100);
                healthManager.damagePeb(i, healthManager.getPebHP()[i]);
            }
            if (pecPositions[i].y >= 800 && healthManager.getPecHP()[i] > 0) {
                playerHP.takeDamage(100);
                healthManager.damagePec(i, healthManager.getPecHP()[i]);
            }
        }

        // --- Actualizar estado de invencibilidad y sprite de vida ---
        playerHP.update();

        if (!playerHP.isAlive()) {
            gameState = GameState::GameOver;
        }

        // --- Lógica de aparición del boss con retardo y depuración ---
        allEnemiesDefeated = true;
        for (int i = 0; i < numEnemies; ++i) {
            if (healthManager.getPeaHP()[i] > 0 || healthManager.getPebHP()[i] > 0 || healthManager.getPecHP()[i] > 0) {
                allEnemiesDefeated = false;
                break;
            }
        }
        if (allEnemiesDefeated && !bossReadyToAppear) {
            std::cout << "[DEBUG] Todos los enemigos eliminados. Iniciando retardo para boss..." << std::endl;
            bossDelayClock.restart();
            bossReadyToAppear = true;
        }
        if (bossReadyToAppear && !bossAppeared && bossDelayClock.getElapsedTime().asSeconds() >= 5.0f) {
            std::cout << "[DEBUG] Boss aparece!" << std::endl;
            boss.appear();
            bossAppeared = true;
            bossClock.restart();
        }
        // --- DEPURACIÓN: Forzar aparición del boss tras 10 segundos de juego ---
        // static sf::Clock debugBossClock;
        // if (!bossAppeared && debugBossClock.getElapsedTime().asSeconds() > 10.0f) {
        //     std::cout << "[DEBUG] Forzando aparición del boss tras 10 segundos." << std::endl;
        //     boss.appear();
        //     bossAppeared = true;
        //     bossClock.restart();
        // }
        // --- Cronómetro visual para aparición del boss ---
        // Solo mostrar el cronómetro cuando el último enemigo es eliminado
        allEnemiesDefeated = true;
        for (int i = 0; i < numEnemies; ++i) {
            if (healthManager.getPeaHP()[i] > 0 || healthManager.getPebHP()[i] > 0 || healthManager.getPecHP()[i] > 0) {
                allEnemiesDefeated = false;
                break;
            }
        }
        if (allEnemiesDefeated && !bossReadyToAppear && !bossAppeared) {
            bossDelayClock.restart();
            bossReadyToAppear = true;
            showBossTimer = true;
        }
        if (bossReadyToAppear && !bossAppeared) {
            bossCountdown = 5.0f - bossDelayClock.getElapsedTime().asSeconds();
            if (bossCountdown < 0) bossCountdown = 0;
            bossTimerText.setString("BOSS en: " + std::to_string((int)bossCountdown) + "s");
            showBossTimer = true;
            if (bossDelayClock.getElapsedTime().asSeconds() >= 5.0f) {
                boss.appear();
                bossAppeared = true;
                bossClock.restart();
                showBossTimer = false;
            }
        } else {
            showBossTimer = false;
        }
        if (bossAppeared) {
            float bossDeltaTime = bossClock.restart().asSeconds();
            boss.update(bossDeltaTime, player.getPosition());
        }

        window.clear();
        window.draw(fondo);
        window.draw(playerHP.getSprite());
        drawBarriers(window, barreras);
        drawPlayer(window, player, playerHP);
        bulletManager.draw(window);
        enemyShooter.drawBullets(window);
        if (!bossAppeared) {
            for (int i = 0; i < numEnemies; ++i)
                if (healthManager.getPeaHP()[i] > 0)
                    peas[i].draw(window);
            for (int i = 0; i < numEnemies; ++i)
                if (healthManager.getPebHP()[i] > 0)
                    pebs[i].draw(window);
            for (int i = 0; i < numEnemies; ++i)
                if (healthManager.getPecHP()[i] > 0)
                    pecs[i].draw(window);
        }
        if (showBossTimer) {
            window.draw(bossTimerText);
        }
        if (bossAppeared && boss.isActive()) {
            boss.draw(window);
        }
        if (showBossText) {
            window.draw(bossText);
            // El mensaje solo se muestra 2 segundos
            static sf::Clock bossTextClock;
            if (bossTextClock.getElapsedTime().asSeconds() > 2.0f) {
                showBossText = false;
                bossTextClock.restart();
            }
        }
        window.display();
    }

    return 0;
}

// --- Función para manejar la entrada del jugador ---
void handlePlayerInput(sf::Sprite& player, float velocidad, const sf::Texture& playerTexture, PlayerHealth& playerHP) {
    sf::Vector2f pos = player.getPosition();
    sf::Vector2u size = playerTexture.getSize();
    float minX = 0;
    float minY = 582;
    float maxX = 800 - size.x;
    float maxY = 800 - size.y;
    if (playerHP.isAlive()) {
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
    }
}

// --- Función para renderizar enemigos ---
void drawEnemies(sf::RenderWindow& window, const EnemyGroup& group, const std::vector<int>& hp) {
    for (int i = 0; i < hp.size(); ++i)
        if (hp[i] > 0)
            (*group.enemies)[i].draw(window);
}

// --- Función para renderizar barreras ---
void drawBarriers(sf::RenderWindow& window, std::vector<Barrier>& barreras) {
    for (auto& barrera : barreras)
        if (barrera.isAlive())
            window.draw(barrera.getSprite());
}

// --- Función para renderizar jugador ---
void drawPlayer(sf::RenderWindow& window, const sf::Sprite& player, const PlayerHealth& playerHP) {
    if (playerHP.isAlive())
        window.draw(player);
}