#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define tileSize 32
#define numTiles 12
#define middleX (500 - 8 * 32) / 2 
#define middleY (700 - 8 * 32) / 2 

void drawBoard(Texture2D atlas, Rectangle tiles[], Rectangle source, Rectangle openCell, Rectangle flagCell, Rectangle bombCell, int clicked[8][8], int lclicked[8][8], int bclicked[8][8]){

  for(int x = 0; x < 8; x++){
    for(int y = 0; y < 8; y++){
      DrawTextureRec(atlas, source, (Vector2){x * tileSize + middleX, y * tileSize + middleY}, WHITE);
      if(lclicked[x][y]){
        DrawTextureRec(atlas, flagCell, (Vector2){x * tileSize + middleX, y * tileSize + middleY}, WHITE);
      }
      else if(clicked[x][y]){
        DrawTextureRec(atlas, openCell, (Vector2){x * tileSize + middleX, y * tileSize + middleY}, WHITE);
      }
      else if(bclicked[x][y]){
        DrawTextureRec(atlas, bombCell, (Vector2){x * tileSize + middleX, y * tileSize + middleY}, WHITE);
      }
      else {
        DrawTextureRec(atlas, source, (Vector2){x * tileSize + middleX, y * tileSize + middleY}, WHITE);
      }
    }
  }
}

int main(void) {

  InitWindow(500, 700, "Minesweeper");
  Texture2D atlas = LoadTexture("tiles.jpg");
  Rectangle tiles[numTiles];
  int clicked[8][8] = {0};
  int lclicked[8][8] = {0};
  int bclicked[8][8] = {0};
  int bombs[8][8] = {0};
  int count = 0;

  srand(time(NULL));
  
  while(count < 10){
    int row = rand() % 8;
    int col = rand() % 8;
    
    if(bombs[row][col] == 0){
      bombs[row][col] = 2;
      count++;
    }

  }

  //Vector2 clickPosition = {0, 0};
  
  for(int i = 0; i < numTiles; i++){
    tiles[i] = (Rectangle){i * tileSize, 0, tileSize, tileSize};
  }
  
  Rectangle source = tiles[10];
  Rectangle openCell = tiles[0];
  Rectangle flagCell = tiles[11];
  Rectangle bombCell = tiles[9];

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(LIGHTGRAY);
    //drawBoard(atlas, tiles, source, openCell, flagCell, clicked, lclicked);
    
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
      Vector2 mouse = GetMousePosition();

      int gridX = (mouse.x - middleX) / tileSize;
      int gridY = (mouse.y - middleY) / tileSize;

      if(gridX >= 0 && gridX < 8 && gridY >= 0 && gridY < 8 && !clicked[gridX][gridY] && lclicked[gridX][gridY] == 0 && bombs[gridX][gridY] == 0){
        //clickedX = gridX;
        //clickedY = gridY;
        
        clicked[gridX][gridY] = 1;

        TraceLog(LOG_INFO, "Clicked cell: %d %d", gridX, gridY);
      } else if(bombs[gridX][gridY] == 2) {
        bclicked[gridX][gridY] = 1;
        TraceLog(LOG_INFO, "Clicked at bomb");
      }

    }
    
    if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)){
      Vector2 mouse = GetMousePosition();

      int gridX = (mouse.x - middleX) / tileSize;
      int gridY = (mouse.y - middleY) / tileSize;

      if(gridX >= 0 && gridX < 8 && gridY >= 0 && gridY < 8 && !lclicked[gridX][gridY] && clicked[gridX][gridY] == 0){
        //clickedX = gridX;
        //clickedY = gridY;
        lclicked[gridX][gridY] = 1;
        TraceLog(LOG_INFO, "Flagged cell: %d %d", gridX, gridY);
      }
      else if(gridX >= 0 && gridX < 8 && gridY >= 0 && gridY < 8 && lclicked[gridX][gridY]){
        lclicked[gridX][gridY] = 0;
        TraceLog(LOG_INFO, "Unflagged cell: %d %d", gridX, gridY);
      }

    }
    
    drawBoard(atlas, tiles, source, openCell, flagCell, bombCell, clicked, lclicked, bclicked);

    EndDrawing();
}

  CloseWindow();
  return 0;
}




