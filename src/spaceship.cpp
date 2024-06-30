#include "spaceship.hpp"
int moveSpeed = 5;
Spaceship::Spaceship()
{
    image = LoadTexture("Graphics/spaceship.png");
    position.x = GetScreenWidth() / 2 - image.width / 2;
    position.y = GetScreenHeight() - image.height -200;
    lastFireTime = 0;
}

Spaceship::~Spaceship()
{
    UnloadTexture(image);
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

    }

}

Rectangle Spaceship::getRect()
{
    return Rectangle {position.x, position.y, float(image.width), float(image.height)};
}

void Spaceship::Reset()
{
    position.x = GetScreenWidth() - image.width / 2.0f;
    position.y = GetScreenHeight() - image.height - 100;
    lasers.clear();
}
