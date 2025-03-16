#include "blocs.h"
#include <stdio.h> 

Sprite CurrentSprite;

Sprite* LoadedSprites = NULL;
int sprites_count = 0;

Texture2D shapes[3];
bool grid[GRID_HEIGHT][GRID_WIDTH] = {false};

int shapeDimensions[3][3][3] = {
    { // Arrow 
        {0, 0, 0},
        {0, 0, 1},
        {0, 1, 1},
    },
    { // Diagonal
        {0, 1, 0},
        {0, 0, 1},
        {0, 0, 0},
    },
    { // Line
        {0, 0, 0},
        {0, 1, 1},
        {0, 0, 0},
    },
};

int track = 0;

bool IsGridBlockEmpty(Sprite* sprite, int dx, int dy) {
    bool tempGrid[GRID_HEIGHT][GRID_WIDTH];
    memcpy(tempGrid, grid, sizeof(grid));

    if (dx != 0 || dy != 0)
    for (int y = 0; y < 3; y++) for (int x = 0; x < 3; x++)

    if (sprite->shape[y][x]) {
        int grid_x = sprite->pos_x + x;
        int grid_y = sprite->pos_y + y;

        if (grid_x >= 0 && 
            grid_x < GRID_WIDTH &&
            grid_y >= 0 && 
            grid_y < GRID_HEIGHT
        )
        tempGrid[grid_y][grid_x] = false;
    }

    for (int y = 0; y < 3; y++) for (int x = 0; x < 3; x++)

    if (sprite->shape[y][x]) {
        int new_x = sprite->pos_x + x + dx;
        int new_y = sprite->pos_y + y + dy;

        if (new_x < 0 || 
            new_x >= GRID_WIDTH || 
            new_y < 0 || 
            new_y >= GRID_HEIGHT
        )
        return false;

        if (tempGrid[new_y][new_x])
        return false;
    }

    return true;
}

void UpdateGrid(void) {
    memset(grid, false, sizeof(grid));

    for (int i = 0; i < sprites_count; i++) {
        Sprite sprite = LoadedSprites[i];

        for (int y = 0; y < 3; y++) for (int x = 0; x < 3; x++)
        if (sprite.shape[y][x]) {
            int grid_x = sprite.pos_x + x;
            int grid_y = sprite.pos_y + y;

            if (grid_x >= 0 && 
                grid_x < GRID_WIDTH && 
                grid_y >= 0 && 
                grid_y < GRID_HEIGHT
            )
            grid[grid_y][grid_x] = true;
        }
    }
    
    if(CurrentSprite.active)
    for (int y = 0; y < 3; y++) for (int x = 0; x < 3; x++)
    if (CurrentSprite.shape[y][x]) {
        int grid_x = CurrentSprite.pos_x + x;
        int grid_y = CurrentSprite.pos_y + y;

        if (grid_x >= 0 && 
            grid_x < GRID_WIDTH && 
            grid_y >= 0 && 
            grid_y < GRID_HEIGHT
        )
        grid[grid_y][grid_x] = true;
    }
}

void PrintGrid(void) {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) 
        printf("| %d ", grid[y][x]);

        printf("|\n");
    }
    printf("\n\n\n");
}

void LoadAllTextures(void) {
    shapes[0] = LoadTexture("assets/shapes/A.png");
    shapes[1] = LoadTexture("assets/shapes/D.png");
    shapes[2] = LoadTexture("assets/shapes/L.png");   
}

Sprite CreateNewSprite(int ADL) { 
    LoadAllTextures();

    srand(time(NULL));
    Shape shape = rand() % 3;

    if (ADL >= 0) shape = ADL;
    
    Sprite NewSprite = {
        .pos_x = 5,
        .pos_y = 0,
        .texture = shapes[shape],
        .active = true
    };

    for (int y = 0; y < 3; y++) for (int x = 0; x < 3; x++)
    NewSprite.shape[y][x] = shapeDimensions[shape][y][x];

    return NewSprite;
}

void DrawSprite(Sprite sprite, int screenWidth, int screenHeight) {
    for (int y = 0; y < 3; y++) for (int x = 0; x < 3; x++)
    if (sprite.shape[y][x]) {
        int offset_x = (sprite.pos_x + x + 1) * BLOC_SIZE;
        int offset_y = (sprite.pos_y + y + 1) * BLOC_SIZE;
    
        int p_x = (screenWidth - WIN_WIDTH) / 2 + offset_x;
        int p_y = (screenHeight - WIN_HEIGHT) / 2 + 10 + offset_y;

        DrawTexture(sprite.texture, p_x, p_y, WHITE);
    }
}

void RotateSprite(Sprite* sprite) {
    int rotatedShape[4][4] = {0};

    for (int y = 0; y < 3; y++) for (int x = 0; x < 3; x++)
    rotatedShape[x][y] = 
    sprite->shape[ 3 - 1 - y][x];

    bool tempGrid[GRID_HEIGHT][GRID_WIDTH];
    memcpy(tempGrid, grid, sizeof(grid));

    for (int y = 0; y < 3; y++) for (int x = 0; x < 3; x++)
    if (sprite->shape[y][x]) {
        int grid_x = sprite->pos_x + x;
        int grid_y = sprite->pos_y + y;

        if (grid_x >= 0 &&
            grid_x < GRID_WIDTH &&
            grid_y >= 0 &&
            grid_y < GRID_HEIGHT
        )
        tempGrid[grid_y][grid_x] = false;
    }

    for (int y = 0; y < 3; y++) for (int x = 0; x < 3; x++)
    if (rotatedShape[y][x]) {
        int new_x = sprite->pos_x + x;
        int new_y = sprite->pos_y + y;

        if (new_x < 0 || 
            new_x >= GRID_WIDTH || 
            new_y < 0 || 
            new_y >= GRID_HEIGHT
        )
        return;

        if (tempGrid[new_y][new_x])
        return;
    }

    for (int y = 0; y < 3; y++) for (int x = 0; x < 3; x++)
    sprite->shape[y][x] = 
    rotatedShape[y][x];

    UpdateGrid();
}

void MoveSprite(Sprite* sprite) {
    switch (GetKeyPressed()) {
        case KEY_LEFT:
        case KEY_A:
            if (IsGridBlockEmpty(sprite, -1, 0))
            sprite->pos_x -= 1;
            break;
        case KEY_RIGHT:
        case KEY_D:
            if (IsGridBlockEmpty(sprite, +1, 0))
            sprite->pos_x += 1;
            break;
        case KEY_SPACE:
            RotateSprite(sprite);
            break;
        case KEY_DOWN:
        case KEY_S:
            while (IsGridBlockEmpty(sprite, 0, 1))
            sprite->pos_y += 1;
            sprite->active = false;
            break;
        default:
            break;
    }
}

void UnloadAllSprites(void) {
    for (
        int i = 0, size = 3;
        i < size;
        i++
    ) UnloadTexture(shapes[i]);
}