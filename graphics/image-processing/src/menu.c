#include <stdio.h>
#include <stdlib.h>

#include "menu.h"
#include "algo.h"

#include "raygui.h"


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
    img.data = apply_red_filter(img.data, img.width, img.height, GetPixelDataSize(img.width, img.height, img.format));
    UnloadTexture(txt);
    txt = LoadTextureFromImage(img);
}

void edge_detection_clicked(void) {
    img.data = apply_gaussian_filter(img.data, img.width, img.height, GetPixelDataSize(img.width, img.height, img.format));
    UnloadTexture(txt);
    txt = LoadTextureFromImage(img);
}

void exit_clicked(void) {
    exit_flag = true;
}