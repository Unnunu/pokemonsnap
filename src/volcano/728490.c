#include "common.h"
#include "world/world.h"
#include "app_level/app_level.h"

void func_802D78A4_728AA4(GObj*);
void func_802D8368_729568(GObj*);
void func_802D85C0_7297C0(GObj*);
void func_802D8830_729A30(GObj*);
void func_802D8868_729A68(GObj*);
void func_802D88D0_729AD0(GObj*);

extern AnimationHeader D_802E0FEC_7321EC;
extern InteractionHandler D_802E10C4_7322C4[];
extern RandomState D_802E1484_732684[];
extern PokemonInitData D_802E14D8_7326D8;
extern AnimationHeader D_802E1540_732740;
extern AnimationHeader D_802E157C_73277C;
extern AnimationHeader D_802E15A4_7327A4;
extern InteractionHandler D_802E15B8_7327B8[];
extern InteractionHandler D_802E1718_732918[];
extern RandomState D_802E17F8_7329F8[];
extern InteractionHandler D_802E1818_732A18[];
extern PokemonInitData D_802E190C_732B0C;

#pragma GLOBAL_ASM("asm/nonmatchings/volcano/728490/func_802D7290_728490.s")

#pragma GLOBAL_ASM("asm/nonmatchings/volcano/728490/func_802D73A8_7285A8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/volcano/728490/func_802D7458_728658.s")

#pragma GLOBAL_ASM("asm/nonmatchings/volcano/728490/func_802D7510_728710.s")

void func_802D7584_728784(GObj* obj) {
    UNUSED s32 pad[3];
    Pokemon* pokemon = GET_POKEMON(obj);

    Pokemon_ForceAnimation(obj, &D_802E0FEC_7321EC);
    Pokemon_StartPathProc(obj, NULL);
    pokemon->transitionGraph = D_802E10C4_7322C4;
    Pokemon_WaitForFlag(obj, POKEMON_PROCESS_FLAG_ANIMATION_ENDED);
    Pokemon_SetStateRandom(obj, D_802E1484_732684);
}

#pragma GLOBAL_ASM("asm/nonmatchings/volcano/728490/func_802D75F4_7287F4.s")

void func_802D7698_728898(GObj* obj) {
    UNUSED s32 pad[3];
    Pokemon* pokemon = GET_POKEMON(obj);

    pokemon->hSpeed = 40.0f;
    Pokemon_RunInCircles(obj, 500.0f, 0.1f, 1);
    pokemon->pathProc = NULL;
    pokemon->processFlags |= POKEMON_PROCESS_FLAG_PATH_ENDED;
    omEndProcess(NULL);
}

#pragma GLOBAL_ASM("asm/nonmatchings/volcano/728490/func_802D76F4_7288F4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/volcano/728490/func_802D77BC_7289BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/volcano/728490/func_802D7830_728A30.s")

#pragma GLOBAL_ASM("asm/nonmatchings/volcano/728490/func_802D78A4_728AA4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/volcano/728490/func_802D798C_728B8C.s")

void func_802D7A10_728C10(GObj* arg0) {
    Pokemon_SetState(arg0, func_802D78A4_728AA4);
}

#pragma GLOBAL_ASM("asm/nonmatchings/volcano/728490/func_802D7A34_728C34.s")

#pragma GLOBAL_ASM("asm/nonmatchings/volcano/728490/func_802D7A94_728C94.s")

#pragma GLOBAL_ASM("asm/nonmatchings/volcano/728490/func_802D7B08_728D08.s")

#pragma GLOBAL_ASM("asm/nonmatchings/volcano/728490/func_802D7B54_728D54.s")

#pragma GLOBAL_ASM("asm/nonmatchings/volcano/728490/func_802D7C30_728E30.s")

#pragma GLOBAL_ASM("asm/nonmatchings/volcano/728490/func_802D7CB4_728EB4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/volcano/728490/func_802D7D84_728F84.s")

