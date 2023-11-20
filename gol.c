#include "types.h"
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 65
#define HEIGHT 38

#ifndef CHAR
#define CHAR '#'
#endif //CHAR

#define PLACE(buffer, x, y) (buffer[(y) * WIDTH + (x)].state = ALIVE)

typedef enum {
    DEAD,
    ALIVE,
} CellState;

typedef struct {
    CellState state;
} Cell;

typedef enum {
    BASIC,
    PRETTY,
} RenderMode;

static Cell first_buffer[WIDTH * HEIGHT] = {};
static Cell second_buffer[WIDTH * HEIGHT] = {};

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

u8 check_neighbors(u32 index, Cell* current_buffer) {
    u8 number_alive = 0;

    for (u8 row = 0; row < 3; ++row) {
        for (u8 col = 0; col < 3; ++col) {
            i32 neighborhood_index = index - WIDTH - 1 + row * WIDTH + col;
            
            if (neighborhood_index < 0 || neighborhood_index >= WIDTH * HEIGHT) continue; // bounds checking

            if (neighborhood_index == (i32)index) continue; // skip the middle cell
    
            Cell neighbor = current_buffer[neighborhood_index];
            
            if (neighbor.state == ALIVE) {
                number_alive++;
            }
        }
    }

    return number_alive;
}

void calc_next_gen(Cell* current_buffer, Cell* write_buffer) {
    for (u32 y = 0; y < HEIGHT; ++y) {
        for (u32 x = 0; x < WIDTH; ++x) {
            u32 index = y * WIDTH + x;
            Cell cell = current_buffer[index]; 

            u8 number_alive = check_neighbors(index, current_buffer);

            if (cell.state == ALIVE && (number_alive == 2 || number_alive == 3)) {
                write_buffer[index].state = ALIVE;
            } else if (cell.state == DEAD && number_alive == 3) {
                write_buffer[index].state = ALIVE;
            } else {
                write_buffer[index].state = DEAD;
            }
        }
    }
}

void render_buffer(Cell* current_buffer, u8 character, RenderMode mode) {
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
            Cell cell = current_buffer[index];
            
            if (mode == PRETTY) {
                if (x == 0) {
                    printf("| ");
                }
            }

            if (cell.state == ALIVE) {
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
    for (i32 i = 0; i < milliseconds * 100000; ++i) {}
}

int main(void) {
    init_grid();
    Cell* buffers[2] = {first_buffer, second_buffer};
    Cell* current_buffer = buffers[0];
    Cell* write_buffer = buffers[1];
    
    for (i32 i = 0; i >= 0; ++i) {
        u8 buf_num = i%2; // either 0 or 1
        calc_next_gen(current_buffer, write_buffer);

        sleep(700);

        current_buffer = buffers[buf_num];
        write_buffer = buffers[!buf_num];

        system("clear");
        render_buffer(current_buffer, CHAR, BASIC);
    }

}
