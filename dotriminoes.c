#include "blocs.h"

Texture2D background;

float fallTimer = 0.0f;
const float fallInterval = 0.8f;

Sprite SpawnNewSprite(void);
void ResetGame(void);
void CleanUp(void);

Sprite SpawnNewSprite(void) {
    CurrentSprite.active = true;

    sprites_count++;
    LoadedSprites = realloc(
        LoadedSprites, sprites_count * sizeof(Sprite)
    );
    
    if (LoadedSprites == NULL)
    CleanUp();

    LoadedSprites[sprites_count - 1] = CurrentSprite;
    Sprite newSprite = CreateNewSprite(-1);
    UpdateGrid();

    if (!IsGridBlockEmpty(&newSprite, 0, 0))
    ResetGame();

    return newSprite;
}

void ResetGame(void) {
    free(LoadedSprites);
    LoadedSprites = NULL;
    sprites_count = 0;
}

int main(void) { 
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    InitWindow(screenWidth, screenHeight, "Dotriminoes");
    ToggleFullscreen();
    SetTargetFPS(60);
    
    background = LoadTexture("assets/Board.png");
    CurrentSprite = CreateNewSprite((Shape) L);
    
    while(!WindowShouldClose()) {
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();
        
        fallTimer += GetFrameTime();
        BeginDrawing();

        ClearBackground(BLACK);
        DrawTexture(
            background, (screenWidth - WIN_WIDTH) / 2, 
            (screenHeight - WIN_HEIGHT) / 2 + 10, WHITE
        );
        DrawText(
            TextFormat("Score : %02d", sprites_count), 
            (screenWidth - WIN_WIDTH / 2) / 2 + BLOC_SIZE, 
            15, 25, WHITE
        );

        for (int i = 0; i < sprites_count; i++)
        DrawSprite(LoadedSprites[i], screenWidth, screenHeight);

        MoveSprite(&CurrentSprite);
        if (fallTimer >= fallInterval) {
            if(IsGridBlockEmpty(&CurrentSprite, 0, 1))
            CurrentSprite.pos_y ++;
            else
            CurrentSprite.active = false;

            PrintGrid();
            fallTimer = 0.0f;
        }
        
        UpdateGrid();
        DrawSprite(CurrentSprite, screenWidth, screenHeight);

        if (!CurrentSprite.active)  
        CurrentSprite = SpawnNewSprite();

        EndDrawing();
    }
    
    CleanUp();
    return 0;
}

void CleanUp(void) {
    UnloadTexture(background);
    UnloadAllSprites();

    free(LoadedSprites);
    CloseWindow();
    exit(0);
}
