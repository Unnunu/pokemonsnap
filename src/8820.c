#include "common.h"

extern void (*D_80040D60)(Gfx**);
extern Gfx D_80040DA8[];
extern s16* D_8004A950;
extern s32 D_8004A958;
extern s32 D_8004A95C;
extern s32 D_8004A980[];

void func_80005418(Gfx**);

#pragma GLOBAL_ASM("asm/nonmatchings/8820/func_80007C20.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8820/func_80007CBC.s")
void func_80007CBC(s32*);

void func_80007D08(void (*arg0)(Gfx**)) {
    D_80040D60 = arg0;
}

#ifdef NON_MATCHING
void func_80007D14(Gfx** arg0) {
    Gfx* sp34 = *arg0;
    s32 x = 0;
    s32 y = 0;

    gSPSegment(sp34++, 0x00, 0x00000000);
    func_80005418(&sp34);
    gDPSetDepthImage(sp34++, D_8004A950);
    func_80007CBC(D_8004A980);
    gSPDisplayList(sp34++, D_80040DA8);
    gDPSetScissor(sp34++, G_SC_NON_INTERLACE, x, y, D_8004A958, D_8004A95C);
    if (D_80040D60 != NULL) {
        D_80040D60(&sp34);
    }
    *arg0 = sp34;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/8820/func_80007D14.s")
#endif