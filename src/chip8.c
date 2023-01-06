#include "chip8.h"
#include <memory.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

const char chip8_default_character_set[] = {
    // it is using Hex
    // painful
    // in instruction it is using capital
    // but we need turn into small case
    0xf0, 0x90, 0x90, 0x90, 0xf0,
    0x20, 0x60, 0x20, 0x20, 0x70,
    0xf0, 0x10, 0xf0, 0x80, 0xf0,
    0xf0, 0x10, 0xf0, 0x10, 0xf0,
    0x90, 0x90, 0xf0, 0x10, 0x10,
    0xf0, 0x80, 0xf0, 0x10, 0xf0,
    0xf0, 0x80, 0xf0, 0x90, 0xf0,
    0xf0, 0x10, 0x20, 0x40, 0x40,
    0xf0, 0x90, 0xf0, 0x90, 0xf0,
    0xf0, 0x90, 0xf0, 0x10, 0xf0,
    0xf0, 0x90, 0xf0, 0x90, 0x90,
    0xe0, 0x90, 0xe0, 0x90, 0xe0,
    0xf0, 0x80, 0x80, 0x80, 0xf0,
    0xe0, 0x90, 0x90, 0x90, 0xe0,
    0xf0, 0x80, 0xf0, 0x80, 0xf0,
    0xf0 ,0x80 ,0xf0 ,0x80 ,0x80
};

void chip8_init(struct chip8* chip8)
{
    memset(chip8, 0, sizeof(struct chip8));
    memcpy(&chip8->memory.memory, chip8_default_character_set, sizeof(chip8_default_character_set));
}

void chip8_load(struct chip8* chip8, const char* buf, size_t size)
{
    assert(size+CHIP8_PROGRAM_LOAD_ADDRESS < CHIP8_MEMORY_SIZE);
    memcpy(&chip8->memory.memory[CHIP8_PROGRAM_LOAD_ADDRESS], buf, size);
    chip8->registers.PC = CHIP8_PROGRAM_LOAD_ADDRESS;
}

static void chip8_exec_extended_eight(struct chip8* chip8, unsigned short opcode)
{
    unsigned char x = (opcode >> 8) & 0x000f;
    unsigned char y = (opcode >> 4) & 0x000f;
    unsigned char final_four_bits = opcode & 0x000f;
    unsigned char tmp = 0;
    switch(final_four_bits)
    {
        // 8xy0 - LD Vx, Vy, Vx = Vy
        case 0x00:
            chip8->registers.V[x] = chip8->registers.V[y];
        break;

        // 8xy - OR Vx, Vy. Performs a bitwise OR on Vx and Vy stores the result in Vx
        case 0x01:
            chip8->registers.V[x] = chip8->registers.V[x] | chip8->registers.V[y];
        break;

        // AND
        case 0x02:
            chip8->registers.V[x] = chip8->registers.V[x] & chip8->registers.V[y];
        break;

        // XOR
        case 0x03:
            chip8->registers.V[x] = chip8->registers.V[x] ^ chip8->registers.V[y];
        break;

        // carry
        case 0x04:
            tmp = chip8->registers.V[x] + chip8->registers.V[y];
            chip8->registers.V[0x0f] = false;
            if (tmp > 0xff)
            {
                chip8->registers.V[0x0f] = true;
            }

            chip8->registers.V[x] = tmp;
        break;

        // Not borrow
        case 0x05:
            chip8->registers.V[0x0f] = false;
            if ( chip8->registers.V[x] > chip8->registers.V[y])
            {
                chip8->registers.V[0x0f] = true;
            }
            chip8->registers.V[x] = chip8->registers.V[x] - chip8->registers.V[y];
        break;

        //least-significant bit it is a term in assembly code
        case 0x06:
            // it is same like above but make it one line
            chip8->registers.V[0x0f] = chip8->registers.V[x] & 0x01;
            chip8->registers.V[x] = chip8->registers.V[x] / 2;
        break;

        case 0x07:
            chip8->registers.V[0x0f] = chip8->registers.V[y] > chip8->registers.V[x];
            chip8->registers.V[x] = chip8->registers.V[y] - chip8->registers.V[x];
        break;

        case 0x0E:
            chip8->registers.V[0x0f] = chip8->registers.V[x] & 0b10000000;
            chip8->registers.V[x] = chip8->registers.V[x] * 2;
        break;
    }
}

static void chip8_exec_extended(struct chip8* chip8, unsigned short opcode)
{
    unsigned short nnn = opcode & 0x0fff;
    unsigned char x = (opcode >> 8) & 0x000f;
    unsigned char y = (opcode >> 4) & 0x000f;
    unsigned char kk = opcode & 0x00ff;
    unsigned char n = opcode & 0x000f;
    switch(opcode & 0xf000)
    {
        // bitwise operator things
        case 0x1000:
        // JP addr, 1nnn jump to location nnns
            chip8->registers.PC = nnn;
        break;

        // CALL addr, Call subroutine at location nnn
        case 0x2000:
            chip8_stack_push(chip8, chip8->registers.PC);
            chip8->registers.PC = nnn;
        break;

        // SE Vx, byte - 3xkk skip next instruction if Vx==kk
        case 0x3000:
            if (chip8->registers.V[x] == kk)
            {
                chip8->registers.PC += 2;
            }

        break;

        // SE Vx, byte - 3xkk skip next instruction if Vx!=kk
        case 0x4000:
            if (chip8->registers.V[x] != kk)
            {
                chip8->registers.PC += 2;
            }

        break;

        // 5xy0 - SE, Vx, Vy, skip the next instrction if Vx = Vy
        case 0x5000:
            if(chip8->registers.V[x] == chip8->registers.V[y])
            {
                chip8->registers.PC += 2;
            }
        break;

        // 6xkk - LD Vx, byte, Vx = kk
        case 0x6000:
            chip8->registers.V[x] = kk;
        break;

        // 7xkk - ADD Vx, byte. Set Vx = Vx + kk
        case 0x7000:
            chip8->registers.V[x] += kk;
        break;

        case 0x8000:
            chip8_exec_extended_eight(chip8, opcode);
        break;

        case 0x9000:
            if ( chip8->registers.V[x] != chip8->registers.V[y])
            {
                chip8->registers.PC += 2;
            }
        break;

        // Annn - LD I, addr. Sets the I registers to nnn
        case 0xA000:
            chip8->registers.I = nnn;
        break;

        case 0xB000:
            chip8->registers.PC = nnn + chip8->registers.V[0x00];
        break;

        case 0xC000:
            srand(clock());
            chip8->registers.V[x] = (rand() % 255) & kk;
        break;

        // Dxyn - DRW Vx, Vy, nibble.
        case 0xD000:
        {
            const char* sprite = (const char*) &chip8->memory.memory[chip8->registers.I];
            chip8->registers.V[0x0f] = chip8_screen_draw_sprite(&chip8->screen, chip8->registers.V[x], chip8->registers.V[y], sprite, n);
        }
        break;
    }
}

void chip8_exec(struct chip8* chip8, unsigned short opcode)
{
    switch(opcode)
    {
        // CLS:clear the display
        case 0x00E0:
            chip8_screen_clear(&chip8->screen);
        break;

        // Ret: Return from subroutine
        case 0x00EE:
            chip8->registers.PC = chip8_stack_pop(chip8);
        break;

        default:
            chip8_exec_extended(chip8, opcode);
    }
}

