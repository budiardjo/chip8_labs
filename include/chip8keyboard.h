#ifndef CHIP8KEYBOARD_H
#define CHIP8KEYBOARD_H

// why i have to use this library?
#include <stdbool.h>
#include "config.h"

struct chip8_keyboard
{
    bool keyboard[CHIP8_TOTAL_KEYS];
};

int chip8_keyboard_map(const char* map, char key); // most complicated
void chip8_keyboard_down(struct chip8_keyboard* keyboard, int key); // why we need pointer in here?
void chip8_keyboard_up(struct chip8_keyboard* keyboard, int key);
bool chip8_keyboard_is_down(struct chip8_keyboard* keyboard, int key);

#endif
