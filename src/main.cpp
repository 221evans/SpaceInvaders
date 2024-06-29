#include <raylib.h>
#include "game.hpp"

// Define the main function
int main()
{
   Color darkGrey = { 29, 29, 27, 255 };
   int windowWidth = 750;
   int windowHeight = 750;
   InitWindow(windowWidth, windowHeight, "Space invaders");
   SetTargetFPS(60);
   
   Game game;
   
  
// Main game loop
   while (!WindowShouldClose())
   {
      game.HandleInput();
      game.Update();
      BeginDrawing();
      ClearBackground(darkGrey);
      game.Draw();
      EndDrawing();
   }

   CloseWindow();

}
  