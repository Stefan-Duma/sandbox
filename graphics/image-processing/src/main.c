#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "menu.h"

int main(void)
{
    init();
    Texture2D* txt;

    while (!WindowShouldClose())
    {
        txt = get_texture();
        BeginDrawing();
            ClearBackground(RAYWHITE);
            draw_menu();
            DrawTextureEx(*txt, (Vector2){150, 150}, 0.0f, 0.25f, WHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}