#include <stdio.h>
#include <stdbool.h>
#include "SDL2/SDL.h"
#include "chip8.h"
#include "chip8keyboard.h"

const char keyboard_map[CHIP8_TOTAL_KEYS] = {
    0xff, 0x01, 0x02, 0x03, 0x04, 0x05,
    0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B,
    0x0C, 0x0D, 0x0E, 0x0F
};


int main(int argc, char** argv)
{

    struct chip8 chip8;
    // before
    // chip8.registers.V[0x0f] = 50;

    // // my first ampersand, dunno couldn't understand
    // chip8_memory_set(&chip8.memory, 0x400, 'Z');
    // printf("%c\n", chip8_memory_get(&chip8.memory, 50));

    // after, how clean is this
    // chip8.registers.SP = 0;
    // chip8_stack_push(&chip8, 0xff);
    // chip8_stack_push(&chip8, 0xaa);

    // printf("%x\n", chip8_stack_pop(&chip8));
    // printf("%x\n", chip8_stack_pop(&chip8));

    // after make keyboard library become like this
    // chip8_keyboard_down(&chip8.keyboard, 0x0f);
    // chip8_keyboard_up(&chip8.keyboard, 0x0f);
    
    printf("%x\n", chip8_keyboard_map(keyboard_map, 0xff));
    bool is_down = chip8_keyboard_is_down(&chip8.keyboard, 0x0f);
    printf("%i\n", (int)is_down);


    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow(
        EMULATOR_WINDOW_TITLE,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        CHIP8_WIDTH * CHIP8_WINDOW_MULTIPLIER,
        CHIP8_HEIGHT * CHIP8_WINDOW_MULTIPLIER,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_TEXTUREACCESS_TARGET
    );
    while(1)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                goto out;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer,255,255,255,0);
        SDL_Rect r;
        r.x = 0;
        r.y = 0;
        r.w = 40;
        r.h = 40;
        SDL_RenderFillRect(renderer, &r);
        SDL_RenderPresent(renderer);
    }

out:
    SDL_DestroyWindow(window);
    return 0;
}
