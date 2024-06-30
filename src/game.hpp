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
        int lives;
        int score;
        int highscore;
        Music music;
        Sound explosionSound;
    private:
            void DeleteInactiveLasers();
            void MoveAliens();
            void MoveDownAliens(int distance);
            void AlienShootLaser();
            void GameOver();
            void Reset();
            void InitGame();
            void CheckForHighScore();
            void SaveHighScoreToFile(int highscore);
            int LoadHighScoreFromFile();
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
            
};