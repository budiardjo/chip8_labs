#include "chip8screen.h"
#include <assert.h>

static void chip8_screen_check_bounds(int x, int y)
{
    assert(x >= 0 && x < CHIP8_WIDTH && y >= 0 && y < CHIP8_HEIGHT);
}

void chip8_screen_set(struct chip8_screen* screen, int x, int y)
{
    chip8_screen_check_bounds(x, y);
    screen->pixels[y][x] = true;
}

bool chip8_screen_is_set(struct chip8_screen* screen, int x, int y)
{
    chip8_screen_check_bounds(x, y);
    return screen->pixels[y][x];
}

bool chip8_screen_draw_sprite(struct chip8_screen* screen, int x, int y, const char* sprite, int num)
{
    bool pixel_collison = false;

    for (int ly=0; ly < num; ly++)
    {
        char c = sprite[ly];
        for(int lx = 0; lx < 8; lx++ )
        {
            // seriously, bitwise operator???
            // and using python repl, using lot of modulo like 63 % 64 = 63
            if ((c & (0b10000000 >> lx)) == 0)
                continue;

            if (screen->pixels[(ly+y) % CHIP8_HEIGHT][(lx+x) % CHIP8_WIDTH] ^= true)
            {

            }

            screen->pixels[(ly+y) % CHIP8_HEIGHT][(lx+x) % CHIP8_WIDTH] ^= true; // that arrow(XOR) make me angry
        }
    }
    return pixel_collison;
}
