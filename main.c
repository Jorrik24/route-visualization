#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "visualizer.h"
#include "board.h"
#include <time.h>

#define APP_TITLE ("SDL")
#define APP_WIDTH (800)
#define APP_HEIGHT (800)

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

static Uint64 last_time = 0;
static const Uint64 frame_delay = 100;

visualizer_context visualizer;

visualizer_context visualize_setup(void) {
    srand((unsigned int) time(NULL));

    board brd = create_board();

    tile_arrays arrays = { };
    tile_array array = {};

    tile start = brd.tiles[0][0];
    add_tile(&array, start);
    add_tile_array(&arrays, &array);

    tile_array unique_tiles = (tile_array){ };
    add_tile(&unique_tiles, start);

    return (visualizer_context){
        .board        = brd,
        .arrays       = arrays,
        .start        = start,
        .end          = brd.tiles[49][49],
        .route        = (tile_array){ },
        .unique_tiles = unique_tiles,
    };
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer(APP_TITLE, APP_WIDTH, APP_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        return SDL_APP_FAILURE;
    }

    last_time = SDL_GetTicks();

    visualizer = visualize_setup();

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    Uint64 now = SDL_GetTicks();
    Uint64 frame_time = now - last_time;

    if (frame_time < frame_delay) {
        SDL_Delay(frame_delay - frame_time);
        return SDL_APP_CONTINUE;
    }

    last_time = SDL_GetTicks();

    SDL_RenderClear(renderer);

    draw_board(&visualizer.board, renderer);

    if (visualizer.route.count == 0) {
        // draw_arrays(&visualizer.arrays, renderer);
        draw_array(&visualizer.unique_tiles, renderer);
        visualize_next_step(&visualizer, renderer);
    } else {
        draw_result(&visualizer.route, renderer);
    }

    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    //
}


