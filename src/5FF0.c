#include "common.h"

typedef struct {
    /* 0x00 */ s32 capacity;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s32 unk_0C;
} Struct_800078C4; // size = 0x10

typedef struct {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s32 unk_0C;
} Struct_8004A8B8; // size = 0x10

typedef struct {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
} Struct_8004A850; // size = 0x08

extern u32* D_800497C8;
extern Struct_8004A850 D_8004A850[];
extern Struct_8004A8B8 D_8004A8B8;
extern Struct_800078C4 D_8004A8C8;
extern s32 D_8004A900;
extern u16 D_8004A904;
extern u16 D_8004A906;
extern s32 D_8004A910;
extern Struct_8004A8B8 D_8004A928[];
extern void* D_8004A94C;


void func_80000A64(void*);
void func_800078C4(Struct_800078C4*, s32, s32, s32);
void func_8000785C(Struct_800078C4*, s32, s32);
void func_80007850(Struct_8004A8B8*);

void func_800053F0(void* arg0) {
    if (arg0 != NULL) {
        D_8004A94C = arg0;
        return;
    } else {
        D_8004A94C = func_80000A64;
    }
}

void func_80005418(Gfx** gfxPtr) {
    D_800497C8 = &(*gfxPtr)->dma.addr;
    gSPSegment((*gfxPtr)++, 0x0F, 0x00000000);
}

void func_80005448(s32 arg0) {
    D_8004A900 = arg0;
}

void func_80005454(u16 arg0, u16 arg1) {
    D_8004A904 = arg0;
    D_8004A906 = arg1;
}

void func_80005470(s32 arg0, s32 size) {
    func_800078C4(&D_8004A8C8, 0x10000, arg0, size);
}

void hal_alloc(s32 arg0, s32 arg1) {
    func_8000785C(&D_8004A8C8, arg0, arg1);
}

void func_800054D0(void) {
    D_8004A8B8.unk_00 = D_8004A928[D_8004A910].unk_00;
    D_8004A8B8.unk_04 = D_8004A928[D_8004A910].unk_04;
    D_8004A8B8.unk_08 = D_8004A928[D_8004A910].unk_08;
    D_8004A8B8.unk_0C = D_8004A928[D_8004A910].unk_0C;
    func_80007850(&D_8004A8B8);
}

void func_8000552C(Struct_8004A850* arg0) {
    s32 i;

    for (i = 0; i < 8; i++) {
        D_8004A850[i] = arg0[i];
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/5FF0/func_800055D0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/5FF0/func_800056BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/5FF0/func_8000577C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/5FF0/func_800057D4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/5FF0/func_8000584C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/5FF0/func_800058D4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/5FF0/func_800059B0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/5FF0/func_80005A1C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/5FF0/func_80005A98.s")

#pragma GLOBAL_ASM("asm/nonmatchings/5FF0/func_80005B38.s")

#pragma GLOBAL_ASM("asm/nonmatchings/5FF0/func_80005D04.s")

#pragma GLOBAL_ASM("asm/nonmatchings/5FF0/func_80005D60.s")

#pragma GLOBAL_ASM("asm/nonmatchings/5FF0/func_80005E64.s")

#pragma GLOBAL_ASM("asm/nonmatchings/5FF0/func_80005F34.s")

#pragma GLOBAL_ASM("asm/nonmatchings/5FF0/func_80006330.s")

#pragma GLOBAL_ASM("asm/nonmatchings/5FF0/func_8000664C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/5FF0/func_80006764.s")

#pragma GLOBAL_ASM("asm/nonmatchings/5FF0/func_800067DC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/5FF0/func_800067EC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/5FF0/func_80006804.s")

#pragma GLOBAL_ASM("asm/nonmatchings/5FF0/func_80006878.s")

#pragma GLOBAL_ASM("asm/nonmatchings/5FF0/func_80006908.s")

#pragma GLOBAL_ASM("asm/nonmatchings/5FF0/func_80006E24.s")

#pragma GLOBAL_ASM("asm/nonmatchings/5FF0/func_80006E5C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/5FF0/func_80006EC0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/5FF0/func_80006F10.s")

#pragma GLOBAL_ASM("asm/nonmatchings/5FF0/func_80006F8C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/5FF0/func_800070B8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/5FF0/func_80007354.s")

#pragma GLOBAL_ASM("asm/nonmatchings/5FF0/func_800073AC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/5FF0/func_800075FC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/5FF0/func_80007618.s")

#pragma GLOBAL_ASM("asm/nonmatchings/5FF0/func_80007640.s")

#pragma GLOBAL_ASM("asm/nonmatchings/5FF0/func_80007668.s")

#pragma GLOBAL_ASM("asm/nonmatchings/5FF0/func_80007694.s")

#pragma GLOBAL_ASM("asm/nonmatchings/5FF0/func_800076B4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/5FF0/func_800076F0.s")
