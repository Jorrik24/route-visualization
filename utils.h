#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

#define APP_TITLE ("SDL")
#define APP_WIDTH (800)
#define APP_HEIGHT (800)

#define TILES_PER_ROW (200)
#define TILE_WIDTH (APP_WIDTH / TILES_PER_ROW)
#define TILE_HEIGHT (APP_HEIGHT / TILES_PER_ROW)

#define ONLY_SHOW_POSSIBLE_FINAL_ROUTES (true)

typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#endif