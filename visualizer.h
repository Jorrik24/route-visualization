#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <stdio.h>
#include <stdlib.h>

#include "board.h"

typedef struct {
    tile*   tiles;
    u16     count;
    u16     capacity;
} tile_array;

typedef struct {
    tile_array* array;
    u16         count;
    u16         capacity;
} tile_arrays;

typedef struct {
    board       board;
    tile_arrays arrays;
    tile        start;
    tile        end;
    tile_array  route;
    tile_array  unique_tiles;
} visualizer_context;

void add_tile(tile_array* ta, tile t);
void add_tile_array(tile_arrays* arrs, tile_array *ta);
void draw_array(tile_array *a, SDL_Renderer *r);
void draw_result(tile_array *a, SDL_Renderer *r);
void visualize_next_step(visualizer_context *ctx, SDL_Renderer *r);
void draw_arrays(tile_arrays* arrs, SDL_Renderer *r);

#endif