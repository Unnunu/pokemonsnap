#include "common.h"

#pragma GLOBAL_ASM("asm/nonmatchings/1F960/func_8001ED60.s")
/*
#define SQ(x) ((x) * (x))
#define CB(x) ((x) * (x) * (x))
// biquadrate lol; easier to make a symbol than quartic (QT looks familiar too me)
#define BQ(x) ((x) * (x) * (x) * (x))
#define BASIS1(t, s) ((-CB((t)) + 2.0f * SQ((t)) - (t)) * (s))
#define BASIS2(t, s) ((2.0f - (s)) * CB((t)) + ((s)-3.0f) * SQ((t)) + 1.0f)
#define BASIS3(t, s) (((s)-2.0f) * CB((t)) + (3.0f - 2.0f * (s)) * SQ((t)) + (s) * (t))
#define BASIS4(t, s) ((CB((t)) - SQ((t))) * (s))

void func_8001ED60(Vec3f* out, Vec3f* ctrl, f32 s, f32 t) {
    f32 newvar;
    f32 b1 = BASIS1(t, s);
    f32 b2 = BASIS2(t, s);
    f32 b3 = BASIS3(t, s);
    f32 b4 = BASIS4(t, s);

    out->x = ctrl[0].x * BASIS1(t, s) + ctrl[1].x * BASIS2(t, s) + ctrl[2].x * BASIS3(t, s) + ctrl[3].x * BASIS4(t, s);

    out->y = ctrl[0].y * BASIS1(t, s) + ctrl[1].y * BASIS2(t, s) + ctrl[2].y * BASIS3(t, s) + ctrl[3].y * BASIS4(t, s);

    out->z = ctrl[0].z * BASIS1(t, s) + ctrl[1].z * BASIS2(t, s) + ctrl[2].z * BASIS3(t, s) + ctrl[3].z * BASIS4(t, s);
}
*/

#pragma GLOBAL_ASM("asm/nonmatchings/1F960/func_8001EEA4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1F960/func_8001F030.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1F960/func_8001F190.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1F960/func_8001F2C4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1F960/func_8001F3D8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1F960/func_8001F4E4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1F960/func_8001F770.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1F960/func_8001F8F8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1F960/func_8001F990.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1F960/func_8001FA94.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1F960/func_8001FCA4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1F960/func_8001FCE8.s")
