#include "common.h"

#include "sys/interpolation.h"

#pragma GLOBAL_ASM("asm/nonmatchings/1F960/func_8001ED60.s")

void func_8001EEA4(Vec3f* arg0, Vec3f* arg1, f32 arg2, f32 arg3) {
    f32 temp_f0;
    f32 temp_f16;
    f32 temp_f18;
    f32 temp_f2;
    f32 temp_f8;
    f32 other;

    temp_f0 = arg3 * arg3;
    temp_f2 = ((((-3.0f) * temp_f0) + (4.0f * arg3)) - 1.0f) * arg2;
    temp_f8 = arg2 - 3.0f;
    other = arg2;
    temp_f16 = (((2.0f - other) * 3.0f) * temp_f0) + ((2.0f * temp_f8) * arg3);
    temp_f8 = 3.0f - (2.0f * other);
    temp_f18 = ((((other - 2.0f) * 3.0f) * temp_f0) + ((2.0f * temp_f8) * arg3)) + other;
    other = ((3.0f * temp_f0) - (2.0f * arg3)) * other;

    arg0->x = (arg1->x * temp_f2) + (arg1[1].x * temp_f16) + (arg1[2].x * temp_f18) + (arg1[3].x * other);
    arg0->y = (arg1->y * temp_f2) + (arg1[1].y * temp_f16) + (arg1[2].y * temp_f18) + (arg1[3].y * other);
    arg0->z = (arg1->z * temp_f2) + (arg1[1].z * temp_f16) + (arg1[2].z * temp_f18) + (arg1[3].z * other);
}

#pragma GLOBAL_ASM("asm/nonmatchings/1F960/func_8001F030.s")

void func_8001F190(Vec3f* arg0, Vec3f* arg1, f32 arg2) {
    UNUSED s32 pad[2];
    f32 temp_f0;
    f32 temp_f14;
    f32 temp_f16;
    f32 temp_f18;
    f32 temp_f2;
    f32 temp_f8;

    temp_f0 = arg2 * arg2;
    temp_f8 = 1.0f - arg2;
    temp_f2 = -0.5f * temp_f8 * temp_f8;
    temp_f14 = ((3.0f * temp_f0) - (4.0f * arg2)) * 0.5f;
    temp_f16 = ((-3.0f * temp_f0) + (2.0f * arg2) + 1.0f) * 0.5f;
    temp_f18 = 0.5f * temp_f0;

    arg0->x = (arg1->x * temp_f2) + (arg1[1].x * temp_f14) + (arg1[2].x * temp_f16) + (arg1[3].x * temp_f18);
    arg0->y = (arg1->y * temp_f2) + (arg1[1].y * temp_f14) + (arg1[2].y * temp_f16) + (arg1[3].y * temp_f18);
    arg0->z = (arg1->z * temp_f2) + (arg1[1].z * temp_f14) + (arg1[2].z * temp_f16) + (arg1[3].z * temp_f18);
}

#pragma GLOBAL_ASM("asm/nonmatchings/1F960/func_8001F2C4.s")

void func_8001F3D8(Vec3f* arg0, Vec3f* arg1, f32 arg2) {
    f32 temp_f0;
    f32 temp_f14;
    f32 temp_f16;
    f32 temp_f18;
    f32 temp_f2;

    temp_f0 = arg2 - 1.0f;
    temp_f2 = -3.0f * temp_f0 * temp_f0;
    temp_f14 = SQ(arg2);
    temp_f14 = 3.0f * temp_f14;
    temp_f16 = ((1.0f - (4.0f * arg2)) + temp_f14) * 3.0f;
    temp_f18 = ((2.0f * arg2) - temp_f14) * 3.0f;

    arg0->x = (arg1->x * temp_f2) + (arg1[1].x * temp_f16) + (arg1[2].x * temp_f18) + (arg1[3].x * temp_f14);
    arg0->y = (arg1->y * temp_f2) + (arg1[1].y * temp_f16) + (arg1[2].y * temp_f18) + (arg1[3].y * temp_f14);
    arg0->z = (arg1->z * temp_f2) + (arg1[1].z * temp_f16) + (arg1[2].z * temp_f18) + (arg1[3].z * temp_f14);
}

void func_8001F4E4(Vec3f*, InterpData*, f32);
#pragma GLOBAL_ASM("asm/nonmatchings/1F960/func_8001F4E4.s")

#ifndef NON_MATCHING
void func_8001F770(Vec3f*, InterpData*, f32);
#pragma GLOBAL_ASM("asm/nonmatchings/1F960/func_8001F770.s")
#else
void func_8001F770(Vec3f* arg0, InterpData* arg1, f32 arg2) {
    Vec3f* temp_v0;
    s16 idx;

    if (!(arg2 < 0.0f) && !(arg2 > 1.0f)) {
        arg2 *= arg1->unk_02 - 1;
        idx = arg2;
        arg2 = arg2 - idx;
        switch (arg1->type) {
            case 0:
                if (arg2 == 1.0f) {
                    idx--;
                }
                temp_v0 = &arg1->unk_08[idx];
                arg0->x = temp_v0[1].x - temp_v0[0].x;
                arg0->y = temp_v0[1].y - temp_v0[0].y;
                arg0->z = temp_v0[1].z - temp_v0[0].z;
                break;
            case 1:
                func_8001F3D8(arg0, &arg1->unk_08[idx * 3], arg2);
                break;
            case 2:
                func_8001F190(arg0, &arg1->unk_08[idx], arg2);
                break;
            case 3:
                func_8001EEA4(arg0, &arg1->unk_08[idx], arg1->unk_04, arg2);
                break;
        }
    }
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/1F960/func_8001F8F8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1F960/func_8001F990.s")

f32 func_8001FA94(InterpData*, f32);
#pragma GLOBAL_ASM("asm/nonmatchings/1F960/func_8001FA94.s")

void func_8001FCA4(Vec3f* arg0, InterpData* arg1, f32 arg2) {
    f32 thing = func_8001FA94(arg1, arg2);

    func_8001F4E4(arg0, arg1, thing);
}

void func_8001FCE8(Vec3f* arg0, InterpData* arg1, f32 arg2) {
    func_8001F770(arg0, arg1, func_8001FA94(arg1, arg2));
}
