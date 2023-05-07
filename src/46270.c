#include "common.h"

s32 scm_padding[2];
s32 scmD_800B0578;

void func_8009A8C0(s32 arg0) {
    scmD_800B0578 = arg0;
}

s32 func_8009A8CC(void) {
    return scmD_800B0578;
}

#pragma GLOBAL_ASM("asm/nonmatchings/46270/func_8009A8D8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/46270/func_8009A8E4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/46270/func_8009A8F0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/46270/func_8009AE0C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/46270/func_8009B2BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/46270/func_8009B40C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/46270/start_scene_manager.s")
