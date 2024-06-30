#include "game.hpp"
#include <iostream>
#include <fstream>

Game::Game()
{
    music = LoadMusicStream("Sounds/music.ogg");
    PlayMusicStream(music);
    explosionSound = LoadSound("Sounds/explosion.ogg");
    InitGame();
}

Game::~Game()
{
    Alien::UnloadImages();
    UnloadMusicStream(music);
    UnloadSound(explosionSound);
}

void Game::Update()
    {
        // check if game is running
        if(run)
    {
        MoveAliens();
        AlienShootLaser();

        double currentTime = GetTime();
        if(currentTime - timeLastSpawn > mysteryShipSpawnInterval)
        {
            mysteryShip.Spawn();
            timeLastSpawn = GetTime();
            mysteryShipSpawnInterval = GetRandomValue(10,20);
        }

        for(auto &laser : spaceship.lasers)
        {
            laser.Update();
        }

        DeleteInactiveLasers();
    
        for(auto &laser : alienLasers)
        {
            laser.Update();
        }
        
        mysteryShip.Update();

        CheckforCollisions();
    }
    else
    {
        if (IsKeyDown(KEY_ENTER))
        {
            Reset();
            InitGame();
        }
    }
 }

void Game::Draw()
{
    spaceship.Draw();
    // draw obstacles  
    for(auto &laser : spaceship.lasers)
    {
        laser.Draw();
    }
    // draw obstacles
    for(auto &obstacle : obstacles)
    {
        obstacle.Draw();
    }
    // draw aliens
    for(auto &alien : aliens)
    {
        alien.Draw();
    }
    // draw alien lasers
    for(auto &laser : alienLasers)
    {
        laser.Draw();
    }
    mysteryShip.Draw();
}

void Game::HandleInput()
{
    if(run)
    {
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
        {
            spaceship.MoveLeft();
        }
        if (IsKeyDown(KEY_RIGHT)|| IsKeyDown(KEY_D))
        {
            spaceship.MoveRight();
        }
        if (IsKeyPressed(KEY_SPACE))
        {
            spaceship.FireLaser();
        }
    }
}



std::vector<Alien> Game::CreateAliens()
{
    std::vector<Alien> aliens;
    for(int row = 0; row < 5; row++)
    {
        // create 11 aliens per row
        for(int column = 0; column < 11; column++)
        {

            int alienType;
            // determine alien type based on row
            if(row == 0)
            {
                alienType = 3;
            }
            else if(row > 0 && row < 3)
            {
                alienType = 2;
            }
            else
            {
                alienType = 1;
            }

            float x = 75 + column * 55;
            float y = 110 + row * 55;
            aliens.push_back(Alien(alienType,{x,y}));
        }
    }
    return aliens;
}

void Game::MoveAliens()
{
    for(auto &alien : aliens)
    {
       if(alien.position.x + alien.alienImages[alien.type -1].width > GetScreenWidth() -25)
        {
            aliensDirection = -1;
            MoveDownAliens(4);
        }

        if(alien.position.x < 25)
        {
            aliensDirection = 1;
            MoveDownAliens(4);
        }

        alien.Update(aliensDirection);

    }
}

void Game::MoveDownAliens(int distance)
{
    for(auto &alien : aliens)
    {
        alien.position.y += distance;
    }
}

void Game::AlienShootLaser()
{
    double currentTime = GetTime();
    if(currentTime - timeLastAlienFired > alienLaserInterval && !aliens.empty())
    {
        int randomIndex = GetRandomValue(0,aliens.size() - 1);
        Alien &alien = aliens[randomIndex];
        alienLasers.push_back(Laser({alien.position.x + alien.alienImages[alien.type - 1].width/2,
        alien.position.y + alien.alienImages[alien.type - 1].height}, 6));

        timeLastAlienFired = GetTime();
    }
}

void Game::GameOver()
{
    std::cout << "Game Over!" << std::endl;
    run = false;
}

