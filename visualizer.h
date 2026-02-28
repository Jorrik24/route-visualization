#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "board.h"

typedef struct {
    tile*   tiles;
    u64     count;
    u64     capacity;
} tile_array;

typedef struct {
    tile_array* array;
    u64         count;
    u64         capacity;
} route_array;

typedef struct {
    board       board;
    route_array routes;
    tile        start;
    tile        end;
    tile_array  final_route;
    tile_array  unique_tiles;
} visualizer_context;

visualizer_context visualize_setup(void);
bool found_final_route(visualizer_context *ctx);
void add_tile(tile_array* ta, tile t);
void add_tile_array(route_array* arrs, tile_array *ta);
void draw_array(tile_array *a, SDL_Renderer *r);
void draw_result(tile_array *a, SDL_Renderer *r);
void visualize_next_step(visualizer_context *ctx, SDL_Renderer *r);
void draw_arrays(route_array* arrs, SDL_Renderer *r);

#endif