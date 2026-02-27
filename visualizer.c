#include <stdbool.h>

#include "visualizer.h"

void add_tile(tile_array* ta, tile t) {
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

void add_tile_array(tile_arrays* arrs, tile_array *ta) {
    if (arrs->count >= arrs->capacity) {
        if (arrs->capacity == 0) {
            arrs->capacity = 128;
        } else {
            arrs->capacity *= 2;
        }

        arrs->array = realloc(arrs->array, arrs->capacity * sizeof(*arrs->array));
    }

    arrs->array[arrs->count] = *ta;
    arrs->count += 1;
}

bool array_contains(tile_array* ta, tile t) {
    for (u32 i = 0; i < ta->count; i++) {
        tile tl = ta->tiles[i];

        if (tl.x == t.x && tl.y == t.y) {
            return true;
        }
    }

    return false;
}

void draw_arrays(tile_arrays *ta, SDL_Renderer *r) {
    for (u32 i = 0; i < ta->count; i++) {
        draw_array(&ta->array[i], r);
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
    for (u16 i = 0; i < a->count; i++) {
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

void temp_array_push(tile_arrays *temp_arrays, tile_array *array, tile t) {
    if (t.x < 0 || t.y < 0 || t.x >= TILES_PER_ROW || t.y >= TILES_PER_ROW) {
        return;
    }
    
    tile_array temp_array = copy_array(array);
    add_tile(&temp_array, t);

    add_tile_array(temp_arrays, &temp_array);
}

void visualize_next_step(visualizer_context *ctx, SDL_Renderer *r) {
    tile_arrays temp_arrays = {};

    for (u32 i = 0; i < ctx->arrays.count; i++) {
        tile_array array = ctx->arrays.array[i];
        tile current_tile = array.tiles[array.count - 1];

        i16 prvs_y = current_tile.y - 1;
        i16 next_x = current_tile.x + 1;
        i16 next_y = current_tile.y + 1;
        i16 prvs_x = current_tile.x - 1;

        if (ctx->route.count == 0) {
            if (current_tile.x == ctx->end.x && current_tile.y == ctx->end.y) {
                ctx->route = array;
            }

            if (prvs_y >= 0) {
                tile t = ctx->board.tiles[prvs_y][current_tile.x];

                if (t.open && !array_contains(&array, t)) {
                    if (!array_contains(&ctx->unique_tiles, t)) {
                        add_tile(&ctx->unique_tiles, t);

                        temp_array_push(&temp_arrays, &array, t);
                    }
                }
            }

            if (next_x < TILES_PER_ROW) {
                tile t = ctx->board.tiles[current_tile.y][next_x];

                if (t.open && !array_contains(&array, t)) {
                    if (!array_contains(&ctx->unique_tiles, t)) {
                        add_tile(&ctx->unique_tiles, t);

                        temp_array_push(&temp_arrays, &array, t);
                    }
                }
            }

            if (next_y < TILES_PER_ROW) {
                tile t = ctx->board.tiles[next_y][current_tile.x];

                if (t.open && !array_contains(&array, t)) {
                    if (!array_contains(&ctx->unique_tiles, t)) {
                        add_tile(&ctx->unique_tiles, t);

                        temp_array_push(&temp_arrays, &array, t);
                    }
                }
            }

            if (prvs_x >= 0) {
                tile t = ctx->board.tiles[current_tile.y][prvs_x];

                if (t.open && !array_contains(&array, t)) {
                    if (!array_contains(&ctx->unique_tiles, t)) {
                        add_tile(&ctx->unique_tiles, t);

                        temp_array_push(&temp_arrays, &array, t);
                    }
                }
            }
        }
    }

    if (temp_arrays.count > 0) {
        ctx->arrays = temp_arrays;
    }
}