void Game::CheckforCollisions()
{
    // Spaceship lasers
    for(auto& laser: spaceship.lasers)
    {
        auto it = aliens.begin();
        while(it != aliens.end())
        {
            if(CheckCollisionRecs(it -> GetRect(), laser.GetRect()))
            {
                PlaySound(explosionSound);
                if(it -> type == 1)
                {
                    score += 100;
                }
                else if(it -> type == 2)
                {
                    score += 200;
                }
                else
                {
                    score += 300;
                     
                }
                CheckForHighScore();
                it = aliens.erase(it);
                laser.active = false;
            }
            else
            {
                ++it;
            }
        }

    for (auto& obstacle: obstacles)
    {
        auto it = obstacle.blocks.begin();
        while(it != obstacle.blocks.end())
        {
            if(CheckCollisionRecs(it -> GetRect(), laser.GetRect()))
            {
                    it = obstacle.blocks.erase(it);
                    laser.active = false;
            }
            else
            {
                    ++it;
            }
        }
    }

     if(CheckCollisionRecs(mysteryShip.GetRect(), laser.GetRect()))
    {
        mysteryShip.alive = false;
        laser.active = false;
        score += 500;
        CheckForHighScore();
        PlaySound(explosionSound);
    }

}
 // Alien lasers 

 for(auto& laser: alienLasers)
    {
        if(CheckCollisionRecs(laser.GetRect(), spaceship.getRect()))
        {
            laser.active = false;
            lives--;
            if(lives == 0)
            {
                GameOver();
            }
        }

        for (auto& obstacle: obstacles)
        {
            auto it = obstacle.blocks.begin();
            while(it != obstacle.blocks.end())
            {
                if(CheckCollisionRecs(it -> GetRect(), laser.GetRect()))
                {
                        it = obstacle.blocks.erase(it);
                        laser.active = false;
                }
                else
                {
                        ++it;
                }
            }
        }
    }

    // Alien collisions with obstacles
    for(auto& alien: aliens)
    {
        for (auto& obstacle: obstacles)
        {
            auto it = obstacle.blocks.begin();
            while(it != obstacle.blocks.end())
            {
                if(CheckCollisionRecs(it -> GetRect(), alien.GetRect()))
                {
                        it = obstacle.blocks.erase(it);
                }
                else
                {
                        ++it;
                }
            }
        }
        if(CheckCollisionRecs(alien.GetRect(), spaceship.getRect()))
        {
            GameOver();
        }
    }
    
}

void Game::DeleteInactiveLasers()
{
    // delete inactive spaceship lasers
   for(auto it = spaceship.lasers.begin(); it != spaceship.lasers.end();)
   {
       if(!it->active)
       {
           it = spaceship.lasers.erase(it);
       }
       else
       {
           ++it;
       }
        // delete inactive alien lasers
      
   }
    for(auto it = alienLasers.begin(); it != alienLasers.end();)
       {
           if(!it->active)
           {
               it = alienLasers.erase(it);
           }
           else
           {
               ++it;
           }
       }
}

void Game::InitGame()
{
    obstacles = CreateObstacles();
    aliens = CreateAliens();
    aliensDirection = 1;
    timeLastAlienFired = 0;
    timeLastSpawn = 0;
    mysteryShipSpawnInterval = GetRandomValue(10,20);
    lives = 3;
    run = true;
    score = 0;
    highscore = LoadHighScoreFromFile();
}
void Game::CheckForHighScore()
{
    if(score > highscore)
    {
        highscore = score;
        SaveHighScoreToFile(highscore);
    }
}
void Game::SaveHighScoreToFile(int highscore)
{
    std::ofstream highscoreFile("highscore.txt");
    if(highscoreFile.is_open())
    {
        highscoreFile << highscore;
        highscoreFile.close();
    }
    else
    {
        std::cerr << "Unable to save highscore to file" << std::endl;
    }
}

int Game::LoadHighScoreFromFile()
{
    int loadedHighscore = 0;
    std::ifstream highscoreFile("highscore.txt");
    if(highscoreFile.is_open())
    {
        highscoreFile >> loadedHighscore;
        highscoreFile.close();
    }
    else
    {
        std::cerr << "Unable to load highscore from file" << std::endl;
    }
    return loadedHighscore;
}
// Reset the game
void Game::Reset()
{
    spaceship.Reset();
    aliens.clear();
    alienLasers.clear();
    obstacles.clear();
}

std::vector<Obstacle> Game::CreateObstacles()
{
    int obstacleWidth = Obstacle::grid[0].size() * 3;
    float gap = (GetScreenWidth()  - (4 * obstacleWidth))/5;

    for(int i = 0; i < 4; ++i)
    {
        float offsetX = (i + 1) * gap + i * obstacleWidth;
        obstacles.push_back(Obstacle({offsetX, float(GetScreenHeight() -300)}));
    }
    return obstacles;
}

