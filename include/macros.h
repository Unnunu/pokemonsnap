#ifndef _MACROS_H
#define _MACROS_H

#define UNUSED __attribute__((unused))

#if !defined(__GNUC__) && !defined(__clang__)
#define __attribute__(x)
#endif

/// Get the number of elements in a static array
#define ARRAY_COUNT(arr) (s32)(sizeof(arr) / sizeof(arr[0]))

/// Convert from a physical addresss to a ROM (0xB0) address
#define PHYSICAL_TO_ROM(x) ((uintptr_t)(x) + 0xB0000000)

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#endif
