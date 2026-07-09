#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "menu.h"
#include <stdbool.h>

int main(void)
{
    init();
    Texture2D* txt_src;
    Texture2D* txt_dest;
    
    const int x_offset = 300;
    const int y_offset = 50;
    const int y_gap = 25;
    
    const float scale_factor = 0.25f;
    const float rotation_factor = 0.0f;
    
    Vector2 src_position = {
        .x = x_offset,
        .y = y_offset
    };

    Vector2 dest_position = {
        .x = x_offset,
        .y = y_offset + x_offset + y_gap
    };
    
    while (!should_exit() && !WindowShouldClose())
    {
        txt_src = get_texture_src();
        txt_dest = get_texture_dest();
        BeginDrawing();
            ClearBackground(RAYWHITE);
            draw_menu();
            DrawTextureEx(*txt_src, src_position, rotation_factor, scale_factor, WHITE);
            DrawTextureEx(*txt_dest, dest_position, rotation_factor, scale_factor, WHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}