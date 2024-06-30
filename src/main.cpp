#include <raylib.h>
#include "game.hpp"
#include <string>



std:: string FormatWithLeadingZeros(int number, int width)
{
      std::string numberText = std::to_string(number);
      int leadingZeros = width - numberText.length();
      return numberText = std::string(leadingZeros, '0') + numberText;
}

// Define the main function
int main()
{
   Color darkGrey = { 29, 29, 27, 255 };
   Color yellow = { 243, 216, 63, 255 };
   int offset = 50;
   int windowWidth = 750;
   int windowHeight = 750;
   InitWindow(windowWidth + offset, windowHeight + 2 * offset, "Space Invaders");
   InitAudioDevice();

   Font font = LoadFontEx("Font/monogram.ttf", 64, 0, 0);
   Texture2D spaceshipImage = LoadTexture("Graphics/spaceship.png");

   SetTargetFPS(60);
   
   Game game;
   
  
// Main game loop
   while (!WindowShouldClose())
   {
      UpdateMusicStream(game.music);
      game.HandleInput();
      game.Update();
      BeginDrawing();
      ClearBackground(darkGrey);
      DrawRectangleRoundedLines({10,10,780,780}, 0.18f, 20, 2, yellow);
      DrawLineEx({25, 730}, {755,730}, 3, yellow);
      if(game.run)
      {
         DrawTextEx(font, "Level 01", {570,740}, 34, 2, yellow);
      }
      else
      {
          DrawTextEx(font, "Game Over", {250, 300}, 64, 2, RED);
          DrawTextEx(font, "Press Enter to Play Again", {150, 400}, 34, 2, yellow);
          DrawTextEx(font, "Game Over", {570,740}, 34, 2, yellow);
      }
      float x = 50;
      for(int i = 0; i < game.lives; i++)
      {
         DrawTextureV(spaceshipImage, {x, 745}, WHITE);
         x += 50;
      }
        
      DrawTextEx(font, "Score", {50, 15}, 34, 2, yellow);
      std::string scoreText = FormatWithLeadingZeros(game.score, 5);
      DrawTextEx(font, scoreText.c_str(), {50, 40}, 34, 2, yellow);

      DrawTextEx(font, "High-Score", {570, 15}, 34, 2, yellow);
      std::string highscoreText = FormatWithLeadingZeros(game.highscore, 5);
      DrawTextEx(font, highscoreText.c_str(), {655, 40}, 34, 2, yellow);
  
      game.Draw();
      EndDrawing();
      
   }

   CloseWindow();
   CloseAudioDevice();
}
  