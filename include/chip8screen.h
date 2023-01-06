#ifndef CHIP8SCRENN_H
#define CHIP8SCREEN_H

#include <stdbool.h>
#include "config.h"

struct chip8_screen
{
    // my first multiple array
    bool pixels[CHIP8_HEIGHT][CHIP8_WIDTH];
};

// he even read the instruction more than 10 minutes, try to understanding and after that to code
void chip8_screen_clear(struct chip8_screen* screen);
void chip8_screen_set(struct chip8_screen* screen, int x, int y);
bool chip8_screen_is_set(struct chip8_screen* screen, int x, int y);
bool chip8_screen_draw_sprite(struct chip8_screen* screen, int x, int y, const char* sprite, int num);

#endif
