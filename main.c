#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "visualizer.h"
#include "board.h"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

static Uint64 last_time = 0;
static const Uint64 frame_delay = 25;

visualizer_context ctx;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer(APP_TITLE, APP_WIDTH, APP_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        return SDL_APP_FAILURE;
    }

    last_time = SDL_GetTicks();

    ctx = visualize_setup();

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

    draw_board(&ctx.board, renderer);

    if (!found_final_route(&ctx)) {
        if (ONLY_SHOW_POSSIBLE_FINAL_ROUTES) {
            draw_arrays(&ctx.routes, renderer);
        } else {
            draw_array(&ctx.unique_tiles, renderer);
        }
         
        visualize_next_step(&ctx, renderer);
    } else {
        draw_result(&ctx.final_route, renderer);
    }

    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    //
}


