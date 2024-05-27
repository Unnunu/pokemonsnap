#include "common.h"
#include "world/world.h"
#include "app_level/app_level.h"

void func_802BF044_6414F4(GObj*);

extern PokemonInitData D_802C69C0_648E70;

void func_802BF020_6414D0(GObj* arg0) {
    Pokemon_SetState(arg0, func_802BF044_6414F4);
}

#pragma GLOBAL_ASM("asm/nonmatchings/cave/6414D0/func_802BF044_6414F4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/cave/6414D0/func_802BF0B4_641564.s")

#pragma GLOBAL_ASM("asm/nonmatchings/cave/6414D0/func_802BF124_6415D4.s")

void func_802BF1C4_641674(GObj* obj) {
    UNUSED s32 pad[3];
    Pokemon* pokemon = GET_POKEMON(obj);

    pokemon->hSpeed = 20.0f;
    Pokemon_RunInCircles(obj, 500.0f, 0.1f, 1);
    pokemon->pathProc = NULL;
    pokemon->processFlags |= POKEMON_PROCESS_FLAG_PATH_ENDED;
    omEndProcess(NULL);
}

#pragma GLOBAL_ASM("asm/nonmatchings/cave/6414D0/func_802BF220_6416D0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/cave/6414D0/func_802BF2D0_641780.s")

#pragma GLOBAL_ASM("asm/nonmatchings/cave/6414D0/func_802BF338_6417E8.s")

extern AnimationHeader D_802C6898_648D48;

void func_802BF3D0_641880(GObj* obj) {
    UNUSED s32 pad[3];
    Pokemon* pokemon = GET_POKEMON(obj);

    Pokemon_TurnToTarget(obj, 0.1f, 0);
    Pokemon_SetAnimation(obj, &D_802C6898_648D48);
    pokemon->hSpeed = 80.0f;
    Pokemon_RunToTarget(obj, 50.0f, 0.1f, 3);
    pokemon->pathProc = NULL;
    pokemon->processFlags |= 2;
    omEndProcess(NULL);
}

void func_802BF4E8_641998(GObj*);
extern AnimationHeader D_802C68FC_648DAC;
extern RandomState D_802C6964_648E14;
extern InteractionHandler D_802C697C_648E2C;

void func_802BF454_641904(GObj* obj) {
    UNUSED s32 pad[3];
    Pokemon* pokemon = GET_POKEMON(obj);

    Pokemon_SetAnimation(obj, &D_802C68FC_648DAC);
    Pokemon_StartPathProc(obj, func_802BF4E8_641998);
    pokemon->transitionGraph = &D_802C697C_648E2C;
    Pokemon_WaitForFlag(obj, 1);
    if (pokemon->interactionTarget != NULL) {
        Pokemon_SetState(obj, func_802BF454_641904);
    }
    Pokemon_SetStateRandom(obj, &D_802C6964_648E14);
}

#pragma GLOBAL_ASM("asm/nonmatchings/cave/6414D0/func_802BF4E8_641998.s")

void func_802BF554_641A04(void) {
}

GObj* func_802BF55C_641A0C(s32 objID, u16 id, WorldBlock* block, WorldBlock* blockB, ObjectSpawn* spawn, PokemonInitData* initData) {
    return Pokemon_SpawnOnGround(objID, id, block, blockB, spawn, &D_802C69C0_648E70);
}
