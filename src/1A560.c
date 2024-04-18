#include "common.h"

s32 D_80040F40 = 1;
s32 D_80040F44 = 0;
s32 (*D_80040F48)(u8*, u8*) = NULL;

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

f32 func_80019ABC(f32 y, f32 x) {
    f32 yDivX;
    s32 ySign;

    if (x > 0.0f) {
        return func_80019994(y / x);
    }
    if (x < 0.0f) {
        yDivX = y / x;
        ySign = ((y < 0.0f) ? -1 : 1);

        return (M_PI_F - func_80019994(ABSF(yDivX))) * ySign;
    }
    if (y != 0.0f) {
        return ((y < 0.0f) ? -1 : 1) * PI_2;
    } else {
        return 0.0f;
    }
}

f32 func_80019BBC(f32 x) {
    if (x > 0.99999f) {
        return PI_2;
    }
    if (x < -0.99999f) {
        return -PI_2;
    }

    return func_80019994(x / sqrtf(1.0f - SQ(x)));
}

f32 func_80019C40(f32 x) {
    if (x > 0.99999f) {
        return 0.0f;
    }
    if (x < -0.99999f) {
        return M_PI_F;
    }

    return PI_2 - func_80019994(x / sqrtf(1.0f - SQ(x)));
}

f32 func_80019CD0(f32 x) {
    return 1.0f / sinf(x);
}

f32 func_80019CF8(f32 x) {
    return 1.0f / cosf(x);
}

f32 func_80019D20(f32 x) {
    return 1.0f / func_80019960(x);
}

void func_80019D48(s32 arg0) {
    D_80040F40 = arg0;
}

s32 func_80019D54(void) {
    return D_80040F40;
}

u16 func_80019D60(void) {
    return (D_80040F40 = D_80040F40 * 214013 + 2531011) >> 0x10;
}

f32 func_80019DB0(void) {
    return (((D_80040F40 = D_80040F40 * 214013 + 2531011) >> 0x10) & 0xFFFF) / ((f32)0x10000);
}

u32 func_80019E14(s32 range) {
    return func_80019D60() * range / 0x10000;
}

u8 func_80019E54(void) {
    return osGetTime();
}

f32 func_80019E78(void) {
    return (osGetTime() & 0xFF) / 256.0f;
}

s32 func_80019EB0(s32 range) {
    return (f32)((s32)(osGetTime() & 0xFF) * range) / 256.0f;
}

void func_80019F00(u8* buf1, u8* buf2, s32 len) {
    while (len--) {
        u8 temp = *buf1;

        *buf1++ = *buf2;
        *buf2++ = temp;
    }
}

void func_80019F38(u8* arg0, u8* arg1) {
    u8* s0;
    u8* s1;

    s0 = arg0;
    s1 = arg1 + D_80040F44;

    while (arg0 < arg1) {
    restart:
        do {
            s0 += D_80040F44;
            if (s0 >= arg1) {
                break;
            }
        } while (D_80040F48(s0, arg0) <= 0);

        do {
            s1 -= D_80040F44;
            if (arg0 >= s1) {
                break;
            }
        } while (D_80040F48(s1, arg0) >= 0);

        if (s0 < s1) {
            func_80019F00(s0, s1, D_80040F44);
            goto restart;
        }

        func_80019F00(arg0, s1, D_80040F44);
        if (s1 - arg0 >= arg1 - s1) {
            func_80019F38(s1 + D_80040F44, arg1);
            arg1 = s1 - D_80040F44;
            s0 = arg0;
        } else {
            func_80019F38(arg0, s1 - D_80040F44);
            arg0 = s1 + D_80040F44;
            s0 = arg0;
            s1 = arg1 + D_80040F44;
        }
    }
}

void func_8001A094(u8* arg0, s32 arg1, s32 arg2, s32 (*arg3)(u8*, u8*)) {
    u8 *s2, *s1;
    s32 s0;
    s32 sp30;
    s32 nv = 1; // required to match

    s2 = arg0;
    s1 = arg0 + arg2;
    sp30 = FALSE;

    if (arg1) {
        s0 = arg1 - nv;
        do { // required to match
            while (s0--) {
                if (arg3(s2, s1) > 0) {
                    sp30 = TRUE;
                    break;
                }
                s2 = s1;
                s1 += arg2;
            }
        } while (FALSE);
    }

    if (sp30) {
        D_80040F44 = arg2;
        D_80040F48 = arg3;
        func_80019F38(arg0, arg0 + ((arg1 - 1) * D_80040F44));
    }
}

u8* find(u8* value, u8* data, u32 count, s32 itemSize, s32 (*compare)(u8*, u8*)) {
    s32 newvar = (count - 1) * itemSize;
    u8* low = data;
    u8* high = data + newvar;

    while (low <= high) {
        u32 midIndex = count >> 1;
        u32 parity = count & 1;

        if (midIndex != 0) {
            u8* mid = low + (parity ? midIndex : midIndex - 1) * itemSize;
            s32 ret = compare(value, mid);

            if (ret == 0) {
                return mid;
            } else if (ret < 0) {
                high = mid - itemSize;
                count = parity ? midIndex : midIndex - 1;
            } else {
                low = mid + itemSize;
                count = midIndex;
            }
        } else {
            if (count == 0) {
                break;
            }
            return compare(value, low) ? NULL : low;
        }
    }
    return 0;
}

u8* func_8001A29C(u8* dst, u8* src, u32* numSrcs, u32 lenSrc, s32 (*checker)(u8*, u8*)) {
    u32 i;

    for (i = 0; i < *numSrcs; i++, src += lenSrc) {
        if (checker(dst, src) == 0) {
            return src;
        }
    }

    func_80037660(dst, src, lenSrc);
    *numSrcs += 1;

    return src;
}
