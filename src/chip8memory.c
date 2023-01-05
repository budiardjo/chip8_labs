#include "chip8memory.h"
// my first assert library
#include <assert.h>

// cool feature for prevent memory or program to crash
static void chip8_is_bound_in_bounds(int index)
{
    assert(index >= 0 && index < CHIP8_MEMORY_SIZE);
}

void chip8_memory_set(struct chip8_memory* memory, int index, unsigned char val)
{
    chip8_is_bound_in_bounds(index);
    memory->memory[index] = val;
}

unsigned char chip8_memory_get(struct chip8_memory* memory, int index)
{
    chip8_is_bound_in_bounds(index);
    return memory->memory[index];
}

unsigned short chip8_memory_get_short(struct chip8_memory* memory, int index)
{
    unsigned char byte1 = chip8_memory_get(memory, index);
    unsigned char byte2 = chip8_memory_get(memory, index+1);
    return byte1 << 8 | byte2;
}
