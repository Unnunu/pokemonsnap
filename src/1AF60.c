#include "common.h"

f32 func_80019C40(f32);

enum VectorAxisFlags { AXIS_X = 1 << 0, AXIS_Y = 1 << 1, AXIS_Z = 1 << 2 };

#define VEC_DOT(v, w) ((v->x * w->x) + (v->y * w->y) + (v->z * w->z))

f32 func_8001A360(Vec3f* v) {
    f32 mag = sqrtf(SQ(v->x) + SQ(v->y) + SQ(v->z));
    if (mag == 0.0f) {
        return 0.0f;
    }
    v->x *= 1.0f / mag;
    v->y *= 1.0f / mag;
    v->z *= 1.0f / mag;

    return mag;
}

f32 func_8001A3FC(const Vec3f* v) {
    return sqrtf(SQ(v->x) + SQ(v->y) + SQ(v->z));
}

Vec3f* func_8001A43C(Vec3f* v1, const Vec3f* v2) {
    v1->x += v2->x;
    v1->y += v2->y;
    v1->z += v2->z;
    return v1;
}

Vec3f* func_8001A474(Vec3f* v1, const Vec3f* v2) {
    v1->x -= v2->x;
    v1->y -= v2->y;
    v1->z -= v2->z;
    return v1;
}

Vec3f* func_8001A4AC(Vec3f* out, const Vec3f* v1, const Vec3f* v2) {
    out->x = v1->x - v2->x;
    out->y = v1->y - v2->y;
    out->z = v1->z - v2->z;
    return out;
}

f32 func_8001A4E4(Vec3f* out, const Vec3f* v1, const Vec3f* v2) {
    out->x = v1->x - v2->x;
    out->y = v1->y - v2->y;
    out->z = v1->z - v2->z;
    return func_8001A360(out);
}

Vec3f* func_8001A530(Vec3f* v, f32 scale) {
    v->x *= scale;
    v->y *= scale;
    v->z *= scale;
    return v;
}

Vec3f* func_8001A564(Vec3f* v, f32 invScale) {
    if (invScale != 0.0f) {
        v->x /= invScale;
        v->y /= invScale;
        v->z /= invScale;
    }
    return v;
}

Vec3f* func_8001A5AC(Vec3f* v1, const Vec3f* v2, f32 scale) {
    v1->x += (v2->x * scale);
    v1->y += (v2->y * scale);
    v1->z += (v2->z * scale);

    return v1;
}

f32 func_8001A5F4(Vec3f* v1, f32 v1Scale, const Vec3f* v2, f32 v2Scale) {
    v1->x *= v1Scale;
    v1->y *= v1Scale;
    v1->z *= v1Scale;

    v1->x += (v2->x * v2Scale);
    v1->y += (v2->y * v2Scale);
    v1->z += (v2->z * v2Scale);

    return func_8001A360(v1);
}

f32 func_8001A67C(Vec3f* v1, Vec3f* v2) {
    f32 magProd = func_8001A3FC(v1) * func_8001A3FC(v2);

    if (magProd != 0.0f) {
        f32 cosDiff = VEC_DOT(v1, v2) / magProd;
        // limit result to -1 < x < 1
        if (cosDiff > 1.0f) {
            cosDiff = 1.0f;
        }
        if (cosDiff < -1.0f) {
            cosDiff = -1.0f;
        }
        return func_80019C40(cosDiff);
    }

    // at least one of the vectors is a zero vector
    return 0.0f;
}

Vec3f* func_8001A750(Vec3f* v, enum VectorAxisFlags axis, f32 angle) {
    f32 xResult, yResult, zResult;
    f32 sinAngle = sinf(angle);
    f32 cosAngle = cosf(angle);

    switch (axis) {
        case AXIS_X:
            xResult = v->x;
            yResult = (v->y * cosAngle) - (v->z * sinAngle);
            zResult = (v->y * sinAngle) + (v->z * cosAngle);
            break;
        case AXIS_Y:
            xResult = (v->x * cosAngle) + (v->z * sinAngle);
            yResult = v->y;
            zResult = (v->z * cosAngle) - (v->x * sinAngle);
            break;
        case AXIS_Z:
            xResult = (v->x * cosAngle) - (v->y * sinAngle);
            yResult = (v->x * sinAngle) + (v->y * cosAngle);
            zResult = v->z;
            break;
    }
    v->x = xResult;
    v->y = yResult;
    v->z = zResult;
    return v;
}

Vec3f* func_8001A88C(Vec3f* v) {
    v->x = -v->x;
    v->y = -v->y;
    v->z = -v->z;

    return v;
}

#pragma GLOBAL_ASM("asm/nonmatchings/1AF60/func_8001A8B8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1AF60/func_8001AA40.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1AF60/func_8001AA88.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1AF60/func_8001AC98.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1AF60/func_8001AD08.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1AF60/func_8001AD94.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1AF60/func_8001AE00.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1AF60/func_8001AEAC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1AF60/func_8001AF30.s")

#pragma GLOBAL_ASM("asm/nonmatchings/1AF60/func_8001AF80.s")
