#include "common.h"

#include "world/world.h"
#include "app_level/app_level.h"

void func_802C4A4C_646EFC(GObj* obj);

extern GObj* D_802C7C3C_64A0EC;
extern PokemonInitData D_802C7CCC_64A17C;

extern AnimationHeader D_802C7BD0_64A080;
extern InteractionHandler D_802C7C40_64A0F0;

void func_802C49F0_646EA0(GObj* obj) {
    UNUSED s32 pad[3];
    Pokemon* pokemon = GET_POKEMON(obj);

    Pokemon_SetAnimation(obj, &D_802C7BD0_64A080);
    pokemon->transitionGraph = &D_802C7C40_64A0F0;
    Pokemon_WaitForFlag(obj, 1);
    Pokemon_SetState(obj, func_802C4A4C_646EFC);
}

extern AnimationHeader D_802C7BD0_64A080;
extern InteractionHandler D_802C7C40_64A0F0;

void func_802C4A4C_646EFC(GObj* obj) {
    UNUSED s32 pad[3];
    Pokemon* pokemon = GET_POKEMON(obj);

    Pokemon_SetAnimation(obj, &D_802C7BD0_64A080);
    pokemon->transitionGraph = &D_802C7C40_64A0F0;
    Pokemon_WaitForFlag(obj, 1);
    Pokemon_SetState(obj, func_802C4A4C_646EFC);
}

extern AnimationHeader D_802C7C0C_64A0BC;
extern InteractionHandler D_802C7C40_64A0F0;

void func_802C4AA8_646F58(GObj* obj) {
    UNUSED s32 pad[3];
    Pokemon* pokemon = GET_POKEMON(obj);

    Pokemon_SetAnimation(obj, &D_802C7C0C_64A0BC);
    pokemon->transitionGraph = &D_802C7C40_64A0F0;
    Pokemon_WaitForFlag(obj, 1);
    Pokemon_SetState(obj, func_802C4A4C_646EFC);
}

#pragma GLOBAL_ASM("asm/nonmatchings/cave/646EA0/func_802C4B04_646FB4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/cave/646EA0/func_802C4C18_6470C8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/cave/646EA0/func_802C4C70_647120.s")

#pragma GLOBAL_ASM("asm/nonmatchings/cave/646EA0/func_802C4D60_647210.s")

GObj* func_802C4E64_647314(s32 gObjID, u16 id, WorldBlock* roomA, WorldBlock* roomB, ObjectSpawn* spawn) {
    D_802C7C3C_64A0EC = Pokemon_SpawnOnGround(gObjID, id, roomA, roomB, spawn, &D_802C7CCC_64A17C);

    return D_802C7C3C_64A0EC;
}
