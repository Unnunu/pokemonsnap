#ifndef _UNKNOWN_STRUCTS_H
#define _UNKNOWN_STRUCTS_H

#include "ultra64.h"

typedef struct {
    /* 0x00 */ char unk_00[0x10];
    /* 0x10 */ s16 unk_10;
    /* 0x12 */ s16 unk_12;
    /* 0x14 */ char unk_14[0x10];
    /* 0x24 */ u16 unk_24;
    /* 0x26 */ char unk_26[2];
    /* 0x28 */ u8 unk_28;
    /* 0x29 */ u8 unk_29;
    /* 0x2A */ u8 unk_2A;
    /* 0x2B */ u8 unk_2B;
    /* 0x2C */ char unk_2C[0x54];
    /* 0x80 */ s32 unk_80;
} Struct_8000C37C_Sub24; // size >= 0x2B

// probably the same as GObj, but not 100% sure
typedef struct {
    /* 0x00 */ char unk_00[0x48];
    /* 0x48 */ Struct_8000C37C_Sub24* unk_48;
} Struct_8000C37C; // size >= 0x4C

#endif
