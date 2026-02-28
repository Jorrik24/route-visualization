#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "board.h"

board create_board(void) {
    board b = {};

    for (u16 y = 0; y < TILES_PER_ROW; y++) {
        for (u16 x = 0; x < TILES_PER_ROW; x++) {
            bool is_open = (rand() % 100) < 65;

            b.tiles[x][y] = (tile){
                x, y, is_open,
            };
        }
    }

    b.tiles[0][0].open = true;
    b.tiles[1][0].open = true;
    b.tiles[0][1].open = true;

    b.tiles[TILES_PER_ROW - 1][TILES_PER_ROW - 2].open = true;
    b.tiles[TILES_PER_ROW - 2][TILES_PER_ROW - 1].open = true;
    b.tiles[TILES_PER_ROW - 1][TILES_PER_ROW - 1].open = true;

    return b;
}

void draw_board(board *b, SDL_Renderer *r) {
    for (u16 y = 0; y < TILES_PER_ROW; y++) {
        for (u16 x = 0; x < TILES_PER_ROW; x++) {
            tile t = b->tiles[y][x];

            SDL_FRect rect = {
                x * TILE_WIDTH,
                y * TILE_HEIGHT,
                TILE_WIDTH,
                TILE_HEIGHT
            };

            if (t.open == true) {
                SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
            } else {
                SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
            }

            SDL_RenderFillRect(r, &rect);
        }
    }
}

bool tile_equals(tile t1, tile t2) {
    return t1.x == t2.x && t1.y == t2.y;
}
