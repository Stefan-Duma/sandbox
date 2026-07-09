#include <stdio.h>
// #include <stdlib.h>
#include <string.h>

#include "menu.h"
#include "algo.h"

#include "raygui.h"


typedef void (*callback)(void);

void load_image_clicked(void);
void red_filter_clicked(void);
void edge_detection_clicked(void);
void exit_clicked(void);

static Image img_src;
static Image img_dest;
static Texture2D txt_src;
static Texture2D txt_dest;

static const int screenWidth = 1024;
static const int screenHeight = 768;

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

Texture2D* get_texture_src(void) {
    return &txt_src;
}

Texture2D* get_texture_dest(void) {
    return &txt_dest;
}

bool should_exit(void) {
    return exit_flag;
}

void load_image_clicked(void) {
    UnloadImage(img_src);
    UnloadImage(img_dest);

    UnloadTexture(txt_src);
    UnloadTexture(txt_dest);

    img_src = LoadImage("assets/roswaal.png");
    txt_src = LoadTextureFromImage(img_src);
    
    img_dest = LoadImageFromTexture(txt_src);
    txt_dest = LoadTextureFromImage(img_dest);
}

void red_filter_clicked(void) {
    void* ptr = img_dest.data;
    int bytes = GetPixelDataSize(img_src.width, img_src.height, img_src.format);
    
    memcpy(img_dest.data, img_src.data, bytes);

    apply_red_filter(ptr, img_src.width, img_src.height, bytes);
    
    UnloadTexture(txt_dest);
    txt_dest = LoadTextureFromImage(img_dest);
}

void edge_detection_clicked(void) {
    void* ptr = img_dest.data;
    int bytes = GetPixelDataSize(img_src.width, img_src.height, img_src.format);
    
    memcpy(img_dest.data, img_src.data, bytes);

    apply_binarization(ptr, img_src.width, img_src.height, bytes);
    apply_gaussian_filter(ptr, img_src.width, img_src.height, bytes);
    apply_sobel_operator(ptr, img_src.width, img_src.height, bytes);
    
    UnloadTexture(txt_dest);
    txt_dest = LoadTextureFromImage(img_dest);
}

void exit_clicked(void) {
    exit_flag = true;
}