#pragma GLOBAL_ASM("asm/nonmatchings/volcano/728490/func_802D7E90_729090.s")

GObj* func_802D7F28_729128(s32 objID, u16 id, WorldBlock* block, WorldBlock* blockB, ObjectSpawn* spawn, PokemonInitData* initData) {
    return Pokemon_SpawnOnGround(objID, id, block, blockB, spawn, &D_802E14D8_7326D8);
}

#pragma GLOBAL_ASM("asm/nonmatchings/volcano/728490/func_802D7F60_729160.s")

void func_802D7FA8_7291A8(GObj* obj) {
    UNUSED s32 pad[3];
    Pokemon* pokemon = GET_POKEMON(obj);

    Pokemon_SetAnimation(obj, &D_802E1540_732740);
    Pokemon_StartPathProc(obj, NULL);
    pokemon->transitionGraph = D_802E15B8_7327B8;
    Pokemon_WaitForFlag(obj, POKEMON_PROCESS_FLAG_ANIMATION_ENDED);
    Pokemon_SetStateRandom(obj, D_802E17F8_7329F8);
}

#pragma GLOBAL_ASM("asm/nonmatchings/volcano/728490/func_802D8018_729218.s")

void func_802D80BC_7292BC(GObj* obj) {
    UNUSED s32 pad[3];
    Pokemon* pokemon = GET_POKEMON(obj);

    pokemon->hSpeed = 40.0f;
    Pokemon_RunInCircles(obj, 500.0f, 0.1f, 1);
    pokemon->pathProc = NULL;
    pokemon->processFlags |= POKEMON_PROCESS_FLAG_PATH_ENDED;
    omEndProcess(NULL);
}

#pragma GLOBAL_ASM("asm/nonmatchings/volcano/728490/func_802D8118_729318.s")

void func_802D81BC_7293BC(GObj* obj) {
    UNUSED s32 pad[3];
    Pokemon* pokemon = GET_POKEMON(obj);

    pokemon->hSpeed = 160.0f;
    Pokemon_RunInCircles(obj, 500.0f, 0.1f, 1);
    pokemon->pathProc = NULL;
    pokemon->processFlags |= POKEMON_PROCESS_FLAG_PATH_ENDED;
    omEndProcess(NULL);
}

void func_802D8218_729418(GObj* obj) {
    UNUSED s32 pad[3];
    Pokemon* pokemon = GET_POKEMON(obj);

    Pokemon_SetAnimation(obj, &D_802E157C_73277C);
    Pokemon_StartPathProc(obj, NULL);
    pokemon->transitionGraph = D_802E1718_732918;
    Pokemon_WaitForFlag(obj, POKEMON_PROCESS_FLAG_ANIMATION_ENDED);
    Pokemon_SetState(obj, func_802D85C0_7297C0);
}

void func_802D8288_729488(GObj* obj) {
    UNUSED s32 pad[3];
    Pokemon* pokemon = GET_POKEMON(obj);

    Pokemon_SetAnimation(obj, &D_802E157C_73277C);
    Pokemon_StartPathProc(obj, NULL);
    pokemon->transitionGraph = D_802E1718_732918;
    Pokemon_WaitForFlag(obj, POKEMON_PROCESS_FLAG_ANIMATION_ENDED);
    Pokemon_SetState(obj, func_802D85C0_7297C0);
}

void func_802D82F8_7294F8(GObj* obj) {
    UNUSED s32 pad[3];
    Pokemon* pokemon = GET_POKEMON(obj);

    Pokemon_ForceAnimation(obj, &D_802E15A4_7327A4);
    Pokemon_StartPathProc(obj, NULL);
    pokemon->transitionGraph = D_802E1718_732918;
    Pokemon_WaitForFlag(obj, POKEMON_PROCESS_FLAG_ANIMATION_ENDED);
    Pokemon_SetState(obj, func_802D7FA8_7291A8);
}

