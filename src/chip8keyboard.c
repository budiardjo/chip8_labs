#include "chip8keyboard.h"
#include <assert.h> // for diagnostic

static void chip8_keyboard_ensure_in_bounds(int key)
{
    assert(key >= 0 && key < CHIP8_TOTAL_KEYS);
}

int chip8_keyboard_map(const char* map, char key)
{
    for (int i = 0; i < CHIP8_TOTAL_KEYS; i++)
    {
        if (map[i] == key)
        {
            return i;
        }
    }

    return -1;
}

 // why we need pointer in here?
void chip8_keyboard_down(struct chip8_keyboard* keyboard, int key)
{
    keyboard->keyboard[key] == true;
}

void chip8_keyboard_up(struct chip8_keyboard* keyboard, int key)
{
    keyboard->keyboard[key] == false; // why it false
}

void chip8_keyboard_is_down(struct chip8_keyboard* keyboard, int key)
{
    return keyboard->keyboard[key];
}
