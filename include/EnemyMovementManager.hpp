#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class EnemyMovementManager {
public:
    struct EnemyMovement {
        std::vector<sf::Vector2f>* positions;
        std::vector<float>* velocities;
        std::vector<float>* directions;
        int frameWidth;
        float bounceLeftX;
        float bounceRightX;
    };

    void addEnemyGroup(std::vector<sf::Vector2f>* positions, std::vector<float>* velocities, std::vector<float>* directions, int frameWidth, float bounceLeftX, float bounceRightX) {
        groups.push_back({positions, velocities, directions, frameWidth, bounceLeftX, bounceRightX});
    }

    void update(float deltaTime) {
        for (auto& group : groups) {
            int numEnemies = group.positions->size();
            bool bounceLeft = (*group.positions)[0].x < group.bounceLeftX;
            bool bounceRight = (*group.positions)[numEnemies - 1].x > group.bounceRightX - group.frameWidth;
            if (bounceLeft) {
                for (int i = 0; i < numEnemies; ++i) {
                    (*group.positions)[i].x = group.bounceLeftX + (i * ((*group.positions)[1].x - (*group.positions)[0].x));
                    (*group.positions)[i].y += 32;
                    (*group.directions)[i] = 1.0f;
                }
            } else if (bounceRight) {
                for (int i = 0; i < numEnemies; ++i) {
                    (*group.positions)[i].x = group.bounceRightX - group.frameWidth - (numEnemies - 1 - i) * ((*group.positions)[1].x - (*group.positions)[0].x);
                    (*group.positions)[i].y += 32;
                    (*group.directions)[i] = -1.0f;
                }
            } else {
                for (int i = 0; i < numEnemies; ++i) {
                    (*group.positions)[i].x += (*group.velocities)[i] * (*group.directions)[i] * deltaTime;
                }
            }
            for (int i = 0; i < numEnemies; ++i) {
                if ((*group.positions)[i].y > 550) {
                    (*group.velocities)[i] = 100.0f;
                }
            }
        }
    }
private:
    std::vector<EnemyMovement> groups;
};
