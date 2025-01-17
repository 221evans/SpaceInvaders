#include "spaceship.hpp"
int moveSpeed = 5;
Spaceship::Spaceship()
{
    image = LoadTexture("Graphics/spaceship.png");
    position.x = GetScreenWidth() / 2 - image.width / 2;
    position.y = GetScreenHeight() - image.height -120;
    lastFireTime = 0;
    laserSound = LoadSound("Sounds/laser.ogg");
}

Spaceship::~Spaceship()
{
    UnloadTexture(image);
    UnloadSound(laserSound);
}

void Spaceship::Draw()
{
    DrawTextureV(image, position, WHITE);
}

void Spaceship::MoveLeft()
{
    position.x -= moveSpeed;
    if(position.x < 25)
        position.x = 25;
    
}

void Spaceship::MoveRight()
{
    position.x += moveSpeed;
    if(position.x >= GetScreenWidth() - image.width -25)
        position.x = GetScreenWidth() - image.width -25;
}

void Spaceship::FireLaser()
{
   
    if(GetTime() - lastFireTime >= 0.35)
    {
        lastFireTime = GetTime();
        lasers.push_back(Laser({position.x + image.width/2 -2, position.y},-6));
        PlaySound(laserSound);
    }

}

Rectangle Spaceship::getRect()
{
    return Rectangle {position.x, position.y, float(image.width), float(image.height)};
}

void Spaceship::Reset()
{
    position.x = GetScreenWidth() / 2 - image.width / 2;
    position.y = GetScreenHeight() - image.height -120;
    lasers.clear();
}
