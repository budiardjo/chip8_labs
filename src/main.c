#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
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
    if (argc < 2)
    {
        printf(" you must provide a file to load\n");
        return -1;
    }

    // filename using char?
    const char* filename = argv[1];
    printf("the filename to load is: %s\n", filename);

    // it using using rb in windows
    FILE* f = fopen(filename, "r");
    if (!f)
    {
        printf("failed to open the file");
        return -1;
    }

    // weird function
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char buf[size];
    int res = fread(buf, size, 1, f);
    if ( res != 1)
    {
        printf("failed to read from file");
        return -1;
    }

    printf("%s\n", buf);


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
    chip8_load(&chip8, buf, size);

    // chip8_screen_set(&chip8.screen, 10, 1);
    // chip8_screen_draw_sprite(&chip8.screen, 32, 30, &chip8.memory.memory[0x00], 5);
    // chip8_exec(&chip8, 0x00E0);
    // chip8_exec(&chip8, 0x1ff2);
    // printf("%x\n", chip8.registers.PC);
    // chip8.registers.PC = 0x00;
    // chip8.registers.V[0x00] = 0x22;
    // chip8_exec(&chip8, 0x3022);
    // printf("%x\n", chip8.registers.PC);

    // chip8.registers.PC = 0;
    // chip8.registers.V[2] = 0x10;
    // chip8.registers.V[3] = 0x10;
    // chip8_exec(&chip8, 0x5230);

    // printf("%x\n", chip8.registers.PC);
    chip8.registers.V[0] = 0x20;
    chip8.registers.V[1] = 0x30;
    chip8_exec(&chip8, 0x8010);
    printf("%x\n", chip8.registers.V[0]);


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

        if (chip8.registers.delay_timer > 0)
        {
            sleep(100);
            chip8.registers.delay_timer -= 1;
        }

        // if (chip8.registers.sound_timer > 0)
        // {
        //     beep(8000, 100);
        //     chip8.registers.sound_timer -= 1;
        // }

        unsigned short opcode = chip8_memory_get_short(&chip8.memory, chip8.registers.PC);
        chip8.registers.PC += 2; // it is 2 bytes
        chip8_exec(&chip8, opcode);
        // printf("%x\n", opcode);
    }

out:
    SDL_DestroyWindow(window);
    return 0;
}
