#include <stdio.h>
#include <stdbool.h>
#include "SDL2/SDL.h"
#include "chip8.h"
#include "chip8keyboard.h"

// first time using go to definition in vs code
// and try to understand how this feture works
// what a cool feature
// but sometimes it is easy to understand, some times

// raw feature
// const char keyboard_map[CHIP8_TOTAL_KEYS] = {
//     0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
//     0x06, 0x07, 0xff, 0x09, 0x0A, 0x0B,
//     0x0C, 0x0D, 0x0E, 0x0F
// };

// implementation in sdl library
const char keyboard_map[CHIP8_TOTAL_KEYS] = {
    SDLK_0, SDLK_1, SDLK_2, SDLK_3, SDLK_4, SDLK_5,
    SDLK_6, SDLK_7, SDLK_8, SDLK_9, SDLK_a, SDLK_b,
    SDLK_c, SDLK_d, SDLK_e, SDLK_f
};

int main(int argc, char **argv)
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

    // no longer to use this
    // printf("%x\n", chip8_keyboard_map(keyboard_map, 0xff));
    // bool is_down = chip8_keyboard_is_down(&chip8.keyboard, 0x0f);
    // printf("%i\n", (int)is_down);

    chip8_init(&chip8);

    chip8_screen_set(&chip8.screen, 10, 1);

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow(
        EMULATOR_WINDOW_TITLE,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        CHIP8_WIDTH * CHIP8_WINDOW_MULTIPLIER,
        CHIP8_HEIGHT * CHIP8_WINDOW_MULTIPLIER,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer *renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_TEXTUREACCESS_TARGET
    );

    while (1)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            // first time using switch
            switch (event.type)
            {

            case SDL_QUIT:
                goto out;
                break;

            case SDL_KEYDOWN:
            {
                // why this is weird and long?
                // what is keysysm?
                char key = event.key.keysym.sym;
                int vkey = chip8_keyboard_map(keyboard_map, key);
                if (vkey != -1)
                {
                    chip8_keyboard_down(&chip8.keyboard, vkey);
                }
            }
            break;

            case SDL_KEYUP:
            {
                char key = event.key.keysym.sym;
                int vkey = chip8_keyboard_map(keyboard_map, key);
                if (vkey != -1)
                {
                    chip8_keyboard_up(&chip8.keyboard, vkey);
                }
            }
            break;
            };

            if (event.type == SDL_QUIT)
            {
                goto out;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
        // omg, my first loop
        // it even using XOR Calculator
        for (int x = 0; x < CHIP8_WIDTH; x++)
        {
            for (int y = 0; y < CHIP8_HEIGHT; y++)
            {
                if (chip8_screen_is_set(&chip8.screen, x, y))
                {
                    SDL_Rect r;
                    r.x = x * CHIP8_WINDOW_MULTIPLIER;
                    r.y = y * CHIP8_WINDOW_MULTIPLIER;
                    r.w = CHIP8_WINDOW_MULTIPLIER;
                    r.h = CHIP8_WINDOW_MULTIPLIER;
                    SDL_RenderFillRect(renderer, &r);
                }
            }
        }

        SDL_RenderPresent(renderer);
    }

out:
    SDL_DestroyWindow(window);
    return 0;
}
