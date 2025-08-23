#include "raylib.h"
#include <stdio.h>

#define tileSize 32
#define numTiles 11
#define middleX (500 - 8 * 32) / 2 
#define middleY (700 - 8 * 32) / 2 

void drawBoard(Texture2D atlas, Rectangle tiles[], Rectangle source){

  for(int x = 0; x < 8; x++){
    for(int y = 0; y < 8; y++){
      DrawTextureRec(atlas, source, (Vector2){x * tileSize + middleX, y * tileSize + middleY}, WHITE);
    }
  }
}

int main(void) {

  InitWindow(500, 700, "Minesweeper");
  Texture2D atlas = LoadTexture("tiles.jpg");
  Rectangle tiles[numTiles];
    
  //Vector2 clickPosition = {0, 0};
  
  for(int i = 0; i < numTiles; i++){
    tiles[i] = (Rectangle){i * tileSize, 0, tileSize, tileSize};
  }
  
  Rectangle source = tiles[10];

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    drawBoard(atlas, tiles, source);
    
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
      Vector2 mouse = GetMousePosition();

      int gridX = (mouse.x - middleX) / tileSize;
      int gridY = (mouse.y - middleY) / tileSize;

      if(gridX >= 0 && gridX < 8 && gridY >= 0 && gridY < 8){
        TraceLog(LOG_INFO, "Clicked cell: %d %d", gridX, gridY);
      }

    }

    //DrawTextureRec(atlas, bomb,(Vector2){0 * tileSize + middleX, 1 * tileSize + middleY}, WHITE);
        
    //DrawTextureRec(atlas, source , (Vector2){0, 0}, WHITE);
    EndDrawing();
}

  CloseWindow();
  return 0;
}

