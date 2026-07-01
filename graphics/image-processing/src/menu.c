#include "menu.h"
#include "raygui.h"
#include <stdio.h>
#include <stddef.h>

static Image img;
static Texture2D txt;

Texture2D* get_texture(void) {
    return &txt;
}

void init(void) {
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Image processing program in C");

    SetTargetFPS(60);

}

void load_image_clicked(void) {
    img = LoadImage("assets/roswaal.png");
    txt = LoadTextureFromImage(img);
}

void red_filer_clicked(void) {
    // Returns the total size in bytes of all pixels
    int bytes = GetPixelDataSize(img.width, img.height, img.format);
    
    int px_count = img.width * img.height;
    int px_size = bytes / px_count;
    
    void* px = NULL;
    for(int i = 0; i < px_count; i++) {
        // Iterate pixel-by-pixel
        px = (unsigned char*)img.data + px_size * i;
        Color px_color = GetPixelColor(px, img.format);
        // Cancel the red channel
        px_color.r = 0;
        // Replace that color
        ImageDrawPixel(&img, i % img.width, i / img.width, px_color);
    }

    UnloadTexture(txt);
    txt = LoadTextureFromImage(img);
}

void draw_menu(void) {
    if (GuiButton((Rectangle){ 24, 24, 120, 30 }, "Load Image")){
        load_image_clicked();
    };

    if (GuiButton((Rectangle){ 24, 60, 120, 30 }, "Red Filter")){
        red_filer_clicked();
    };
}

