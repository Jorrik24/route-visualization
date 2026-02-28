#include <stdio.h>
#include <stdbool.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "utils.h"

#ifndef BOARD_H
#define BOARD_H

typedef struct {
    u16 y, x;
    bool open;
} tile;

typedef struct {
    tile tiles[TILES_PER_ROW][TILES_PER_ROW];
} board;

board create_board(void) ;
void draw_board(board *b, SDL_Renderer *r);
bool tile_equals(tile t1, tile t2);

#endif