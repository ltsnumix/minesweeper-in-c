#include "raylib.h"
#include <stdio.h>

#define tileSize 32
#define numTiles 11

Texture2D atlas;

int main(void) {

    InitWindow(500, 700, "Minesweeper");
    atlas = LoadTexture("tiles.jpg"); 
    
    Rectangle tiles[numTiles];
    for (int i = 0; i <= numTiles; i++) {
      tiles[i] = (Rectangle){ i * tileSize, 0, tileSize, tileSize };
    }
    
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        Rectangle source = tiles[11];
        DrawTextureRec(atlas, source , (Vector2){0, 0}, WHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

