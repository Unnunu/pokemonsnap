#ifndef SYS_VI_H
#define SYS_VI_H

#include <PR/ultratypes.h>

typedef struct {
    /* 0x00 */ void* fb1;
    /* 0x04 */ void* fb2;
    /* 0x08 */ void* fb3;
    /* 0x0C */ u16* zBuffer;
    /* 0x10 */ u32 screenWidth;
    /* 0x14 */ u32 screenHeight;
    /* 0x18 */ u32 flags;
} ScreenSettings; // size >= 0x18

extern u16* viZBuffer;
extern s32 viScreenWidth;
extern s32 viScreenHeight;
extern s32 viBitDepth;

void vi_apply_screen_settings(ScreenSettings* settings);
void vi_set_screen_offsets(s16, s16, s16, s16);
u32 func_80007910(u32 color);

#endif /* SYS_VI_H */