#ifndef MENU_H
#define MENU_H

#include "raylib.h"
#include <stdbool.h>

void init(void);
void draw_menu(void);
Texture2D* get_texture_src(void);
Texture2D* get_texture_dest(void);
bool should_exit(void);

#endif // MENU_H