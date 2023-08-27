#include "types.h"
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 65
#define HEIGHT 38
#define CHAR '#'

#define ARR_LEN(arr) (sizeof(arr)/sizeof(arr[0]))
#define PLACE(buffer, x, y) (buffer[(y) * WIDTH + (x)] = ALIVE)

typedef enum {
    DEAD,
    ALIVE,
} CellState;

typedef enum {
    BASIC,
    PRETTY,
} RenderMode;

static CellState first_buffer[WIDTH * HEIGHT] = {};
static CellState second_buffer[WIDTH * HEIGHT] = {};

void init_grid() {
    PLACE(first_buffer, 1, 5);
    PLACE(first_buffer, 1, 6);
    PLACE(first_buffer, 2, 5);
    PLACE(first_buffer, 2, 6);

    PLACE(first_buffer, 11, 5);
    PLACE(first_buffer, 11, 6);
    PLACE(first_buffer, 11, 7);

    PLACE(first_buffer, 12, 4);
    PLACE(first_buffer, 12, 8);

    PLACE(first_buffer, 13, 3);
    PLACE(first_buffer, 13, 9);

    PLACE(first_buffer, 14, 3);
    PLACE(first_buffer, 14, 9);
    
    PLACE(first_buffer, 15, 6);

    PLACE(first_buffer, 16, 4);
    PLACE(first_buffer, 16, 8);

    PLACE(first_buffer, 17, 5);
    PLACE(first_buffer, 17, 6);
    PLACE(first_buffer, 17, 7);

    PLACE(first_buffer, 18, 6);

    PLACE(first_buffer, 21, 5);
    PLACE(first_buffer, 21, 4);
    PLACE(first_buffer, 21, 3);

    PLACE(first_buffer, 22, 5);
    PLACE(first_buffer, 22, 4);
    PLACE(first_buffer, 22, 3);

    PLACE(first_buffer, 23, 2);
    PLACE(first_buffer, 23, 6);

    PLACE(first_buffer, 25, 1);
    PLACE(first_buffer, 25, 2);
    PLACE(first_buffer, 25, 6);
    PLACE(first_buffer, 25, 7);

    PLACE(first_buffer, 35, 3);
    PLACE(first_buffer, 35, 4);
    PLACE(first_buffer, 36, 3);
    PLACE(first_buffer, 36, 4);
}

void check_neighbors(u32 index, CellState* current_buffer, u8* number_dead, u8* number_alive) {
    for (u8 row = 0; row < 3; ++row) {
        for (u8 col = 0; col < 3; ++col) {
            i32 neighborhood_index = index - WIDTH - 1 + row * WIDTH + col;
            
            if (neighborhood_index < 0 || neighborhood_index >= WIDTH * HEIGHT) continue; // bounds checking

            if (neighborhood_index == (i32)index) continue; // skip the middle cell
    
            CellState neighbor = current_buffer[neighborhood_index];
            
            if (neighbor) {
                (*number_alive)++;
            } else{
                (*number_dead)++;
            }
        }
    }
}

void calc_next_gen(CellState* current_buffer, CellState* write_buffer) {
    for (u32 y = 0; y < HEIGHT; ++y) {
        for (u32 x = 0; x < WIDTH; ++x) {
            u32 index = y * WIDTH + x;
            CellState cell = current_buffer[index]; 

            u8 number_dead  = 0;
            u8 number_alive = 0;

            check_neighbors(index, current_buffer, &number_dead, &number_alive);

            if (cell == ALIVE && (number_alive == 2 || number_alive == 3)) {
                write_buffer[index] = ALIVE;
            } else if (cell == DEAD && number_alive == 3) {
                write_buffer[index] = ALIVE;
            } else {
                write_buffer[index] = DEAD;
            }
        }
    }
}

void render_buffer(CellState* current_buffer, u8 character, RenderMode mode) {
    if (mode == PRETTY) {
        printf("+ ");
        for (u32 i = 0; i < WIDTH; ++i) {
            printf("- ");
        }
        printf("+\n");
    }
    
    for (u32 y = 0; y < HEIGHT; ++y) {
        for (u32 x = 0; x < WIDTH; ++x) {
            u32 index = y * WIDTH + x;
            CellState cell = current_buffer[index];
            
            if (mode == PRETTY) {
                if (x == 0) {
                    printf("| ");
                }
            }

            if (cell) {
                printf("%c ", character);
            } else {
                printf(". ");
            }

            if (mode == PRETTY) {
                if (x + 1 == WIDTH) {
                    printf("|");
                }
            }

            if (x + 1 == WIDTH) {
                printf("\n");
            }
        }
    }

    if (mode == PRETTY) {
        printf("+ ");
        for (u32 i = 0; i < WIDTH; ++i) {
            printf("- ");
        }
        printf("+\n");
    }
}

void sleep(i32 milliseconds) {
    for (i32 i = 0; i < milliseconds * 10000; ++i) {}
}

int main(void) {
    init_grid();
    CellState* buffers[2] = {first_buffer, second_buffer};
    CellState* current_buffer = buffers[0];
    CellState* write_buffer = buffers[1];
    
    for (i32 i = 0; i >= 0; ++i) {
        u8 buf_num = i%2; // either 0 or 1
        calc_next_gen(current_buffer, write_buffer);

        sleep(700);

        current_buffer = buffers[buf_num];
        write_buffer = buffers[!buf_num];

        system("clear");
        render_buffer(current_buffer, CHAR, PRETTY);
    }

}
