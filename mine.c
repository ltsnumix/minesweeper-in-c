#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define tileSize 32
#define numTiles 12
#define BOARD_SIZE 30
#define NUM_BOMBS 100

typedef enum GameState {
  PLAYING,
  GAME_OVER
} GameState;

void drawBoard(Texture2D atlas, int clicked[BOARD_SIZE][BOARD_SIZE], int lclicked[BOARD_SIZE][BOARD_SIZE], int bclicked[BOARD_SIZE][BOARD_SIZE], int bombs[BOARD_SIZE][BOARD_SIZE]);
int countNeighbors(int row, int col, int bombs[BOARD_SIZE][BOARD_SIZE]);
void revealCell(int row, int col, int clicked[BOARD_SIZE][BOARD_SIZE], int bombs[BOARD_SIZE][BOARD_SIZE]);

Rectangle source;
Rectangle flagCell;
Rectangle bombCell;
Rectangle numCells[9];

void drawBoard(Texture2D atlas, int clicked[BOARD_SIZE][BOARD_SIZE], int lclicked[BOARD_SIZE][BOARD_SIZE], int bclicked[BOARD_SIZE][BOARD_SIZE], int bombs[BOARD_SIZE][BOARD_SIZE]) {
  int middleX = (GetScreenWidth() - BOARD_SIZE * tileSize) / 2;
  int middleY = (GetScreenHeight() - BOARD_SIZE * tileSize) / 2;

  for (int x = 0; x < BOARD_SIZE; x++) {
    for (int y = 0; y < BOARD_SIZE; y++) {
      Vector2 position = {x * tileSize + middleX, y * tileSize + middleY};

      if (lclicked[x][y]) {
        DrawTextureRec(atlas, flagCell, position, WHITE);
      } else if (bclicked[x][y]) {
        DrawTextureRec(atlas, bombCell, position, WHITE);
      } else if (clicked[x][y]) {
        int neighborCount = countNeighbors(x, y, bombs);
        DrawTextureRec(atlas, numCells[neighborCount], position, WHITE);
      } else {
        DrawTextureRec(atlas, source, position, WHITE);
      }
    }
  }
}

void resetGame(int clicked[BOARD_SIZE][BOARD_SIZE], int lclicked[BOARD_SIZE][BOARD_SIZE], int bclicked[BOARD_SIZE][BOARD_SIZE], int bombs[BOARD_SIZE][BOARD_SIZE]) {
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int k = 0; k < BOARD_SIZE; k++) {
      clicked[i][k] = 0;
      lclicked[i][k] = 0;
      bclicked[i][k] = 0;
      bombs[i][k] = 0;
    }
  }

  int counter = 0;
  while (counter < NUM_BOMBS) {
    int row = rand() % BOARD_SIZE;
    int col = rand() % BOARD_SIZE;
    if (bombs[row][col] == 0) {
      bombs[row][col] = 2;
      counter++;
    }
  }
}

int countNeighbors(int row, int col, int bombs[BOARD_SIZE][BOARD_SIZE]) {
  int count = 0;
  for (int dx = -1; dx <= 1; dx++) {
    for (int dy = -1; dy <= 1; dy++) {
      if (dx == 0 && dy == 0) continue;
      int nx = row + dx, ny = col + dy;
      if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE) {
        if (bombs[nx][ny] == 2) count++;
      }
    }
  }
  return count;
}

void revealCell(int row, int col, int clicked[BOARD_SIZE][BOARD_SIZE], int bombs[BOARD_SIZE][BOARD_SIZE]) {
  if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) return;
  if (clicked[row][col]) return;
  if (bombs[row][col] == 2) return;

  clicked[row][col] = 1;

  int neighbors = countNeighbors(row, col, bombs);

  if (neighbors == 0) {
    for (int dx = -1; dx <= 1; dx++) {
      for (int dy = -1; dy <= 1; dy++) {
        if (dx == 0 && dy == 0) continue;
        revealCell(row + dx, col + dy, clicked, bombs);
      }
    }
  }
}

int main(void) {
  InitWindow(1000, 1000, "Minesweeper");
  Texture2D atlas = LoadTexture("tiles.jpg");

  for (int i = 0; i < 9; i++) {
    numCells[i] = (Rectangle){i * tileSize, 0, tileSize, tileSize};
  }
  source = (Rectangle){10 * tileSize, 0, tileSize, tileSize};
  flagCell = (Rectangle){11 * tileSize, 0, tileSize, tileSize};
  bombCell = (Rectangle){9 * tileSize, 0, tileSize, tileSize};

  int clicked[BOARD_SIZE][BOARD_SIZE] = {0};
  int lclicked[BOARD_SIZE][BOARD_SIZE] = {0};
  int bclicked[BOARD_SIZE][BOARD_SIZE] = {0};
  int bombs[BOARD_SIZE][BOARD_SIZE] = {0};
  int counter = 0;
  int bombsRevealed = 0;
  GameState gameState = PLAYING;

  srand(time(NULL));
  while (counter < NUM_BOMBS) {
    int row = rand() % BOARD_SIZE;
    int col = rand() % BOARD_SIZE;
    if (bombs[row][col] == 0) {
      bombs[row][col] = 2;
      counter++;
    }
  }

  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    int middleX = (GetScreenWidth() - BOARD_SIZE * tileSize) / 2;
    int middleY = (GetScreenHeight() - BOARD_SIZE * tileSize) / 2;

    if (gameState == PLAYING) {
      if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse = GetMousePosition();
        int gridX = (mouse.x - middleX) / tileSize;
        int gridY = (mouse.y - middleY) / tileSize;

        if (gridX >= 0 && gridX < BOARD_SIZE && gridY >= 0 && gridY < BOARD_SIZE && !lclicked[gridX][gridY]) {
          if (bombs[gridX][gridY] == 2) {
            bclicked[gridX][gridY] = 1;
            TraceLog(LOG_INFO, "Game Over! You clicked a bomb at (%d, %d)", gridX, gridY);
            gameState = GAME_OVER;
          } else {
            revealCell(gridX, gridY, clicked, bombs);
            TraceLog(LOG_INFO, "Revealed cell: %d %d", gridX, gridY);
          }
        }
      }

      if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        Vector2 mouse = GetMousePosition();
        int gridX = (mouse.x - middleX) / tileSize;
        int gridY = (mouse.y - middleY) / tileSize;

        if (gridX >= 0 && gridX < BOARD_SIZE && gridY >= 0 && gridY < BOARD_SIZE && !clicked[gridX][gridY]) {
          lclicked[gridX][gridY] = !lclicked[gridX][gridY];
          TraceLog(LOG_INFO, "%s cell: %d %d", lclicked[gridX][gridY] ? "Flagged" : "Unflagged", gridX, gridY);
        }
      }
    }

    BeginDrawing();
    ClearBackground(LIGHTGRAY);
    drawBoard(atlas, clicked, lclicked, bclicked, bombs);

    if (gameState == GAME_OVER) {
      for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
          if (bombs[x][y] == 2) {
            DrawTextureRec(atlas, bombCell,
              (Vector2){x * tileSize + middleX, y * tileSize + middleY}, WHITE);
          }
        }
      }

      if (bombsRevealed) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
          resetGame(clicked, lclicked, bclicked, bombs);
          gameState = PLAYING;
          bombsRevealed = 0;
        }
      } else {
        bombsRevealed = 1;
      }
    }

    EndDrawing();
  }

  UnloadTexture(atlas);
  CloseWindow();
  return 0;
}
