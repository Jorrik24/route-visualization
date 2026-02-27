#include <stdio.h>
#include <stdbool.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#ifndef BOARD_H
#define BOARD_H

#define APP_WIDTH (800)
#define APP_HEIGHT (800)

#define TILES_PER_ROW (50)
#define TILE_WIDTH (APP_WIDTH / TILES_PER_ROW)
#define TILE_HEIGHT (APP_HEIGHT / TILES_PER_ROW)

typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef struct {
    u16 y, x;
    bool open;
} tile;

typedef struct {
    tile tiles[TILES_PER_ROW][TILES_PER_ROW];
} board;

board create_board(void) ;
void draw_board(board *b, SDL_Renderer *r);

#endif