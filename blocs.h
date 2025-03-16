#ifndef BLOCS_H
#define BLOCS_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "include/raylib.h"

#define GRID_WIDTH 10
#define GRID_HEIGHT 20

#define WIN_WIDTH 372
#define WIN_HEIGHT 683

#define BLOC_SIZE 31

extern bool grid[GRID_HEIGHT][GRID_WIDTH];
extern int shapeDimensions[3][3][3];
extern Texture2D shapes[3];

typedef enum {A, D, L} Shape;
     
typedef struct {
    int pos_x;
    int pos_y;

    int shape[3][3];
    Texture2D texture;
    bool active;
} Sprite;

extern Sprite CurrentSprite;
extern Sprite* LoadedSprites;
extern int sprites_count;

bool IsGridBlockEmpty(Sprite* sprite, int dx, int dy);
void UpdateGrid(void);
void PrintGrid(void);
void LoadAllTextures(void);
Sprite CreateNewSprite(int ADL);
void DrawSprite(Sprite sprite, int screenWidth, int screenHeight);
void RotateSprite(Sprite* sprite);
void MoveSprite(Sprite* sprite);
void UnloadAllSprites(void);

#endif