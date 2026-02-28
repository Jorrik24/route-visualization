#include <stdbool.h>
#include <time.h>

#include "utils.h"
#include "visualizer.h"

visualizer_context visualize_setup(void) {
    srand((unsigned int) time(NULL));

    board board = create_board();

    route_array routes = {};
    tile_array array = {};

    tile start = board.tiles[0][0];
    add_tile(&array, start);
    add_tile_array(&routes, &array);

    tile_array unique_tiles = (tile_array){ };
    add_tile(&unique_tiles, start);

    return (visualizer_context){
        .board        = board,
        .routes       = routes,
        .start        = start,
        .end          = board.tiles[TILES_PER_ROW - 1][TILES_PER_ROW - 1],
        .final_route  = (tile_array){ },
        .unique_tiles = unique_tiles,
    };
}

bool found_final_route(visualizer_context *ctx) {
    return ctx->final_route.count != 0;
}

void add_tile(tile_array *ta, tile t) {
    if (ta->count >= ta->capacity) {
        if (ta->capacity == 0) {
            ta->capacity = 128;
        } else {
            ta->capacity *= 2;
        }

        ta->tiles = realloc(ta->tiles, ta->capacity * sizeof(*ta->tiles));
    }

    ta->tiles[ta->count] = t;
    ta->count += 1;
}

void add_tile_array(route_array *routes, tile_array *array) {
    if (routes->count >= routes->capacity) {
        if (routes->capacity == 0) {
            routes->capacity = 128;
        } else {
            routes->capacity *= 2;
        }

        routes->array = realloc(routes->array, routes->capacity * sizeof(*routes->array));
    }

    routes->array[routes->count] = *array;
    routes->count += 1;
}

bool array_contains(tile_array *array, tile t) {
    for (u32 i = 0; i < array->count; i++) {
        tile tl = array->tiles[i];

        if (tl.x == t.x && tl.y == t.y) {
            return true;
        }
    }

    return false;
}

void draw_arrays(route_array *array, SDL_Renderer *r) {
    for (u32 i = 0; i < array->count; i++) {
        draw_array(&array->array[i], r);
    }
}

void draw_array(tile_array *a, SDL_Renderer *r) {
    for (u32 i = 0; i < a->count; i++) {
        tile ta_tile = a->tiles[i];

        SDL_FRect rect = {
            ta_tile.x * TILE_WIDTH,
            ta_tile.y * TILE_HEIGHT,
            TILE_WIDTH,
            TILE_HEIGHT
        };

        SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
        SDL_RenderFillRect(r, &rect);
    }
}

void draw_result(tile_array *a, SDL_Renderer *r) {
    for (u32 i = 0; i < a->count; i++) {
        tile ta_tile = a->tiles[i];

        SDL_FRect rect = {
            ta_tile.x * TILE_WIDTH,
            ta_tile.y * TILE_HEIGHT,
            TILE_WIDTH,
            TILE_HEIGHT
        };

        SDL_SetRenderDrawColor(r, 0, 128, 0, 255);

        SDL_RenderFillRect(r, &rect);
    }
}

tile_array copy_array(tile_array *a) {
    tile_array result;

    result.count = a->count;
    result.capacity = a->capacity;

    result.tiles = malloc(sizeof(tile) * result.capacity);
    memcpy(result.tiles, a->tiles, sizeof(tile) * result.count);

    return result;
}

void temp_array_push(route_array *temp_arrays, tile_array *array, tile tile) {
    if (tile.x < 0 || tile.y < 0 || tile.x >= TILES_PER_ROW || tile.y >= TILES_PER_ROW) {
        return;
    }

    if (array_contains(array, tile)) {
        return;
    }
    
    tile_array temp_array = copy_array(array);
    add_tile(&temp_array, tile);

    add_tile_array(temp_arrays, &temp_array);
}

void visualize_next_step(visualizer_context *ctx, SDL_Renderer *r) {
    route_array temp_arrays = {};

    for (u32 i = 0; i < ctx->routes.count; i++) {
        tile_array array = ctx->routes.array[i];
        tile current_tile = array.tiles[array.count - 1];

        i16 prvs_y = current_tile.y - 1;
        i16 next_x = current_tile.x + 1;
        i16 next_y = current_tile.y + 1;
        i16 prvs_x = current_tile.x - 1;

        if (found_final_route(ctx)) {
            return;
        }

        if (tile_equals(ctx->end, current_tile)) {
            ctx->final_route = array;
        }

        if (prvs_y >= 0) {
            tile tile = ctx->board.tiles[prvs_y][current_tile.x];

            if (!array_contains(&ctx->unique_tiles, tile) && tile.open) {
                add_tile(&ctx->unique_tiles, tile);

                temp_array_push(&temp_arrays, &array, tile);
            }
        }

        if (next_x < TILES_PER_ROW) {
            tile tile = ctx->board.tiles[current_tile.y][next_x];

            if (!array_contains(&ctx->unique_tiles, tile) && tile.open) {
                add_tile(&ctx->unique_tiles, tile);

                temp_array_push(&temp_arrays, &array, tile);
            }
        }

        if (next_y < TILES_PER_ROW) {
            tile tile = ctx->board.tiles[next_y][current_tile.x];

            if (!array_contains(&ctx->unique_tiles, tile) && tile.open) {
                add_tile(&ctx->unique_tiles, tile);

                temp_array_push(&temp_arrays, &array, tile);
            }
        }

        if (prvs_x >= 0) {
            tile tile = ctx->board.tiles[current_tile.y][prvs_x];

            if (!array_contains(&ctx->unique_tiles, tile) && tile.open) {
                add_tile(&ctx->unique_tiles, tile);

                temp_array_push(&temp_arrays, &array, tile);
            }
        }
    }

    if (temp_arrays.count > 0) {
        ctx->routes = temp_arrays;
    }
}
