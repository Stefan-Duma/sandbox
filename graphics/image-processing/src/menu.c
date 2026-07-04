#include "menu.h"
#include "raygui.h"
#include <stddef.h>
#include <stdbool.h>

typedef void (*callback)(void);

void load_image_clicked(void);
void red_filter_clicked(void);
void edge_detection_clicked(void);
void exit_clicked(void);

static Image img;
static Texture2D txt;

static const int screenWidth = 800;
static const int screenHeight = 600;

static const int button_width = 120;
static const int button_height = 30;
static const int offset_x = 25;
static const int offset_y = 50;

static bool exit_flag = false;

static const char* button_labels[] = {
    "Load Image",
    "Red Filter",
    "Edge Detection",
    "Exit",
};

static const callback button_actions[] = {
    load_image_clicked,
    red_filter_clicked,
    edge_detection_clicked,
    exit_clicked,
};

void init(void) {
    InitWindow(screenWidth, screenHeight, "Image processing program in C");
    SetTargetFPS(60);

}

void draw_menu(void) {
    if(sizeof(button_labels) != sizeof(button_actions)) return;

    for(size_t i = 0; i < sizeof(button_labels) / sizeof(*button_labels); i++) {
        Rectangle btn = {
            offset_x,
            offset_y * (i + 1),
            button_width,
            button_height
        };

        if (GuiButton(btn, button_labels[i])) button_actions[i]();
    }
}

Texture2D* get_texture(void) {
    return &txt;
}

bool should_exit(void) {
    return exit_flag;
}

void load_image_clicked(void) {
    img = LoadImage("assets/roswaal.png");
    txt = LoadTextureFromImage(img);
}

void red_filter_clicked(void) {
    // Returns the total size in bytes of all pixels
    int bytes = GetPixelDataSize(img.width, img.height, img.format);
    
    int px_count = img.width * img.height;
    int px_size = bytes / px_count;
    
    void* px = NULL;
    for(size_t i = 0; i < px_count; i++) {
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

Color difference(Color c1, Color c2) {
    
}

void edge_detection_clicked(void) {
    int bytes = GetPixelDataSize(img.width, img.height, img.format);
    
    int px_count = img.width * img.height;
    int px_size = bytes / px_count;

    void* px = img.data;
    void* px_next = NULL;
    for(size_t i = 1; i < px_count; i++) {
        px_next = (unsigned char*)img.data + px_size * i;
        Color px_color = GetPixelColor(px, img.format);
        Color px_next_color = GetPixelColor(px, img.format);

    }
}

void exit_clicked(void) {
    exit_flag = true;
}