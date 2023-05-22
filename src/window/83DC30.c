#include "common.h"

#define PACK_RGBA(r, g, b, a) ((r << 24) | (g << 16) | (b << 8) | a)

typedef struct Window {
    /* 0x00 */ char unk_00[0x10];
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ char unk_18[0x3C - 0x18];
    /* 0x3C */ struct Window* unk3C;
    /* 0x40 */ struct Window* unk40;
} Window; // size >= 0x44

extern Window* D_8037EA8C;
extern Window* D_8037EA90;

#pragma GLOBAL_ASM("asm/nonmatchings/window/83DC30/func_8036A480_83DC30.s")

// link window
void func_8036A4AC_83DC5C(Window* arg0) {
    if (D_8037EA8C == NULL) {
        D_8037EA8C = arg0;
        D_8037EA90 = arg0;
        arg0->unk3C = NULL;
        arg0->unk40 = NULL;
    } else {
        D_8037EA8C->unk40 = arg0;
        arg0->unk3C = D_8037EA8C;
        arg0->unk40 = NULL;
        D_8037EA8C = arg0;
    }
}

void func_8036A4F4_83DCA4(Window* arg0) {
    if (D_8037EA90 == NULL) {
        D_8037EA8C = arg0;
        D_8037EA90 = arg0;
        arg0->unk3C = NULL;
        arg0->unk40 = NULL;
    } else {
        D_8037EA90->unk3C = arg0;
        arg0->unk3C = NULL;
        arg0->unk40 = D_8037EA90;
        D_8037EA90 = arg0;
    }
}

void func_8036A53C_83DCEC(Window* arg0) {
    Window* temp_v0;
    Window* temp_v1;

    temp_v0 = arg0->unk3C;
    if (temp_v0 == NULL && arg0->unk40 == NULL) {
        D_8037EA8C = NULL;
        D_8037EA90 = NULL;
        return;
    }
    temp_v1 = arg0->unk40;
    if (temp_v0 == NULL) {
        D_8037EA90 = temp_v1;
        arg0->unk40->unk3C = NULL;
        return;
    }
    if (temp_v1 == NULL) {
        temp_v0->unk40 = NULL;
        D_8037EA8C = arg0->unk3C;
        return;
    }
    temp_v0->unk40 = temp_v1;
    arg0->unk40->unk3C = arg0->unk3C;
}

#pragma GLOBAL_ASM("asm/nonmatchings/window/83DC30/func_8036A5B8_83DD68.s")
/*void func_8036A5B8_83DD68(Window* arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7,
                          s32 arg8) {
    s32 t0;

    if (arg1 < 0) {
        arg1 = 0;
    } else if (arg1 >= arg0->unk10) {
        arg1 = arg0->unk10 - 1;
    }

    if (arg3 < 0) {
        arg3 = 0;
    } else if (arg3 >= arg0->unk10) {
        arg3 = arg0->unk10 - 1;
    }

    if (arg2 < 0) {
        arg2 = 0;
    } else if (arg2 >= arg0->unk14) {
        arg2 = arg0->unk14 - 1;
    }

    if (arg4 < 0) {
        arg4 = 0;
    } else if (arg4 >= arg0->unk14) {
        arg4 = arg0->unk14 - 1;
    }

    if (arg3 < arg1) {
        s32 tmp = arg1;
        arg1 = arg3;
        arg3 = tmp;
    }

    if (arg4 < arg2) {
        s32 tmp = arg4;
        arg2 = arg4;
        arg4 = tmp;
    }

    t0 = arg4 - arg2 + 1;

}*/

#pragma GLOBAL_ASM("asm/nonmatchings/window/83DC30/func_8036A8E4_83E094.s")

#pragma GLOBAL_ASM("asm/nonmatchings/window/83DC30/func_8036A968_83E118.s")

#pragma GLOBAL_ASM("asm/nonmatchings/window/83DC30/func_8036A9AC_83E15C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/window/83DC30/func_8036AC6C_83E41C.s")
/*
Window* func_8036AC6C_83E41C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    Window* ret;
    s32 sp9C;
    s32 sp98;
    s32 t5;
    s32 t3;
    s32 t1;
    s32 templo;
    s32 t4;
    s32 templo2;
    s32 v1;
    s32 ra;
    s32 a1;
    u8* ptra1;

    if (arg4 & 1) {
        sp9C = 4;
        sp98 = 4;
    } else if (arg4 & 2) {
        sp9C = 8;
        sp98 = 8;
    } else {
        sp9C = 0;
        sp98 = 0;
    }

    if (arg4 & 0x100) {
        t5 = 1;
    } else if (arg4 & 0x400) {
        t5 = 4;
    } else {
        t5 = 2;
    }

    t3 = sp9C * 2 + arg2;
    t1 = ALIGN(t3 + (t5 >> 2) * 0x10 + 0x20, 0x10);
    templo = 0x1000 / t5 / t1;

    t4 = sp98 * 2 + arg3;
    templo2 = (t4 + templo - 1) / templo;
    v1 = ALIGN(templo2 * 0x10, 8);
    ra = ALIGN(t1 * templo * templo2 * t5, 8);

    if (arg4 & 0x80) {
        ra *= 2;
        v1 *= 2;
    }

    a1 = templo2 * 12 + 24;

    ret = mem_alloc(ra + v1 + a1 * 8 + 0x130);
    ptra1 = 
}*/

#pragma GLOBAL_ASM("asm/nonmatchings/window/83DC30/func_8036B5F0_83EDA0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/window/83DC30/func_8036B628_83EDD8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/window/83DC30/func_8036B734_83EEE4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/window/83DC30/func_8036B870_83F020.s")

#pragma GLOBAL_ASM("asm/nonmatchings/window/83DC30/func_8036B988_83F138.s")

#pragma GLOBAL_ASM("asm/nonmatchings/window/83DC30/func_8036B9EC_83F19C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/window/83DC30/func_8036BA50_83F200.s")

#pragma GLOBAL_ASM("asm/nonmatchings/window/83DC30/func_8036C2FC_83FAAC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/window/83DC30/func_8036C898_840048.s")

#pragma GLOBAL_ASM("asm/nonmatchings/window/83DC30/func_8036C9C0_840170.s")

#pragma GLOBAL_ASM("asm/nonmatchings/window/83DC30/func_8036CAE8_840298.s")

#pragma GLOBAL_ASM("asm/nonmatchings/window/83DC30/func_8036CB10_8402C0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/window/83DC30/func_8036CB58_840308.s")

#pragma GLOBAL_ASM("asm/nonmatchings/window/83DC30/func_8036CBA0_840350.s")

#pragma GLOBAL_ASM("asm/nonmatchings/window/83DC30/func_8036CBFC_8403AC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/window/83DC30/func_8036CC48_8403F8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/window/83DC30/func_8036CCEC_84049C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/window/83DC30/func_8036CDAC_84055C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/window/83DC30/func_8036D018_8407C8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/window/83DC30/func_8036D024_8407D4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/window/83DC30/func_8036D040_8407F0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/window/83DC30/func_8036D09C_84084C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/window/83DC30/func_8036D0F8_8408A8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/window/83DC30/func_8036D1A4_840954.s")

#pragma GLOBAL_ASM("asm/nonmatchings/window/83DC30/func_8036D22C_8409DC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/window/83DC30/func_8036D248_8409F8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/window/83DC30/func_8036D28C_840A3C.s")
