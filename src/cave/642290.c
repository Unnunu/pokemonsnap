#include "common.h"
#include "world/world.h"
#include "app_level/app_level.h"

extern PokemonInitData D_802C6C64_649114;
extern AnimationHeader D_802C6BA4_649054;
extern InteractionHandler D_802C6BE0_649090;
extern AnimationHeader D_802C6BCC_64907C;

#pragma GLOBAL_ASM("asm/nonmatchings/cave/642290/func_802BFDE0_642290.s")

#pragma GLOBAL_ASM("asm/nonmatchings/cave/642290/func_802BFE34_6422E4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/cave/642290/func_802BFEF0_6423A0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/cave/642290/func_802BFF74_642424.s")

void func_802C0058_642508(GObj* obj) {
    UNUSED s32 pad[3];
    Pokemon* pokemon = GET_POKEMON(obj);

    Pokemon_SetAnimation(obj, &D_802C6BA4_649054);
    pokemon->transitionGraph = &D_802C6BE0_649090;
    Pokemon_WaitForFlag(obj, 0);
    Pokemon_SetState(obj, NULL);
}

void func_802C00B0_642560(GObj* obj) {
    UNUSED s32 pad[3];
    Pokemon* pokemon = GET_POKEMON(obj);

    Pokemon_SetAnimation(obj, &D_802C6BCC_64907C);
    pokemon->transitionGraph = NULL;
    Pokemon_WaitForFlag(obj, 1);
    Pokemon_SetState(obj, func_802C0058_642508);
}

GObj* func_802C0104_6425B4(s32 objID, u16 id, WorldBlock* block, WorldBlock* blockB, ObjectSpawn* spawn, PokemonInitData* initData) {
    return Pokemon_SpawnOnGround(objID, id, block, blockB, spawn, &D_802C6C64_649114);
}
