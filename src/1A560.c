#include "common.h"

f32 func_80019960(f32 angle) {
    return sinf(angle) / cosf(angle);
}

f32 func_80019994(f32 arg) {
    f32 arg2;
    f32 result;
    s32 phi_v0;

    if (arg == 0.0f) {
        return 0.0f;
    }

    phi_v0 = 1;

    if (arg > 1.0f) {
        arg = 1.0f / arg;
    } else if (arg < -1.0f) {
        arg = 1.0f / arg;
        phi_v0 = 2;
    } else {
        phi_v0 = 0;
    }

    arg2 = arg * arg;
    result =
        (arg2 / (arg2 / (arg2 / (arg2 / (arg2 / ((arg2 / -0.10810675f) + -44.57192f) + -0.1619081f) + -15.774018f) +
                         -0.55556977f) +
                 -3.000003f) +
         1) *
        arg;

    switch (phi_v0) {
        case 0:
            return result;
        case 1:
            return PI_2 - result;
        case 2:
            return -PI_2 - result;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/1A560/func_80019ABC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1A560/func_80019BBC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1A560/func_80019C40.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1A560/func_80019CD0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1A560/func_80019CF8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1A560/func_80019D20.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1A560/func_80019D48.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1A560/func_80019D54.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1A560/func_80019D60.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1A560/func_80019DB0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1A560/func_80019E14.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1A560/func_80019E54.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1A560/func_80019E78.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1A560/func_80019EB0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1A560/func_80019F00.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1A560/func_80019F38.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1A560/func_8001A094.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1A560/func_8001A16C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1A560/func_8001A29C.s")
