#include <raylib.h>
#include "game.hpp"

// Define the main function
int main()
{
   Color darkGrey = { 29, 29, 27, 255 };
   Color yellow = { 243, 216, 63, 255 };
   int offset = 50;
   int windowWidth = 750;
   int windowHeight = 750;
   InitWindow(windowWidth + offset, windowHeight + 2 * offset, "Space Invaders");
   SetTargetFPS(60);
   
   Game game;
   
  
// Main game loop
   while (!WindowShouldClose())
   {
      game.HandleInput();
      game.Update();
      BeginDrawing();
      ClearBackground(darkGrey);
      DrawRectangleRoundedLines({10,10,780,780}, 0.18f, 20, 2, yellow);
      DrawLineEx({25, 730}, {755,730}, 3, yellow);
      game.Draw();
      EndDrawing();
   }

   CloseWindow();

}
  