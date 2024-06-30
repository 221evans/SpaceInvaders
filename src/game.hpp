#pragma once
#include "alien.hpp"
#include "spaceship.hpp"
#include "obstacle.hpp"
#include "mysteryship.hpp"

class Game {
    public:
        Game();
        ~Game();
        void Update();
        void Draw();
        void HandleInput();
        void CheckforCollisions();
        bool run;
    private:
            void DeleteInactiveLasers();
            void MoveAliens();
            void MoveDownAliens(int distance);
            void AlienShootLaser();
            void GameOver();
            void Reset();
            void InitGame();
            std::vector<Obstacle> CreateObstacles();
            std::vector<Alien> CreateAliens();
            Spaceship spaceship;
            MysteryShip mysteryShip;
            std::vector<Obstacle> obstacles;
            std::vector<Alien> aliens;
            int aliensDirection;
            std::vector<Laser> alienLasers;
            constexpr static float alienLaserInterval = 0.35;
            float timeLastAlienFired;
            float mysteryShipSpawnInterval;
            float timeLastSpawn;
            int lives;
};