#include "game.hpp"
#include <iostream>


Game::Game()
{
    obstacles = CreateObstacles();
    aliens = CreateAliens();
    aliensDirection = 1;
    timeLastAlienFired = 0;
    timeLastSpawn = 0;
    mysteryShipSpawnInterval = GetRandomValue(10,20);
}

Game::~Game()
{
    Alien::UnloadImages();
}

void Game::Update()
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
    }

}
 // Alien lasers

 for(auto& laser: alienLasers)
    {
        if(CheckCollisionRecs(laser.GetRect(), spaceship.getRect()))
        {
            laser.active = false;
            std::cout << "Spaceship hit!" << std::endl;
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
            std::cout << "Alien hit Spaceship!" << std::endl;
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
}

std::vector<Obstacle> Game::CreateObstacles()
{
    int obstacleWidth = Obstacle::grid[0].size() * 3;
    float gap = (GetScreenWidth()  - (4 * obstacleWidth))/5;

    for(int i = 0; i < 4; ++i)
    {
        float offsetX = (i + 1) * gap + i * obstacleWidth;
        obstacles.push_back(Obstacle({offsetX, float(GetScreenHeight() -100)}));
    }
    return obstacles;
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
       if(alien.position.x + alien.alienImages[alien.type -1].width > GetScreenWidth())
        {
            aliensDirection = -1;
            MoveDownAliens(4);
        }

        if(alien.position.x < 0)
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
