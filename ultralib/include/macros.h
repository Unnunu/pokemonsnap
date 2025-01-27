#ifndef __MACROS_H__
#define __MACROS_H__

#define ALIGNED(x) __attribute__((aligned(x)))

#define ARRLEN(x) ((s32) (sizeof(x) / sizeof(x[0])))

#define STUBBED_PRINTF(x) ((void) (x))

#define UNUSED __attribute__((unused))

#if !defined(__GNUC__) && !defined(__clang__)
#define __attribute__(x)
#endif

#define offsetof(st, m) ((u32) & (((st*) 0)->m))

/// Get the number of elements in a static array
#define ARRAY_COUNT(arr) (s32)(sizeof(arr) / sizeof(arr[0]))

#define PANIC() \
    while (1) { \
    }

/// Convert from a physical addresss to a ROM (0xB0) address
#define PHYSICAL_TO_ROM(x) ((uintptr_t) (x) + 0xB0000000)

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

#define ABS(x) (((x) < 0) ? (-(x)) : (x))

#define SQ(x) ((x) * (x))

#define M_PI_F ((f32) M_PI)

#define FLOAT_MAX 3.4028234e38f
#define FLOAT_NEG_MAX -FLOAT_MAX

#define TAU 6.2831855f
#define PI 3.1415927f
#define PI_2 1.5707964f

#define SIGN(x) (((x) < 0) ? -1 : 1)

#define SET_SPRITE_POS(s, _x, _y) \
    (s).x = _x;                   \
    (s).y = _y;
#define SET_SPRITE_POS_PTR(s, _x, _y) \
    (s)->x = _x;                      \
    (s)->y = _y;

#define __CONCAT__(x, y) x##y
#define __CONCAT2__(x, y) __CONCAT__(x, y)

#define __CONCAT_3__(x, y, z) x##y##z
#define __CONCAT2_3__(x, y, z) __CONCAT_3__(x, y, z)

#define __ALIGNER static u8 __CONCAT2__(aligner, __LINE__)[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
#define __ALIGNER2 static u8 __CONCAT2__(aligner, __LINE__)[8];

// The varargs functions in this file align the va_list
// This might be due to a change in the va_list macro from 5.3 to 7.1
// But, until that is known as a fact, only do the alignment when
// building for matching IDO code
#if defined(__sgi) && !defined(NON_MATCHING)
#define VA_LIST_ALIGN(ap, paramN) ((ap) = (va_list) ALIGN((uintptr_t) (ap), sizeof((paramN))))
#else
#define VA_LIST_ALIGN(ap, paramN)
#endif

#endif