#pragma GLOBAL_ASM("asm/nonmatchings/volcano/728490/func_802D8368_729568.s")

#pragma GLOBAL_ASM("asm/nonmatchings/volcano/728490/func_802D842C_72962C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/volcano/728490/func_802D8478_729678.s")

#pragma GLOBAL_ASM("asm/nonmatchings/volcano/728490/func_802D84FC_7296FC.s")

void func_802D859C_72979C(GObj* arg0) {
    Pokemon_SetState(arg0, func_802D8368_729568);
}

#pragma GLOBAL_ASM("asm/nonmatchings/volcano/728490/func_802D85C0_7297C0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/volcano/728490/func_802D8620_729820.s")

#pragma GLOBAL_ASM("asm/nonmatchings/volcano/728490/func_802D8694_729894.s")

void func_802D86E0_7298E0(GObj* obj) {
    UNUSED s32 pad[3];
    Pokemon* pokemon = GET_POKEMON(obj);

    Pokemon_SetAnimation(obj, &D_802E1540_732740);
    Pokemon_StartPathProc(obj, NULL);
    pokemon->transitionGraph = D_802E1818_732A18;
    Pokemon_WaitForFlag(obj, POKEMON_PROCESS_FLAG_ANIMATION_ENDED);
    Pokemon_SetState(obj, func_802D86E0_7298E0);
}

void func_802D8750_729950(GObj* obj) {
    UNUSED s32 pad[3];
    Pokemon* pokemon = GET_POKEMON(obj);

    Pokemon_SetAnimation(obj, &D_802E157C_73277C);
    Pokemon_StartPathProc(obj, NULL);
    pokemon->transitionGraph = D_802E1818_732A18;
    Pokemon_WaitForFlag(obj, POKEMON_PROCESS_FLAG_ANIMATION_ENDED);
    Pokemon_SetState(obj, func_802D8830_729A30);
}

void func_802D87C0_7299C0(GObj* obj) {
    UNUSED s32 pad[3];
    Pokemon* pokemon = GET_POKEMON(obj);

    Pokemon_ForceAnimation(obj, &D_802E15A4_7327A4);
    Pokemon_StartPathProc(obj, NULL);
    pokemon->transitionGraph = D_802E1818_732A18;
    Pokemon_WaitForFlag(obj, POKEMON_PROCESS_FLAG_ANIMATION_ENDED);
    Pokemon_SetState(obj, func_802D8830_729A30);
}

void func_802D8830_729A30(GObj* arg0) {
    Pokemon_StartPathProc(arg0, func_802D88D0_729AD0);
    Pokemon_SetState(arg0, func_802D8868_729A68);
}

#pragma GLOBAL_ASM("asm/nonmatchings/volcano/728490/func_802D8868_729A68.s")

void func_802D88D0_729AD0(GObj* obj) {
    UNUSED s32 pad[3];
    Pokemon* pokemon = GET_POKEMON(obj);

    Pokemon_ResetPathPos(obj);
    Pokemon_FollowPath(obj, 0, 1, 0.15f, 0.0f, MOVEMENT_FLAG_UPDATE_TARGET_POS | MOVEMENT_FLAG_ON_GROUND);
    pokemon->pathProc = NULL;
    pokemon->processFlags |= POKEMON_PROCESS_FLAG_PATH_ENDED;
    omEndProcess(NULL);
}

#pragma GLOBAL_ASM("asm/nonmatchings/volcano/728490/func_802D893C_729B3C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/volcano/728490/func_802D89B0_729BB0.s")

GObj* func_802D8A5C_729C5C(s32 objID, u16 id, WorldBlock* block, WorldBlock* blockB, ObjectSpawn* spawn, PokemonInitData* initData) {
    return Pokemon_SpawnOnGround(objID, id, block, blockB, spawn, &D_802E190C_732B0C);
}
